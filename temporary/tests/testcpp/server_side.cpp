
#include <iostream>

#include <poll.h>
#include<netdb.h>
#include<unistd.h>

#include<sys/epoll.h>
#include<sys/socket.h>
#include<sys/types.h>
# include<sys/fcntl.h>

#include <stdio.h>
#include<string.h>
#include<errno.h>
# include <stdlib.h>
# include <arpa/inet.h>

# define DEBUG(x) printf(" ### Debug ### : %s\n", x)

# define MAXEVENT 5
# define PORT 4444
# define BUFSIZE 2048
# define NL '\n'

//Server side

void printerror() {
	fprintf(stderr, "error: %s\n", strerror(errno));
}

int unlock_socket(int socket) {
	int flags = fcntl(socket, F_GETFL);
	if (flags < 0) { return (-1); }
	flags |= O_NONBLOCK;
	if (fcntl(socket, F_SETFL, flags) < 0) { return (-1); }
	return (0);
}

# define MAX_EVENT 10

int main() {
	int s = socket(PF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		printerror();
		return (1);
	}
	DEBUG("Passed socket");

	struct sockaddr_in	addr;
	int					len = sizeof(addr);
	memset(&addr, 0, len);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	char tmp[INET_ADDRSTRLEN + 1];
	memset(tmp, 0, INET_ADDRSTRLEN + 1);
	if (inet_ntop(AF_INET, &addr.sin_addr, tmp, INET_ADDRSTRLEN) == 0){
		printerror();
		abort();}
	std::cout << "Host is : " << tmp << NL;

	int yes=1; 
	// In case the socket wasn't properly closed prior, avoir "already in use" error
	// REUSEPORT is to avoid pb when forking (allow child to reuse same socket)
	if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		printerror();
		close(s);
		return (1);
	}
	if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		printerror();
		close(s);
		return (1);
	}
	DEBUG("Passed bind");

	if (unlock_socket(s) < 0) {
		printerror();
		close(s);
		return (1);
	}

	if (listen(s, 10) == -1) {
		printerror();
		close(s);
		return (1);
	}
	DEBUG("Passed listen");

	int efd;
	struct epoll_event event;
	struct epoll_event *events;

	if ((efd = epoll_create1(0)) < 0) {
		printerror();
		close(s);
		return (1);
	}
	DEBUG("Created epoll");

	memset(&event, 0, sizeof(event));
	event.data.fd = s;
	event.events = EPOLLIN | EPOLLET;

	int control = epoll_ctl(efd, EPOLL_CTL_ADD, s, &event);
	if (control < 0) {
		printerror();
		close(s);
		return (1);
	}
	DEBUG("Added s to epoll");

	events = new struct epoll_event[MAXEVENT];
	DEBUG("Created events");

	int index = 0; // Nombre de requetes possibles

	while (index < 3) {
		DEBUG("Calling epoll_wait");
		int	nb_sockets = epoll_wait(efd, events, MAXEVENT, -1);

		DEBUG("xxxxxxxxxxxxxxxx epoll_wait finished, checking sockets");
		for (int i = 0; i < nb_sockets; i++) { //loop for every connection
			if (events[i].events & EPOLLERR ||
					events[i].events & EPOLLHUP ||
					!(events[i].events & EPOLLIN)) {
				/* random error on epoll_wait occurred */
				printerror();
				close(events[i].data.fd);
				continue; // keep looking for the rest
			}
			else if (s == events[i].data.fd) {
				/* Notification ! Accepting all incomming connection */
				while (1) {
					struct sockaddr_in cl_addr;
					int cl_addr_len = sizeof(cl_addr);
					memset(&cl_addr, 0, cl_addr_len);

					int news = accept(s, (struct sockaddr*)&cl_addr, (socklen_t *)&cl_addr_len);
					if (news < 0) {
						if (errno == EAGAIN ||
								errno == EWOULDBLOCK) { // All incomming connections been processed
							break ;
						}
						else { // An error occurred
							printerror();
							exit(EXIT_FAILURE);
						}
					}

					if (getsockname(news, (struct sockaddr*)&cl_addr, (socklen_t *)&cl_addr_len) < 0) {
						printerror();
						exit(EXIT_FAILURE);
					}
					/* --- Bonus: display stuff --- */
					char addr_buf[INET_ADDRSTRLEN + 1];
					memset(&addr_buf, 0, INET_ADDRSTRLEN + 1);
					inet_ntop(AF_INET, &cl_addr.sin_addr, addr_buf, INET_ADDRSTRLEN);

					std::cout	<< "Accepted connection: fd is " << news
						<< " (host address: " << addr_buf
						<< ", port: " << ntohs(cl_addr.sin_port) << ")"<<NL;
					/* --- End of the bonus --- */
					if (unlock_socket(news) < 0) {
						printerror();
						abort();
					}
					memset(&event, 0, sizeof(event));
					event.data.fd = news;
					event.events = EPOLLIN | EPOLLET;
					if (epoll_ctl(efd, EPOLL_CTL_ADD, news, &event) < 0) {
						printerror();
						abort();
					}
					DEBUG("Added connection to epoll");
				}
			}
			else {
				DEBUG("Reading socket");
				/* data ready to be read on the fd */
				char buf[BUFSIZE + 1];
				std::string str;
				memset(buf, 0, BUFSIZE + 1);

				ssize_t count = read(events[i].data.fd, buf, BUFSIZE);
				while (count) {
					if (count < 0) {
						printerror();
						abort();
					}
					str += buf;
					if (count == BUFSIZE) { // there is still data to read, maybe...
						memset(buf, 0, BUFSIZE + 1);
						count = read(events[i].data.fd, buf, BUFSIZE);
					}
					else { break; } // count < BUFSIZE : eof is inside
				}
				std::cout << "Eof reached. Closed connection on " << events[i].data.fd << NL;
				std::cout << "Message receivred: (len = " << str.size() << ")\n" << str << "\n ====== \n";
				close(events[i].data.fd);
				index++;
				if (index == 3) { break; }
			}
		}
	}
	delete[] events;
	DEBUG("Deleted events");
	close(s);
	DEBUG("Closed s");
	close (efd);
	DEBUG("Closed efd");
	return (0);
}
