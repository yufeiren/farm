#include "kd.h"

static void
usage(const char *msg)
{
	printf(
"usage: kd [ options ]\n"
"options: -i \t\t\tinput ompl file\n"
"         -o \t\t\toutput tex file\n"
);

	if (msg[0] != 0)
		fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
	char omplfile[128];
	char texfile[128];
	
	/* get parameter */
	int c;
	memset(omplfile, '\0', 128);
	memset(texfile, '\0', 128);
	
	while ((c = getopt(argc, argv, "i:o:")) != -1) {
		switch (c) {
		case 'i':
			strcpy(omplfile, optarg);
			break;
		case 'o':
			strcpy(texfile, optarg);
			break;
		default:
			usage("wrong parameter\n");
			break;
		}
	}
	
	if ((omplfile == NULL) || (texfile == NULL))
		usage("need parameter\n");
	
	/* open or create ompl file and texfile */
	texfp = fopen(texfile, "w+");
	if (!texfp) {
		perror("File Open tex: ");
		exit(EXIT_FAILURE);
	}
	
	texinit(texfp);
	
	ompl2tex(const char *omplfile);
	
	texclose(texfp);

	exit(EXIT_SUCCESS);
}
