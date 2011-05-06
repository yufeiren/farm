#include "kindleweb.h"

extern MYSQL *conn;

static int id;

static void
parseRssChannelItem(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *key;
	
	cur = cur->xmlChildrenNode;
	
	xmlChar *title;
	xmlChar *link;
	xmlChar *pubDate;
	xmlChar *origLink;
	xmlChar *description;
	xmlChar *encoded;
	
	char query[1024];
	
	while (cur != NULL) {
		/* title - descrption - content:encoded */
		if (!xmlStrcmp(cur->name, (const xmlChar *)"title")) {
			title = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"link")) {
			link = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"origLink")) {
			origLink = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		}
		
		cur = cur->next;
	}
	
	/* query link id */
	memset(query, '\0', 1024);
	snprintf(query, 1024, \
		"INSERT INTO kw_rss_item (rssid, title, origLink) VALUES ('%d', '%s', '%s')", id, title, origLink);
	
	mysql_query(conn, query);
	
	xmlFree(title);
	xmlFree(link);
	xmlFree(origLink);
	
/*	INSERT INTO kw_rss_item (rssid) VALUES (...); */ 
	
	
/*	
| rssid       | int(4)       | NO   |     | NULL    |                |
| title       | varchar(256) | NO   |     |         |                |
| link        | varchar(256) | NO   |     |         |                |
| pubDate     | datetime     | YES  |     | NULL    |                |
| origLink    | varchar(256) | NO   |     |         |                |
| description | mediumblob   | NO   |     | NULL    |                |
| encoded     | mediumblob   | NO   |     | NULL    |                |
*/
	
	return;
}

static void
parseRssChannel(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *key;
	
	cur = cur->xmlChildrenNode;
	
	char title[256];
	
	/* mysql */
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[1024];
	
	/* item */
	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, (const xmlChar *)"item")) {
			parseRssChannelItem(doc, cur);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"title")) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			/* update key to kw_rss_link
			html2text(title, key);
			memset(title, '\0', 256); */
			
			/* query link id */
			memset(query, '\0', 1024);
			snprintf(query, 1024, \
				"UPDATE kw_rss_link SET title = '%s' WHERE id = %d", key, id);
			
			mysql_query(conn, query);
			
			xmlFree(key);
		}
		
		cur = cur->next;
	}
	
	return;
}


int
rssparser(const char *rssfile, int linkid)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	
	id = linkid;

	/* parse rss file */
	doc = xmlParseFile(rssfile);
	if (doc == NULL ) {
		fprintf(stderr, "Document not parsed successfully.\n");
		return 1;
	}
	
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return 1;
	}
	
	/* rss */
	if (xmlStrcmp(cur->name, (const xmlChar *) "rss")) {
		fprintf(stderr,"document of the wrong type, root node != rss\n");
		xmlFreeDoc(doc);
		return 1;
	}
	
	/* head - body */
	cur = cur->xmlChildrenNode;
	
	/* channel */
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"channel"))){
			parseRssChannel(doc, cur);
		}
		
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	
	return 0;
}