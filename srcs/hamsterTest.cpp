#include "Response.hpp"

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

// std::string		Response::_generatePanierPage(const std::string& name, const std::string& hamster, const std::string& color)
// {
// 	std::string	htmlPage;

// 	return (htmlPage);
// }
