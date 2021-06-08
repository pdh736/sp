#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#include <glib.h>
#include <microhttpd.h>
#include <json-c/json.h>
#include <curl/curl.h>

#define PORT 8080

struct memory {
    char *response;
    size_t size;
};

size_t cb(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *) userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL)
        return 0; /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

void request_get_helloworld(void) {
    CURL *curl;
    CURLcode res;
    struct memory data;
    char url[100] = { 0, };

    memset(&data, 0, sizeof(data));

    curl = curl_easy_init();
    if (curl) {
        sprintf(url, "http://127.0.0.1:8080/helloworld");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);

        res = curl_easy_perform(curl);
        if (CURLE_OK == res) {
            long status_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

            printf("[status line] Status Code: %ld\n", status_code);
            printf("[body] %s\n", data.response);
	    //parse data here

        } else {
            printf("response is not OK: %d\n", res);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

void request_post_helloworld(void) {
    CURL *curl;
    CURLcode res;
    struct memory data;
    char url[100] = { 0, };
    char post[100] = { 0, };

    memset(&data, 0, sizeof(data));

    curl = curl_easy_init();
    if (curl) {
        sprintf(url, "http://127.0.0.1:8080/helloworld");
        sprintf(post, "Hello World!");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);
        curl_easy_setopt(curl, CURLOPT_POST, 1L); // POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post); // POST request payload
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(post)); // POST request payload size

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);

        res = curl_easy_perform(curl);
        if (CURLE_OK == res) {
            long status_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

            printf("[status line] Status Code: %ld\n", status_code);
            printf("[body] %s\n", data.response);
	    //parse data here
        } else {
            printf("response is not OK: %d\n", res);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

int main(int argc, char *argv[]){

    curl_global_init(CURL_GLOBAL_DEFAULT);

    request_get_helloworld();
    request_post_helloworld();

    return EXIT_SUCCESS;
}
