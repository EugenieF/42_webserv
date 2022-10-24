<?php
	session_start();
?>

<!DOCTYPE html>
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
			<center>Remerciements Test apres Form</center>
		</h1>
		<div class="hamster">
			<img src="../html/img/hamster_party.png" width="600px">
		</div>
		<?php
			switch($_SERVER['REQUEST_METHOD'])
			{
				case 'GET':
					$_SESSION['name'] = $_GET['name'];
					$_SESSION['hamster'] = $_GET['hamster'];
					$_SESSION['color'] = $_GET['color'];
					echo 'Votre nom : '.$_GET["name"].'<br>';
					echo 'Le nom de votre futur hamster : '.$_GET["hamster"].'<br>';
					echo 'Vous avez choisi sa couleur magnifique qui est le '.$_GET["color"].'<br>';
					break;
				case 'POST':
					$_SESSION['name'] = $_POST['name'];
					$_SESSION['hamster'] = $_POST['hamster'];
					$_SESSION['color'] = $_POST['color'];
					echo 'Votre nom : '.$_POST["name"].'<br>';
					echo 'Le nom de votre futur hamster : '.$_POST["hamster"].'<br>';
					echo 'Vous avez choisi sa couleur magnifique qui est le '.$_POST["color"].'<br>';
					break;
				default:
					echo "Il faut un minimum d'infos pour obtenir votre petit bebou!\n";
			}
		?>
		</div>
			<form action="../form.html">
				<center><input type="submit" value="Go acheter un nouvel ami !" class="bouton" /></center>
			</form>
			<form action="cgi_achat.php">
				<center><input type="submit" value="Go voir le panier achete !" class="bouton" /></center>
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




