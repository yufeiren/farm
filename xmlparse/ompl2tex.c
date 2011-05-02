#include "kd.h"

static int fileseq = 0;

static void
parseOmplHead(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"title"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode,1);
			data2tex(key);
			xmlFree(key);
			return;
		}
		
		cur = cur->next;
	}
	
	return;
}

static void
parseOmplBody(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *title;
	xmlChar *text;
	xmlChar *htmlurl;
	xmlChar *xmlurl;
	xmlChar *type;
	
	char rssfile[128];
	cur = cur->xmlChildrenNode;
	
	for ( ; cur != NULL; fileseq ++) {
/*	while (cur != NULL) { */
		if (cur->xmlChildrenNode != NULL) {
			parseOmplBody(doc, cur);
		} else {
			title = xmlGetProp(cur, "title");
			text = xmlGetProp(cur, "text");
			htmlurl = xmlGetProp(cur, "htmlUrl");
			xmlurl = xmlGetProp(cur, "xmlUrl");
			type = xmlGetProp(cur, "type");
			
			data2tex(title);
			
			/* get rss file */
			memset(rssfile, '\0', 128);
			snprintf(rssfile, 128, "rss%05d", fileseq);
			urltofile(xmlurl, rssfile);
			
			/* parse rss file */
			rss2tex(rssfile);
			
			xmlFree(title);
			xmlFree(text);
			xmlFree(htmlurl);
			xmlFree(xmlurl);
			xmlFree(type);
		}
		
		cur = cur->next;
	}
	
	return;
}

int
ompl2tex(const char *omplfile)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	/* parse ompl file */
	doc = xmlParseFile(omplfile);
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
	
	/* opml */
	if (xmlStrcmp(cur->name, (const xmlChar *) "opml")) {
		fprintf(stderr,"document of the wrong type, root node != opml");
		xmlFreeDoc(doc);
		return 1;
	}
	
	/* head - body */
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"head"))){
			parseOmplHead(doc, cur);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *)"body"))) {
			parseOmplBody(doc, cur);
		}
		
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	
	return 0;
}