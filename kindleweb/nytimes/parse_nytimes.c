/* parse_nytimes
 * 1. fetch the rss xml file (urltofile)
 * 2. parse rss file (get `id` from table kw_rss_link via link)
 * 	SELECT id FROM kw_rss_link WHERE link = 'url';
 * 3. insert into the latest item into kw_rss_item
 * 	INSERT INTO kw_rss_item (rssid) VALUES (...); 
 */

/* #include "kindleweb.h" */
#include "kd.h"

#include <my_global.h>
#include <mysql.h>

static void
usage(const char *msg)
{
	printf(
"usage: parse_nytimes [ options ]\n"
"options: -u \t\t\trss url\n"
);

	if (msg[0] != 0)
		fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
	char *rssurl;
	char *texfile;
	
	/* mysql */
	MYSQL *conn;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[1024];
	
	FILE *fp;
	
	int rssid = -1;

	rssurl = texfile = NULL;
	
	while ((c = getopt(argc, argv, "u:")) != -1) {
		switch (c) {
		case 'u':
			rssurl = optarg;
			break;
		default:
			usage("wrong parameter\n");
			break;
		}
	}
	
	if ((rssurl == NULL) || (texfile == NULL))
		usage("need parameter\n");
	
	/* get the latest xml
	urltofile(rssurl, "nytimes.xml"); */
	
	/* connect to db */
	conn = mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "kindleweb", "kindleweb", "kw_rss_link", 0, NULL, 0);

	/* query link id */
	memset(query, '\0', 1024);
	snprintf(query, 1024, \
		"SELECT id FROM kw_rss_link WHERE link = '%s';", rssurl);
	
	mysql_query(conn, "SELECT id FROM pet WHERE ");

	result = mysql_store_result(conn);

	if ((row = mysql_fetch_row(result)) == NULL) {
		fprintf(stderr, "can not find rss url %s in kw_rss_link", \
			rssurl);
		exit(EXIT_FAULURE);
	}
	
	rssid = atoi(row[0]);
	printf("rss id is %d\n", rssid);
	
	/* parse rss xml */
	
	
	exit(EXIT_SUCCESS);
}