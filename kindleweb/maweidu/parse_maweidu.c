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
		"SELECT id, title, pubDate, description FROM kw_rss_item WHERE rssid = '%d'", rssid);
	
	mysql_query(conn, query);
	
	result = mysql_store_result(conn);
	
	int num_fields;
	MYSQL_FIELD *field;
	num_fields = mysql_num_fields(result);
	
	char tmpfile[256];
	int id;
	
	unsigned long *lengths;
	
	while ((row = mysql_fetch_row(result)) != NULL) {
		printf("%s %s\n", row[0], row[2]);
/*		for(i = 0; i < num_fields; i++) {
			if (i == 0) {
				while(field = mysql_fetch_field(result)) {
					printf("%s ", field->name);
				}
				printf("\n");
			}
			printf("%s  ", row[i] ? row[i] : "NULL");
			
			switch (i) {
			case 0:
				id = atoi(row[i]);
				break;
			case 1:
				
				break;
			default:
				break;
			} */
/*			if (row[i] != NULL) {
				memset(tmpfile, '\0', 256);
				
				snprintf
			}
				urltofile(rssurl, rssfile);
		}*/
		
/*		memset(tmpfile, '\0', 256);
		snprintf(tmpfile, 256, "NYT_%d_%s.html", rssid, row[0]);
		
		urltofile(row[1], tmpfile); */
	}
	
	
	exit(EXIT_SUCCESS);
}