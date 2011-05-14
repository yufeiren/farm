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
data2tex(const char *str, enum tex_font_size fontsize)
{
	char *outstr;
	outstr = (char *) malloc(strlen(str) * 2 + 1);
	if (outstr == NULL)
		return;
	
	checkTexSymbol(outstr, str);
	
	char *fs;
	
	switch(fontsize) {
	case TEX_FONT_tiny:
		fs = "tiny";
		break;
	case TEX_FONT_scriptsize:
		fs = "scriptsize";
		break;
	case TEX_FONT_footnotesize:
		fs = "footnotesize";
		break;
	case TEX_FONT_small:
		fs = "small";
		break;
	case TEX_FONT_normalsize:
		fs = "normalsize";
		break;
	case TEX_FONT_large:
		fs = "large";
		break;
	case TEX_FONT_Large:
		fs = "Large";
		break;
	case TEX_FONT_huge:
		fs = "huge";
		break;
	case TEX_FONT_Huge:
		fs = "Huge";
		break;
	default:
		fs = "normalsize";
		break;
	}
	
	fprintf(texfp, "\\%s{%s}\r\n\r\n", fs, outstr);
	
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
