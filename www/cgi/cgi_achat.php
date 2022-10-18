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
    src: url(../html/fonts/Berry\ Merry.otf);
    }

    @font-face {
    font-family: peralta;
    src: url(../html/fonts/Peralta-Regular.ttf);
    }

    @font-face {
	font-family: NotoSerifKR_R;
	src: url(../html/fonts/NotoSerifKR-Bold.otf)
    }

    @font-face {
	font-family: NotoSerifKR_L;
	src: url(../html/fonts/NotoSerifKR-Light.otf)
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
			<center>Etat de votre panier 🧺</center>
		</h1>
		<div class="hamster">
			<img src="../html/img/hamster_panier.jpg" width="600px">
		</div>
        <?php
	    	if (isset($_SESSION['name']))
            {
	    		echo $_SESSION['name'];
                if (isset($_SESSION['hamster']))
                {
                    echo ' a bien reserve '.$_SESSION['hamster'];
                }
                else
                {
                    echo " n'a pas encore choisi son futur compagnon de vie";
                }
	    	}
            else
            {
	    		echo 'Vous etes inconnu parmi nos acheteurs!';
	    	}
        ?>
		</div>
			<form action="../html/form.html">
				<center><input type="submit" value="Go acheter un nouvel ami !" class="bouton" /></center>
			</form>
            <form action="./cgi_destroy.php">
				<center><input type="submit" value="Jeter son hamster et reformuler une adoption" class="bouton" /></center>
			</form>
			<form action="../html/index.html">
				<center><input type="submit" value="Retour vers la page d'accueil" class="bouton" /></center>
			</form>
		</div>
	</body>
</html>

