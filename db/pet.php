<?
$username="root";
$password="Cewit365";
$database="menagerie";

mysql_connect(localhost,$username,$password);
@mysql_select_db($database) or die( "Unable to select database");
$query="SELECT * FROM pet";
$result=mysql_query($query);

$num=mysql_numrows($result);

mysql_close();

echo "<b><center>Database Output</center></b><br><br>";

$i=0;
while ($i < $num) {

$name=mysql_result($result,$i,"name");
$owner=mysql_result($result,$i,"owner");
$species=mysql_result($result,$i,"species");
$sex=mysql_result($result,$i,"sex");
$birth=mysql_result($result,$i,"birth");
$death=mysql_result($result,$i,"death");

echo "<b>$name</b><br>owner: $owner<br>species: $species<br>sex: $sex<br>birth: $birth<br>death: $death<br><hr><br>";

$i++;
}

?>
