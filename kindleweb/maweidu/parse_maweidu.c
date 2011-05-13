/* parse_nytimes
 * 1. fetch the rss xml file (urltofile)
 * 2. parse rss file (get `id` from table kw_rss_link via link)
 * 	SELECT id FROM kw_rss_link WHERE link = 'url';
 * 3. insert into the latest item into kw_rss_item
 * 	INSERT INTO kw_rss_item (rssid) VALUES (...);
 */

#include <my_global.h>
#include <mysql.h>

#include "kindleweb.h"

MYSQL *conn;

FILE *texfp;

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
	char *rssfile = "maweidu.xml";
	char *texfile;
	
	/* mysql */
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[1024];
	
	int i;
	
	FILE *fp;
	
	int rssid = -1;

	rssurl = texfile = NULL;
	
	int c;
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
	
	if (rssurl == NULL)
		usage("need parameter\n");
	
	/* get the latest xml */
	urltofile(rssurl, rssfile);
	
	/* connect to db */
	conn = mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "kindleweb", "kindleweb", "kindleweb", 0, NULL, 0);

	/* query link id */
	memset(query, '\0', 1024);
	snprintf(query, 1024, \
		"SELECT id FROM kw_rss_link WHERE link = '%s'", rssurl);
	
	mysql_query(conn, query);
	
	result = mysql_store_result(conn);
	
	if ((row = mysql_fetch_row(result)) == NULL) {
		fprintf(stderr, "can not find rss url %s in kw_rss_link", \
			rssurl);
		exit(EXIT_FAILURE);
	}
	
	rssid = atoi(row[0]);
	printf("rss id is %d\n", rssid);
	
	/* parse rss xml */
	rssparser(rssfile, rssid);
	
	/* get html */
	/* query link id */
	memset(query, '\0', 1024);
	snprintf(query, 1024, \
		"SELECT id, title, pubDate, description FROM kw_rss_item WHERE rssid = '%d' and pubDate > '2011-05-11 12:00:00'", rssid);
	
	mysql_query(conn, query);
	
	result = mysql_store_result(conn);
	
	int num_fields;
	MYSQL_FIELD *field;
	num_fields = mysql_num_fields(result);
	
	char tmpfile[256];
	int id;
	
	unsigned long *lengths;
	
	char *title;
	char *desc;
	
	/* open or create ompl file and texfile */
	texfp = fopen("mwd.tex", "w+");
	if (!texfp) {
		perror("File Open tex: ");
		exit(EXIT_FAILURE);
	}
	
	texinit(texfp);
	
	while ((row = mysql_fetch_row(result)) != NULL) {
		printf("%s %s\n", row[0], row[2]);
		
		title = removetags(row[1]);
		desc = removetags(row[3]);
		
		data2tex("----------------------------\r\n");
		data2tex(title);
		data2tex(row[2]);
		data2tex(desc);
		
		free(title);
		free(desc);
	}
	
	texclose(texfp);
	
	fclose(texfp);
	
	exit(EXIT_SUCCESS);
}