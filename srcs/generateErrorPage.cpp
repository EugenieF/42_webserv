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

std::string		Response::_generateAcceptFormPage()
{
	std::string	htmlPage;

	htmlPage =  \
	"<!DOCTYPE html>\n\
	<html>\n\
		<head>\n\
			<title>Cutie webserv 😚 💖</title>
			<meta charset=\"UTF-8\">
			<link href=\"form_accept.css\" rel=\"stylesheet\">
		</head>
		<body class=\"background\">
			<h1>
				<center>Demande d'adoption ACCEPTED 💝</center>
			</h1>
			<div class=\"hamster\">
				<img src=\"img/hamster_cookie.jpg\" width=\"640\">
			</div>
			<div class=\"requete\">
				<center>Votre requete pour obtenir un hamster a ete accepte.<br></center>
			</div>
			<div class=\"happy\">
				<center>Vous avez rendu un hamster heureux !</center>
			</div>
				<form action=\"./form.html\">
					<center><input type=\"submit\" value=\"Creer une nouvelle demande pour creer une horde d'hamsters\" class=\"bouton\" /></center>
				<form action=\"./index.html\">
					<center><input type=\"submit\" value=\"Retour vers la page d'accueil\" class=\"bouton\" /></center>
				</form>
			</div>\n\
		</body>\n\
	</html>\n";
	return (htmlPage);
}
