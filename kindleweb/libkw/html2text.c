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
