<?php
	session_start();
?>
<html>
	<head>
		<title>Cutie webserv 😚 💖</title>
		<meta charset="UTF-8">
        <link href="cgi_achat.css" rel="stylesheet">
	</head>
	<body class="background">
		<div class="paint">
			<a href="../form_gallery.html"><img src="../img/icon_paint.png" width="100px"></a>
		</div>
		<div class="house">
			<a href="../index.html"><img src="../img/icon_house.png" width="100px"></a>
		</div>
		<div class="danse">
			<a href="http://gcasale.free.fr/viva.htm"><img src="../img/icon_dance.gif" width="90px"></a>
		</div>
		<h1>
			Etat de votre panier 🧺
		</h1>
		<p class="cgi">
			avec CGI !
		</p>
		<!-- <div class="hamster">
			<img src="../img/hamster_panier.jpg" width="600px">
		</div> -->
        <div class="informations">
        <?php
        // if(isset($_POST['submit']))
        // {
        //     echo 'bonjour looooool<br>';
        //     deleteItem($item);
        // }
        foreach($_SESSION['cart'] as $cart)
        {
                echo '<br>Votre nom : ', $cart['name'], '<br>';
                echo 'Votre Hamster : ', $cart['hamster'], '<br>';
                echo 'Sa Fourrure : ', $cart['color'], '<br>';
                // echo '<form action="deleteItem()" method="post"><center><input name="submit" type="submit" value="A LA POUBELLE" class="bouton"></center></form>';
        }
        // function deleteItem($item)
        // {
        //     echo 'Bonjour<br>';
        //     unset($_SESSION['cart'][$item]);
        // }
        ?>
		</div>
		<form action="../form.html">
			<input type="submit" value="Go acheter un nouvel ami !" class="bouton" />
		</form>
        <form action="./cgi_destroy.php">
			<input type="submit" value="Jeter votre hamster et reformuler une adoption" class="bouton" />
		</form>
		<form action="../index.html">
			<input type="submit" value="Retour vers la page d'accueil" class="bouton" />
		</form>
	</body>
</html>
