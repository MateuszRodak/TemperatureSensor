<html>
<head>
</head>
<body>

<?php

$czujnik = 1;


if ($_SERVER["REQUEST_METHOD"] == "POST") {
 

  if (empty($_POST["wybrczuj"])) {
  } else {
    $wybrczuj = test_input($_POST["wybrczuj"]);
  }
}

function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
}
?>


<form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">  
  
  <input type="text" name="wybrczuj" value="wpisz numer czujnika"> 
  <br><br>
  <input type="submit" name="submit" value="Submit">  
</form>

<?php
$czujnik= $wybrczuj;
echo "Wybrany czujnik:";
echo $czujnik;
 echo '<br>';
 echo '<br>';

?>


<?php

$POLACZENIE = mysqli_connect('localhost','DB_username','DB_password', 'DB_tablename');

 if ( !mysqli_connect('localhost','DB_username','DB_password', 'DB_tablename') ) {
    echo 'Nie moge polaczyc sie z baza danych';
 	 exit (0);
 }
 else {
 echo 'polaczono z baza danych';
 echo '<br>';
 echo '<br>';
}


  $sql2 = "SELECT * FROM DANE_Z_CZUJNIKOW2 WHERE NR_CZUJNIKA =".$czujnik." ORDER BY CZAS DESC";
   $retval2 = mysqli_query($POLACZENIE, $sql2);
 


$stan = true;

   while($row = mysqli_fetch_array($retval2, MYSQL_ASSOC)) {
	   
	   if($stan) {  echo "<br> -------- NAJNOWSZA WARTOSC: ---------<br> ";}
	
      echo "counter: {$row['ID']}  <br> ".
           "czujnik: {$row['NR_CZUJNIKA']} <br> ".
           "czas: {$row['CZAS']} <br> ".
           "temp: {$row['TEMPERATURA']} <br> ".
           "swiatlo: {$row['SWIATLO']} <br> ".
           "nastawiona temp: {$row['NASTAWIONA_TEMP']} <br> ".
           "stan: {$row['STAN']} <br> ".
           "--------------------------------<br>";	
		   
		if($stan) { echo "<br><br><br><br>--------------------------------<br>";}
		$stan = false;
   }
   

mysql_close($POLACZENIE);
echo '<br>';
echo '<br>';
echo "mysql close";

 ?> 

</body>
</html> 