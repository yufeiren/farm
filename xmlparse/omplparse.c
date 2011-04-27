/* parserss: parse a google-reader ompl xml file */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <libxml/HTMLparser.h>

void
removetags(xmlChar *content)
{
	int intag = 0;
	int cur = 0;
	int cur2 = 0;
	int tagstart = 0;
	int tagend = 0;
	
	char buf[4096];
	
	int c;
	memset(buf, '\0', 4096);
	
	while ((c = *(content + cur++)) != '\0') {
		switch (c) {
		case '<':
			intag = 1;
			if (*(content + cur) == 'p')
				*(buf + cur2++) = '\n';
			break;
		case '>':
			intag = 0;
			break;
		case '&':
			if (memcmp(content + cur, "lt;", 3) == 0) {
				intag = 1;
				cur += 3;
			} else if (memcmp(content + cur, "gt;", 3) == 0) {
				intag = 0;
				cur += 3;
			} else if (memcmp(content + cur, "nbsp;", 5) == 0) {
				cur += 5;
			}
			break;
		case '\n':
			if (intag == 0)
				*(buf + cur2++) = ' ';
			break;
		default:
			if (intag == 0)
				*(buf + cur2++) = c;					
		}
	}
	*(buf + cur2) == '\0';		

	printf("pure txt:\n:%s\n", buf);
	return;
}

void walkTree(xmlNode * a_node)
{
  xmlNode *cur_node = NULL;
  xmlAttr *cur_attr = NULL;
  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
     // do something with that node information, like… printing the tag’s name and attributes
    printf("Got tag : %s\n", cur_node->name);
    for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) {
      printf("  -> with attribute : %s\n", cur_attr->name);
    }
    walkTree(cur_node->children);
  }
}

void
myparsehtml(const xmlChar *content)
{
	htmlParserCtxtPtr parser = htmlCreatePushParserCtxt(NULL, NULL, NULL, 0, NULL, 0);
	
	htmlCtxtUseOptions(parser, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
	
	char *content2 = "<html>\
<body>\
\
<h1>My First Heading</h1>\
\
<p>My first paragraph.</p>\
\
</body>\
</html>";

	int ret;
	
	ret = htmlParseChunk(parser, content2, strlen(content2), 0);
	if (ret != 0) {
		fprintf(stderr, "htmlParseChunk failure: %d\n", ret);
		exit(1);
	}
	
	ret = htmlParseChunk(parser, NULL, 0, 1);
	if (ret != 0) {
		fprintf(stderr, "htmlParseChunk failure: %d\n", ret);
		exit(1);
	}
	
	walkTree(xmlDocGetRootElement(parser->myDoc));
	
	return;
}


void
parseitem(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *title;
	xmlChar *link;
	xmlChar *desp;
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, (const xmlChar *)"title")) {
			title = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("title: %s\n", title);
			xmlFree(title);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"link")) {
			link = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("link: %s\n", link);
			xmlFree(link);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"description")) {
			desp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			/* printf("description: %s\n", desp); */
			/* myparsehtml(desp); */
			removetags(desp);
			xmlFree(desp);
		}
		
		cur = cur->next;
	}
	return;
}

void
parseStory (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *key;
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"link"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("link: %s\n", key);
			xmlFree(key);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"item")) {
	 	    	key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			parseitem(doc, cur);
		}
		
		cur = cur->next;
	}
	
	return;
}

void
parseHead(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"title"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode,1);
			printf("title: %s\n", key);
			xmlFree(key);
		}
		
		cur = cur->next;
	}
	
	return;
}


void
parseBody(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *title;
	xmlChar *text;
	xmlChar *htmlurl;
	xmlChar *xmlurl;
	xmlChar *type;
	
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if (cur->xmlChildrenNode != NULL) {
			parseBody(doc, cur);
		}
		
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"outline"))) {
			title = xmlGetProp(cur, "title");
			text = xmlGetProp(cur, "text");
			htmlurl = xmlGetProp(cur, "htmlUrl");
			xmlurl = xmlGetProp(cur, "xmlUrl");
			type = xmlGetProp(cur, "type");
			
			printf("title: %s; text: %s; type: %s\n", \
				title, text, type);
			printf("htmlurl: %s\n", htmlurl);
			printf("xmlurl: %s\n", xmlurl);
			
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


static void
parseDoc(char *docname)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	
	/* rss */
	if (xmlStrcmp(cur->name, (const xmlChar *) "opml")) {
		fprintf(stderr,"document of the wrong type, root node != opml");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	
	/* head - body */
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"head"))){
			parseHead(doc, cur);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *)"body"))){
			parseBody(doc, cur);
		}
		
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int
main(int argc, char **argv)
{

	char *docname;
		
	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	parseDoc (docname);

	return (1);
}
