#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sock = 0;
    long valread;
    (void)valread;
    struct sockaddr_in serv_addr;
    
    // char hello[] = "POST /data HTTP/1.1\r\nhost:    www.yoursite.com  	\r\nTransfer-Encoding: chunked	\r\nContent-TYPE: application/json \r\n\r\n26\r\nVoici les données du premier morceau \r\n";
    // char hello[] = "1F\r\net voici deux derniers morceaux\r\n0\r\n\r\n";
    // char hello[] = "POST /test/page.php HTTP/1.1\r\nhost:    www.yoursite.com  \r\nContent-Length: 8\r\nContent-TYPE: text/html \r\n\r\nRESPONSE\r\n";
    char hello[] = "POST /www/test HTTP/1.1\r\nHOST: exemple.com:8080	\r\nContent-length: 17\r\nCookie: SID=elfl59dngq5ba0i5; surname=bibi; love=hamster\r\nContent-TYPE: 	application/json \r\n\r\nHello from client\r\n";
    // char hello[] = "POST /www/test HTTP/1.1\r\nHOST: exemple.com:8080	\r\nContent-length: 17\r\nCookie: name=Manu; hamster=Kenya; filename=test\r\nContent-TYPE: 	application/json \r\n\r\nHello from client\r\n";
    // char hello[] = "POST /test HTTP/1.1\r\nHOST: :8080 \r\nCOntent-length: 70\r\nContent-TYPE: multipart/form-data; boundary=\"test\"\r\n\r\n--test\r\nContent-Disposition: form-data; name=\"file1\"; filename=\"a.txt\"\r\nContent-Type: text/plain\r\n\r\nContent of a.txt.\r\n";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
