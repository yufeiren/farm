/* parserss which already downloaded
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
"usage: pullrss\n"
);

	if (msg[0] != 0)
		fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
	/* mysql */
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[1024];

	char xmlfile[32];
	int i;

	/* connect to db */
	conn = mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "kindleweb", "kindleweb", \
				"kindleweb", 0, NULL, 0);
	
	/* query id */
	memset(query, '\0', 1024);
	snprintf(query, 1024, \
		"SELECT id FROM kw_rss_link");
	
	mysql_query(conn, query);
	
	result = mysql_store_result(conn);
	
	int id;
	
	while ((row = mysql_fetch_row(result)) != NULL) {
		memset(xmlfile, '\0', 32);
		id = atoi(row[0]);
		snprintf(xmlfile, 32, "rss%05d.xml", id);
		
		/* parserss */
		rssparser(xmlfile, id);
	}
	
	mysql_close(conn);
	
	exit(EXIT_SUCCESS);
}