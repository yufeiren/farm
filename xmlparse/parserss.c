/* parserss: parse a rss type xml file */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <libxml/HTMLparser.h>

static int fd;
static FILE *fp;

char *
removetags(xmlChar *content)
{
	int intag = 0;
	int cur = 0;
	int cur2 = 0;
	int tagstart = 0;
	int tagend = 0;
	
	char *buf;
	
	int c;
	buf = (char *) malloc(409600);
	if (buf == NULL) {
		printf("malloc fail\n");
		exit(0);
	}
	memset(buf, '\0', 409600);
	
	while ((c = *(content + cur++)) != '\0') {
		switch (c) {
		case '<':
			intag = 1;
			if (*(content + cur) == 'p') {
				*(buf + cur2++) = '\r';
				*(buf + cur2++) = '\n';
				*(buf + cur2++) = '\r';
				*(buf + cur2++) = '\n';
			}
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
			} else if (memcmp(content + cur, "bull;", 5) == 0) {
				cur += 5;
				*(buf + cur2++) = '*';
			}
			break;
		case '\t':
			break;
		case '\\':
		case '$':
		case '{':
		case '}':
		case '~':
		case '^':
		case '_':
		case '%':
		case '#':
			/* TEX sensitive character:
			 * \ { } $ ^ _ % ~ # &
			 */
			break;
		case '\n':
			if (intag == 0)
				*(buf + cur2++) = ' ';
			break;
		default:
			if (intag == 0)
				*(buf + cur2++) = c;
			if (c == '&') {
				printf("fuck\n");
				exit(0);
			}
			break;
		}
	}
	*(buf + cur2) == '\0';

/*	printf("pure txt:\n:%s\n", buf); */
	
	return buf;
}

void walkTree(xmlDocPtr doc, xmlNode * a_node)
{
  xmlNode *cur_node = NULL;
  xmlAttr *cur_attr = NULL;
  
  xmlChar *content;
  
  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
     // do something with that node information, like… printing the tag’s name and attributes
    printf("Got tag : %s: content %s\n", cur_node->name, cur_node->content);

	if (!xmlStrcmp(cur_node->name, (const xmlChar *)"text")) {
		content = xmlNodeListGetString(doc, cur_node, 1);/*->xmlChildrenNode*/
		fprintf(fp, "\\Huge{content: %s}\r\n\r\n", content);
		xmlFree(content);
/*		fprintf(fp, "\\Huge{%s}\r\n\r\n", cur_node->content); */
	}
    for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) {
      printf("  -> with attribute : %s\n", cur_attr->name);
    }
    walkTree(doc, cur_node->children);
  }
}

void
myparsehtml(const xmlChar *content)
{
	xmlErrorPtr err;

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
	xmlNodePtr root;
	
	printf("content len: %d\n", xmlStrlen(content));
	
	ret = htmlParseChunk(parser, content, xmlStrlen(content), 0);
	if (ret != 0) {
		err = xmlCtxtGetLastError(parser);
		fprintf(stderr, "htmlParseChunk failure: %d: %s\n", \
			ret, err->message);
		return;
	}
	
	ret = htmlParseChunk(parser, NULL, 0, 1);
	if (ret != 0) {
		err = xmlCtxtGetLastError(parser);
		fprintf(stderr, "htmlParseChunk failure 2: %d: %s\n", \
			ret, err->message);
		return;
	}
	
/*	walkTree(xmlDocGetRootElement(parser->myDoc)); */
	root = xmlDocGetRootElement(parser->myDoc);
	walkTree(parser->myDoc, root);
	
	return;
}


void
parseitem(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *title;
	xmlChar *link;
	xmlChar *desp;
	xmlChar *content;
	char *newcont;
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, (const xmlChar *)"title")) {
			title = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			newcont = removetags(title);
			fprintf(fp, "\\section{%s}\r\n\r\n", newcont);
			xmlFree(title);
			free(newcont);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"link")) {
			link = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			newcont = removetags(link);
			/* fprintf(fp, "link: %s\r\n\r\n", newcont); */
			xmlFree(link);
			free(newcont);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"description")) {
			/* XML_CDATA_SECTION_NODE = 4 */
			printf("desp element type: %d\n", cur->xmlChildrenNode->type);
			desp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
/*			myparsehtml(desp); */
			newcont = removetags(desp);
			fprintf(fp, "\\Huge{%s}\r\n\r\n", newcont);
			fprintf(fp, "\r\n\r\n");
			xmlFree(desp);
			free(newcont);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"encoded")) {
			/* printf("get content:encoded\n"); */
			printf("encoded element type: %d\n", cur->xmlChildrenNode->type);
			content = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
/*			myparsehtml(content); */
			newcont = removetags(content);
			fprintf(fp, "\\Huge{content: %s}\r\n\r\n", newcont);
			fprintf(fp, "\r\n\r\n");
			xmlFree(content);
			free(newcont);
		} else if (!xmlStrcmp(cur->name, (const xmlChar *)"pubDate")) {
			content = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("pubDate: %s\n", content);
			fprintf(fp, "\\emph{Date: %s}\r\n\r\n", content);
			fprintf(fp, "\r\n\r\n");
			xmlFree(content);
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

static void
parseDoc(char *docname) {

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
	if (xmlStrcmp(cur->name, (const xmlChar *) "rss")) {
		fprintf(stderr,"document of the wrong type, root node != rss");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	
	/* channel */
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"channel"))){
			parseStory (doc, cur);
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
	char *outfile;
		
	if (argc <= 2) {
		printf("Usage: %s docname outputfile\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	outfile = argv[2];

	fp = fopen(argv[2], "w+");
	if (fp == NULL) {
		printf("cannot open output file: %s\n", outfile);
		exit(1);
	}
	
	fprintf(fp, "\\documentclass[a4paper,12pt]{article}\n");
	fprintf(fp, "\\usepackage {geometry}\n");
	fprintf(fp, "\\usepackage {fancyhdr}\n");
	fprintf(fp, "\\usepackage {amsmath ,amsthm , amssymb}\n");
	fprintf(fp, "\\usepackage {graphicx}\n");
	fprintf(fp, "\\usepackage {hyperref}\n");
	fprintf(fp, "\\usepackage {CJK}\n");
/*	fprintf(fp, "\\usepackage [utf8x]{inputenc}\n"); */

	fprintf(fp, "\\begin {document}\n");
	fprintf(fp, "\\begin {CJK}{UTF8}{gkai}\n");
	
	parseDoc (docname);
	
	fprintf(fp, "\n");
	fprintf(fp, "\\end {CJK}\n");
	fprintf(fp, "\\end {document}\n");

	fclose(fp);
	
	return (1);
}
