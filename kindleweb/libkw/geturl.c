#include "kindleweb.h"

static size_t
write_data(void *ptr, size_t size, size_t nmeb, void *stream)
{
	return fwrite(ptr, size, nmeb, stream);
}

int
urltofile(const char *url, const char *fname)
{
	FILE *fp;
	CURL *handle;
	CURLcode ret;
	
	fp = fopen(fname, "w+");
	if(!fp){
		perror("File Open Failure ");
		return 1;
	}
	
	handle = curl_easy_init();
	
	/*Using the http protocol*/
	curl_easy_setopt(handle, CURLOPT_URL, url);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
	
	ret = curl_easy_perform(handle);
	if (ret != 0) {
		fprintf(stderr, "curl_easy_perform fail: %d\n", ret);
		fclose(fp);
		return 1;
	}
	
	curl_easy_cleanup(handle);
	
	fclose(fp);
	
	return 0;
}
