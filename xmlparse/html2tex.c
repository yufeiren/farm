#include "kd.h"

static int imgseq = 0;

static void
walkTree(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlAttr *cur_attr = NULL;
	
	xmlChar *content;
	xmlChar *tagstr;
	
	int ret;
	
	char imgfile[128];

	while (cur != NULL) {
		/* text */
		if (!xmlStrcmp(cur->name, (const xmlChar *)"text")) {
			content = xmlNodeListGetString(doc, cur, 1);
			data2tex(content);
			xmlFree(content);
		}
		
		/* img */
		if (!xmlStrcmp(cur->name, (const xmlChar *)"img")) {
			/* download a new img */
			for (cur_attr = cur->properties; \
				cur_attr; \
				cur_attr = cur_attr->next)
			{
				if (!xmlStrcmp(cur_attr->name, (const xmlChar *)"src"))
				{
					tagstr = xmlNodeListGetString(doc, cur_attr->xmlChildrenNode, 1);
					/* get img file */
					memset(imgfile, '\0', 128);
					if (memcmp(tagstr + xmlStrlen(tagstr) - 3, "jpg", 3) == 0)
					{
						snprintf(imgfile, 128, "img%05d.jpg", ++ imgseq);
					
					
						printf("%s: %s\n", imgfile, tagstr);
						if (urltofile(tagstr, imgfile) == 0)
							img2tex(imgfile);
					}
					
					xmlFree(tagstr);
				}
    			}
		}
		
		walkTree(doc, cur->xmlChildrenNode);
		
		cur = cur->next;
	}

	return;
}

int
html2tex(const char *content)
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
	walkTree(parser->myDoc, root);
	
	return 0;
}