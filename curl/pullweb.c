/* fetch a xml file */

/* pullweb -u [url] -o [output_file] */

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

static void
usage(const char *msg)
{
	printf(
"usage: pullweb [ options ]\n"
"options: -u \t\t\turl\n"
"         -o \t\t\toutput_file\n"
);

	if (msg[0] != 0)
		fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

size_t
write_data(void *ptr, size_t size, size_t nmeb, void *stream)
{
	return fwrite(ptr, size, nmeb, stream);
}

int
main(int argc, char **argv)
{
	char url[1024];
	char out[1024];
	
	memset(out, '\0', 1024);
	strcpy(out, "./sample");

	/* get parameter */
	int c;
	
	while ((c = getopt(argc, argv, "u:o:")) != -1) {
		switch (c) {
		case 'u':
			memset(url, '\0', 1024);
			strcpy(url, optarg);
			break;
		case 'o':
			memset(out, '\0', 1024);
			strcpy(out, optarg);
			break;
		default:
			usage("wrong parameter\n");
			break;
		}
	}

	FILE *fp = fopen(out, "w+");
	if(!fp){
		perror("File Open:");
		exit(EXIT_FAILURE);
	}
	
	CURL *handle = curl_easy_init();
	/*Using the http protocol*/
	curl_easy_setopt(handle, CURLOPT_URL, url);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
	
	curl_easy_perform(handle);
	curl_easy_cleanup(handle);
	
	exit(0);
}
