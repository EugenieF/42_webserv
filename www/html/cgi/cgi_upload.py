#!/usr/bin/python

import cgi, os
import cgitb; cgitb.enable()

# permet de recevoir la request de l'user
form = cgi.FieldStorage()

# on recupere le nom du fichier sense etre uploade
f = form['nude']

# on regarde si le fichier a bien ete uploade
if f.filename:
   # on supprimer le chemin d'accès principal
   # du nom de fichier pour éviter les problemes
   fn = os.path.abspath(f.filename)
   # fn = os.path.basename(f.filename)
   # message = fn
   # wb = on ecrit et on le transforme en binaire
   open(fn, 'wb').write(f.file.read())
   message = 'The file "' + fn + '" was uploaded successfully'
else:
   message = 'Pas de nude :( Sadly!'

print ("""
<!DOCTYPE html>
<html>
	<head>
		<title>Cutie webserv 😚 💖</title>
		<meta charset="UTF-8">
		<link href="../form_upload_ok.css" rel="stylesheet">
	</head>
	<body class="background">
		<div class="icon">
			<a href="../form_order"><img src="../img/icon_panier_no_cgi.png" width="100px"></a>
		</div>
		<div class="paint">
			<a href="../form_gallery"><img src="../img/icon_paint.png" width="100px"></a>
		</div>
		<div class="house">
			<a href="../index.html"><img src="../img/icon_house.png" width="100px"></a>
		</div>
		<div class="danse">
			<a href="http://gcasale.free.fr/viva.htm"><img src="../img/icon_dance.gif" width="90px"></a>
		</div>
		<h1>
			<center>Envoi de votre nude ACCEPTED 💝</center>
		</h1>
		<div class="hamster">
			<img src="../img/hamster_nude.jpg" width="500px">
		</div>
		<div class="requete">
			<center>Ton envoi de votre nude a ete accepte.<br></center>
		</div>
		<div class="happy">
			<center>Vous nous rendez tres tres heureux 😏 !</center>
		</div>
			<form action="../form.html">
				<center><input type="submit" value="Créer une nouvelle demande pour réveiller nos sens" class="bouton" /></center>
			</form>
			<form action="../index.html">
				<center><input type="submit" value="Retour vers la page d'accueil" class="bouton" /></center>
			</form>
		</div>
	</body>
</html>
""")