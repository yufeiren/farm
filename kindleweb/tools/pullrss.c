/* pullrss from the web accroding to the `link` of kw_rss_link
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

	/* query link and id */
	memset(query, '\0', 1024);
	snprintf(query, 1024, \
		"SELECT id, link FROM kw_rss_link");
	
	mysql_query(conn, query);
	
	result = mysql_store_result(conn);
	
	i = 1;
	while ((row = mysql_fetch_row(result)) != NULL) {
		memset(xmlfile, '\0', 32);
		snpintf(xmlfile, 32, "rss%05d.xml", i);
		
		/* get the latest xml */
		urltofile(row[1], xmlfile);
	}
	
	mysql_close(conn);
	
	exit(EXIT_SUCCESS);
}