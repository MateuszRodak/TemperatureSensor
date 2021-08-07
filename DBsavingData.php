<?php print_r($_POST); 

echo "ROZPOCZYNA SIE SKRYPT PHP";
echo "\n";

$data = file_get_contents('php://input');

echo "\n";
echo $data;
echo "\n";
echo "\n";
$POLACZENIE = mysqli_connect('localhost','DB_username','DB_password', 'DB_tablename');

 if ( !mysqli_connect('localhost','DB_username','DB_password', 'DB_tablename') ) {
    echo 'Nie moge polaczyc sie z baza danych';
 	 exit (0);
 }
 else {
 echo 'polaczono z baza danych';

}

echo "\n";
echo "POBRANA TABLICA:";
echo "\n";
echo "\n";

$arrayphp = json_decode($data,true);
print_r($arrayphp);

echo "\n";

$i =0;

echo "WYSYLANIE DO BAZY DANYCH";

foreach ($arrayphp as $v) {
		echo "\n";	echo "\n";	echo "\n";
    print_r($arrayphp[$i]["Num"]);echo "\n";
	print_r($arrayphp[$i]["time"]);echo "\n";
	print_r($arrayphp[$i]["Temp"]);echo "\n";
	print_r($arrayphp[$i]["Light"]);echo "\n";
	print_r($arrayphp[$i]["dTemp"]);echo "\n";
	print_r($arrayphp[$i]["Stan"]);echo "\n";
    
	echo "\n";
		
	$sql = "INSERT INTO `DANE_Z_CZUJNIKOW2`(`NR_CZUJNIKA`, `CZAS`, `TEMPERATURA`, `SWIATLO`, `NASTAWIONA_TEMP`, `STAN`) 
	VALUES (".$arrayphp[$i]["Num"].",'".date('Y-m-d H:i:s',($arrayphp[$i]["time"]))."',".$arrayphp[$i]["Temp"].",".$arrayphp[$i]["Light"].",".
	$arrayphp[$i]["dTemp"].",".$arrayphp[$i]["Stan"].")";
	
	echo $sql;
	$retval = mysqli_query($POLACZENIE, $sql);
	$i++;
}

mysql_close($POLACZENIE);
echo "mysql close";
echo "KONIEC PRACY PHP";
 
?>