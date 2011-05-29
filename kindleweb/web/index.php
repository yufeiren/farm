<?
$username="kindleweb";
$password="kindleweb";
$database="kindleweb";

mysql_connect(localhost,$username,$password);
@mysql_select_db($database) or die( "Unable to select database");
$query="SELECT * FROM kw_rss_link";
$result=mysql_query($query);

$num=mysql_numrows($result);

mysql_close();

echo "<html>"
echo "<b><center>Database Output</center></b><br><br>";

$i=0;
while ($i < $num) {

$title=mysql_result($result,$i,"title");
$link=mysql_result($result,$i,"link");

echo "<br>$title $link<br>";

$i++;
}

echo "<\html>"

?>
