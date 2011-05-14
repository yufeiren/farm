#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* libxml2 */
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <libxml/HTMLparser.h>

/* curl */
#include <curl/curl.h>

/* mysql */
#include <my_global.h>
#include <mysql.h>

enum tex_font_size {
	TEX_FONT_tiny = 0,
	TEX_FONT_scriptsize,
	TEX_FONT_footnotesize,
	TEX_FONT_small,
	TEX_FONT_normalsize,
	TEX_FONT_large,
	TEX_FONT_Large,
	TEX_FONT_huge,
	TEX_FONT_Huge
	
};

FILE *texfp;

void texinit(FILE *fp);

void data2tex(const char *str);
void img2tex(const char *loc);

char *removetags(xmlChar *content);

void texclose(FILE *fp);

int ompl2tex(const char *omplfile);

int urltofile(const char *url, const char *file);

int rssparser(const char *rssfile, int linkid);

int html2text(char *text, const char *html);
