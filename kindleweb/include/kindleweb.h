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
