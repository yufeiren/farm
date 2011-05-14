/* formatitem to tex file
 * select id from kw_rss_link;
 * select encoded/description from kw_rss_item
 * 	where rssid =  and pubDate < and pubDate > ORDER BY pubDate 
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
"usage: formatitem [ options ]\n"
"options: -s \t\t\tstart date\n"
"         -e \t\t\tend date\n"
);

	if (msg[0] != 0)
		fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}


/* fetch item from kw_rss_item */
static void
fetchitem(int rssid, char *start, char *end)
{
	/* mysql */
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[1024];
	
	char *title;
	char *pubdate;
	char *content;
	
	/* query link id */
	memset(query, '\0', 1024);
	snprintf(query, 1024, \
		"SELECT title, pubDate, description, encoded FROM kw_rss_item WHERE rssid = %d AND pubDate >= '%s' AND pubDate <= '%s' ORDER BY pubDate", \
		rssid, start, end);
	
	mysql_query(conn, query);
	
	result = mysql_store_result(conn);
	
	while ((row = mysql_fetch_row(result)) != NULL) {
		title = removetags(row[0]);
		pubdate = row[1];
		
		if (strlen(row[3]) != 0) {
			content = removetags(row[3]);
		} else {
			content = removetags(row[2]);
		}
		
		data2tex("----------------------------\r\n", TEX_FONT_Large);
		data2tex(title, TEX_FONT_huge);
		data2tex(pubdate, TEX_FONT_normalsize);
		data2tex("---\r\n", TEX_FONT_normalsize);
		data2tex(content, TEX_FONT_Large);
		data2tex("---\r\n", TEX_FONT_normalsize);
		
		free(title);
		free(content);
	}
	
	mysql_free_result(result);
	
	return;
}

int
main(int argc, char **argv)
{
	/* mysql */
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[1024];
	
	int i;
	
	int rssid = -1;
	
	char *startdate;
	char *enddate;
	
	startdate = enddate = NULL;
	
	int c;
	while ((c = getopt(argc, argv, "s:e:")) != -1) {
		switch (c) {
		case 's':
			startdate = optarg;
			break;
		case 'e':
			enddate = optarg;
			break;
		default:
			usage("wrong parameter\n");
			break;
		}
	}
	
	if (startdate == NULL)
		startdate = "1000-01-01";
	if (enddate == NULL)
		enddate = "9999-12-31";
	
	char texfile[128];
	
	snprintf(texfile, 128, "%s_%s_%s.tex", "rss", startdate, enddate);
	
	texfp = fopen(texfile, "w+");
	if (!texfp) {
		perror("File Open tex: ");
		exit(EXIT_FAILURE);
	}
	
	texinit(texfp);
	
	/* connect to db */
	conn = mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "kindleweb", "kindleweb", "kindleweb", 0, NULL, 0);

	/* query link id */
	memset(query, '\0', 1024);
	snprintf(query, 1024, "SELECT id, title FROM kw_rss_link");
	
	mysql_query(conn, query);
	
	result = mysql_store_result(conn);
	
	char *title;
	
	while ((row = mysql_fetch_row(result)) != NULL) {
		rssid = atoi(row[0]);
		title = removetags(row[1]);
		
		data2tex(title, TEX_FONT_Huge);
		free(title);
		
		fetchitem(rssid, startdate, enddate);
	}
	
	mysql_free_result(result);
	
	texclose(texfp);
	
	fclose(texfp);
	
	exit(EXIT_SUCCESS);
}
