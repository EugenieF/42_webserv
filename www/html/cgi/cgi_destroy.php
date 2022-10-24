<?php
	session_start();
?>
<html>
	<head>
		<title>Cutie webserv 😚 💖</title>
		<meta charset="UTF-8">
	</head>
    <style type="text/css">
    body {
    margin: 0;
    padding: 0;
    }

    @font-face {
    font-family: berry;
    src: url(../fonts/Berry_Merry.otf);
    }

    @font-face {
    font-family: peralta;
    src: url(../fonts/Peralta-Regular.ttf);
    }

    @font-face {
	font-family: NotoSerifKR_R;
	src: url(../fonts/NotoSerifKR-Bold.otf)
    }

    @font-face {
	font-family: NotoSerifKR_L;
	src: url(../fonts/NotoSerifKR-Light.otf)
    }

    h1 {
    color: #d71b3b;
    font-family: peralta;
    }

    .hamster {
    text-align: center;
    }

    .bouton {
	margin: 20px;
	background-color: #FF69B4;
	color: white;
	text-align: center;
	font-family: berry;
	font-size: 150%;
	box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2), 0 6px 20px 0 rgba(0,0,0,0.19);
    }

    </style>
	<body bgcolor="#D6EFED">
		<h1>
			<center>Suppression de votre panier 💔</center>
		</h1>
		<div class="hamster">
			<img src="../img/hamster_trash.png" width="600px">
		</div>
		<div><center>
        <?php
	    	if (isset($_SESSION['name']))
            {
				session_destroy();
				echo "En esperant que vous avez apprecie cette experience d'achat! A bientot!";
	    	}
            else
            {
	    		echo 'Vous etes inconnu parmi nos acheteurs pour supprimer votre panier!';
	    	}
        ?>
		</center></div>
			<form action="../form.html">
				<center><input type="submit" value="Go acheter un nouvel ami !" class="bouton" /></center>
			</form>
			<form action="./cgi_achat.php">
				<center><input type="submit" value="Verifier si le panier a bien ete detruit" class="bouton" /></center>
			</form>
			<form action="../index.html">
				<center><input type="submit" value="Retour vers la page d'accueil" class="bouton" /></center>
			</form>
		</div>
	</body>
</html>
