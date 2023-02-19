<!DOCTYPE html>
<html>
<meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="css/bootstrap.min.css">
    <link rel="stylesheet" href="style.css">
<link rel="apple-touch-icon" sizes="120x120" href="apple-touch-icon.png">
<link rel="icon" type="image/png" sizes="32x32" href="favicon-32x32.png">
<link rel="icon" type="image/png" sizes="16x16" href="favicon-16x16.png">
<link rel="manifest" href="site.webmanifest">
<link rel="mask-icon" href="safari-pinned-tab.svg" color="#5bbad5">
<meta name="msapplication-TileColor" content="#da532c">
<meta name="theme-color" content="#ffffff">
<link rel="stylesheet" href="formulaire.css">
<head>
    <title>Nous contacter</title>
</head>
<body class="pop">
<header class="ba">
    <a href="index.html" class="logo2">Digit-Dash</a>
    <div class="menuToggle2" onclick="toggleMenu2();"></div>
    <ul class="navbar">
    <li><a href="index.html" class="m" onclick="toggleMenu();">Accueil</a></li>
    <li><a href="Gameplay.html" class="m" onclick="toggleMenu();">Gameplay</a></li>
    <li><a href="Chat.html" class="m" onclick="toggleMenu();">Le chat</a></li>
    <li><a href="nc.php" class="m" onclick="toggleMenu();">Nous contacter</a></li>
    <li><a href="Ressources.html" class="m" onclick="toggleMenu();">Ressources</a></li>
    <a href="" class="Jouer" onclick="toggleMenu();">Jouer</a>
    </ul>
</header>
<script type="text/javascript">


    function toggleMenu(){
        const menuToggle = document.querySelector('.menuToggle');
        const navbar = document.querySelector('.navbar');
        navbar.classList.toggle('active');
        menuToggle.classList.toggle('active');
    }
</script>
    <form method="POST" class="post">
  <h3>CONTACTEZ NOUS</h3>
  <p type="Pseudo:"><input type="text" name="pseudo" id="pseudo" placeholder="Votre pseudo" required><br/></p>
  <p type="Email:"><input type="text" name="email" id="email" placeholder="Votre Email" required><br/></p>
  <p type="Message:"><textarea type="text" name="message" id="message" placeholder="Votre message" required></textarea><br/></p>
  <input type="submit" class="kaka" name="formsend" id ="formsend">
</form>

 <?php
    include'database.php';
    global $db;
    $q=$db->prepare("INSERT INTO `users`(`pseudo`, `email`, `message`) VALUES (:pseudo,:email,:message)");
    if (isset($_POST['formsend'])) 
    {
        $pseudo=$_POST['pseudo'];
        $email=$_POST['email'];
        $message=$_POST['message'];
    
        if(!empty($pseudo) && !empty($message) && !empty($email))
        {
          
    $q->execute([
        'pseudo'=>$pseudo,
        'email'=>$email,
        'message'=>$message
    ]);  
        }
    }
    
 ?>
</body>
</html>