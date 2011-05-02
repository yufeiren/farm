#include "kd.h"

extern FILE *texfp;

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
	fprintf(texfp, "\\LARGE{%s}\r\n\r\n", str);

	return;
}

void
img2tex(const char *loc)
{
	fprintf(texfp, "\\begin{center}\n");
	fprintf(texfp, "\\includegraphics[scale=0.3]{%s}\n", loc);
	fprintf(texfp, "\\\end{center}\n");
	
	return;
}