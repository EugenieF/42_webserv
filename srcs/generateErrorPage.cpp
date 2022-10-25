#include "Response.hpp"

std::string		Response::_generateErrorPage()
{
	std::string	errorPage;
	std::string	imagePath;

	imagePath = "/img/hamster_banana.png";
	errorPage = \
		"<!DOCTYPE html>\n\
		<html>\n\
				<head>\n\
					<meta charset=\"utf-8\"/>\n\
					<title>No available page 😰</title>\n\
				</head>\n\
			<body bgcolor=\"#FFF8BC\">\n\
				<div class=\"hamster\">\n\
					<center>\n\
						<img src=\"https://i.skyrock.net/8756/19858756/pics/583694619_small.jpg\" alt=\"slide\" style=\"width:50%\">\n\
					</center>\n\
				</div>\n\
				<div>\n\
					<center>\n\
						<div style=\"font-family: Helvetica; font-size: 150%\">\n\
							<br><br>Vous etes ici parce que vous n'avez pas mentionne une page d'erreur specifique.<br>\n\
							C'est pourquoi vous etes sur cette page simple, sobre, mais qui reste fonctionnelle.<br><br><br>\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 250%; font-weight: bold\">\n\
							🥰 Admirez ce hamster heureux. 🥰<br><br><br>\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 150%\">\n\
							Toutefois, vous pouvez connaitre ici le code erreur de cette page qui est :\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 150%; font-weight: bold\">\n"\
							+ getStatusCodeStr() + "<br>\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 150%\">\n\
							qui correspond a cette raison suivante :\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 150%; font-weight: bold\">\n\
							<br>" + g_statusCode[_statusCode] + "\n\
						</div>\n\
					</center>\n\
				</div>\n\
			</body>\n\
		</html>\n";
	return (errorPage);
}

std::string		Response::_generateFormAcceptPage()
{
	std::string	htmlPage;

	htmlPage = \
		"<!DOCTYPE html>\n\
		<html>\n\
			<head>\n\
				<title>Cutie webserv 😚 💖</title>\n\
				<meta charset=\"UTF-8\">\n\
				<link href=\"form_accept.css\" rel=\"stylesheet\">\n\
			</head>\n\
			<body class=\"background\">\n\
				<h1>\n\
					<center>Demande d'adoption ACCEPTED 💝</center>\n\
				</h1>\n\
				<div class=\"hamster\">\n\
					<img src=\"img/hamster_cookie.jpg\" width=\"640\">\n\
				</div>\n\
				<div class=\"requete\">\n\
					<center>Votre requete pour obtenir un hamster a ete accepte.<br></center>\n\
				</div>\n\
				<div class=\"happy\">\n\
					<center>Vous avez rendu un hamster heureux !</center>\n\
				</div>\n\
					<form action=\"./form.html\">\n\
						<center><input type=\"submit\" value=\"Creer une nouvelle demande pour creer une horde d'hamsters\" class=\"bouton\" /></center>\n\
					<form action=\"form_list\">\n\
						<center><input type=\"submit\" value=\"Voir le contenu du panier\" class=\"bouton\" /></center>\n\
					</form>\n\
					<form action=\"./index.html\">\n\
						<center><input type=\"submit\" value=\"Retour vers la page d'accueil\" class=\"bouton\" /></center>\n\
					</form>\n\
				</div>\n\
			</body>\n\
		</html>\n";
	return (htmlPage);
}

std::string		Response::_generateFormOrderPage()
{
	std::string	htmlPage;

	std::string name = "Bobby";
	std::string hamster = "Crotte";
	std::string color = "#00000";
	htmlPage = \
		"<!DOCTYPE html>\n\
		<html>\n\
			<head>\n\
				<title>Cutie webserv 😚 💖</title>\n\
				<meta charset=\"UTF-8\">\n\
				<link href=\"form_accept.css\" rel=\"stylesheet\">\n\
			</head>\n\
			<body class=\"background\">\n\
				<h1>\n\
					<center>Etat de votre panier 🧺</center>\n\
				</h1>\n\
				<div class=\"name\">\n\
					<center>Hamster : " + name + "<br></center>\n\
				</div>\n\
				<div class=\"hamster\">\n\
					<center>Hamster : " + hamster + "<br></center>\n\
				</div>\n\
				<div class=\"color\">\n\
					<center>Hamster : " + color + "<br></center>\n\
				</div>\n\
			</body>\n\
		</html>\n";
	return (htmlPage);
}
