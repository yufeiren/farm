#include "kindleweb.h"

extern FILE *texfp;

/* refer http://www.maths.tcd.ie/~dwilkins/LaTeXPrimer/TextSymbols.html */
static void
checkTexSymbol(char *outstr, const char *instr)
{
	int c;
	int curin, curout;
	
	curin = curout = 0;
	
	while ((c = *(instr + curin ++)) != '\0') {
		switch (c) {
		case '#':
		case '$':
		case '%':
		case '&':
		case '_':
		case '{':
		case '}':
			*(outstr + curout ++) = '\\';
			*(outstr + curout ++) = c;
			break;
		case '\\':
		case '^':
		case '~':	
			break;
		default:
			*(outstr + curout ++) = c;
		}
	}
	
	*(outstr + curout) = '\0';
	return;
}

void
texinit(FILE *fp)
{
	fprintf(fp, "\\documentclass[a4paper,12pt]{article}\n");
	fprintf(fp, "\\usepackage{graphicx}\n");
	fprintf(fp, "\\usepackage {geometry}\n");
	fprintf(fp, "\\usepackage {fancyhdr}\n");
	fprintf(fp, "\\usepackage {amsmath ,amsthm , amssymb}\n");
	fprintf(fp, "\\usepackage {graphicx}\n");
	fprintf(fp, "\\usepackage {hyperref}\n");
	fprintf(fp, "\\usepackage {CJK}\n");
	
	fprintf(fp, "\\begin {document}\n");
	fprintf(fp, "\\begin {CJK}{UTF8}{gkai}\n");
/*	fprintf(fp, "\\maketitle\n"); */
	
	return;
}

void
texclose(FILE *fp)
{
	fprintf(fp, "\n");
	fprintf(fp, "\\end {CJK}\n");
	fprintf(fp, "\\end {document}\n");
	return;
}


void
data2tex(const char *str)
{
	char *outstr;
	outstr = (char *) malloc(strlen(str) * 2 + 1);
	if (outstr == NULL)
		return;
	
	checkTexSymbol(outstr, str);
	
	fprintf(texfp, "\\LARGE{%s}\r\n\r\n", outstr);
	
	free(outstr);
	
	return;
}

void
img2tex(const char *loc)
{
	fprintf(texfp, "\\begin{center}\n");
	fprintf(texfp, "\\includegraphics[scale=0.5]{%s}\n", loc);
	fprintf(texfp, "\\end{center}\n");
	
	return;
}
