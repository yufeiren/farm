#include "kindleweb.h"

extern MYSQL *conn;

static int id;

static void
parseRssChannelItem(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *key;
	
	xmlNodePtr cur2;
	int havecontent = 0;
	
	cur2 = cur->xmlChildrenNode;
	cur = cur->xmlChildrenNode;
	
	/* choose description or content:encoded */
	while (cur2 != NULL) {
		if (!xmlStrcmp(cur2->name, (const xmlChar *)"encoded")) {
			havecontent = 1;
			break;
		}
		
		cur2 = cur2->next;
	}
	
	while (cur != NULL) {
		/* title - descrption - content:encoded */
		if ( (!xmlStrcmp(cur->name, (const xmlChar *)"title"))
		   || (!xmlStrcmp(cur->name, (const xmlChar *)"description") && (havecontent == 0))
		   || (!xmlStrcmp(cur->name, (const xmlChar *)"encoded")) )
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			/* parse html
			html2tex(, key); */
			
			xmlFree(key);
		}
		
		cur = cur->next;
	}
	
	return;
}

static void
parseRssChannel(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *key;
	xmlNodePtr cur2;
	
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
			key = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
			/* update key to kw_rss_link */
			
			html2text(title, key);
			memset(title, '\0', 256);
			
			/* query link id */
			memset(query, '\0', 1024);
			snprintf(query, 1024, \
				"UPDATE kw_rss_link SET title = '%s' FROM  WHERE id = %d", title, id);
			
			mysql_query(conn, query);
			
			xmlFree(key);
			break;
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