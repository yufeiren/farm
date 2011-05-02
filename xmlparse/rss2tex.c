#incude "kd.h"

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
			/* parse html */
			html2tex(key);
			
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
	
	cur2 = cur->xmlChildrenNode;
	cur = cur->xmlChildrenNode;
	
	/* title */
	while (cur2 != NULL) {
		if (!xmlStrcmp(cur2->name, (const xmlChar *)"title")) {
			key = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
			data2tex(key);
			xmlFree(key);
			break;
		}
		
		cur2 = cur2->next;
	}

	/* item */
	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, (const xmlChar *)"item")) {
			parseRssChannelItem(doc, cur);
		}
		
		cur = cur->next;
	}
	
	return;
}


int
rss2tex(const char *rssfile)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

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
		fprintf(stderr,"document of the wrong type, root node != rss");
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