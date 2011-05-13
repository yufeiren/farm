#include "kindleweb.h"

static void
walkTree(xmlDocPtr doc, xmlNodePtr cur, char *text)
{
	xmlAttr *cur_attr = NULL;
	xmlChar *content;
	
	while (cur != NULL) {
		/* text */
		if (!xmlStrcmp(cur->name, (const xmlChar *)"text")) {
			content = xmlNodeListGetString(doc, cur, 1);
			memcpy(text, content, xmlStrlen(content));
			xmlFree(content);
			return;
		}
		
		walkTree(doc, cur->xmlChildrenNode, text);
		cur = cur->next;
	}

	return;
}

int
html2text(char *text, const char *content)
{
	int ret;
	xmlNodePtr root;
	xmlErrorPtr err;

	htmlParserCtxtPtr parser;

	parser = htmlCreatePushParserCtxt(NULL, NULL, NULL, 0, NULL, 0);

/*	htmlCtxtUseOptions(parser, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET); */
	htmlCtxtUseOptions(parser, HTML_PARSE_RECOVER | HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
	
	
	ret = htmlParseChunk(parser, content, xmlStrlen(content), 0);
	if (ret != 0) {
		err = xmlCtxtGetLastError(parser);
		fprintf(stderr, "htmlParseChunk failure: %d: %s\n", \
			ret, err->message);
	}
	
	ret = htmlParseChunk(parser, NULL, 0, 1);
	if (ret != 0) {
		err = xmlCtxtGetLastError(parser);
		fprintf(stderr, "htmlParseChunk failure 2: %d: %s\n", \
			ret, err->message);
	}
	
	root = xmlDocGetRootElement(parser->myDoc);
	walkTree(parser->myDoc, root, text);
	
	return 0;
}
