<?
$username="kindleweb";
$password="kindleweb";
$database="kindleweb";

mysql_connect(localhost,$username,$password);
@mysql_select_db($database) or die( "Unable to select database");
$query="SELECT * FROM kw_item_link";
$result=mysql_query($query);

$num=mysql_numrows($result);

mysql_close();

echo "<b><center>Database Output</center></b><br><br>";

$i=0;
while ($i < $num) {

# $name=mysql_result($result,$i,"name");
echo "<br>$result";

$i++;
}

?>
