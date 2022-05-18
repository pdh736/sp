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
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// #include <microhttpd.h>

#include "tct_library.h"

void tct_string_token(GPtrArray *str_list, char *original_str, char *delimeter) {
    int len = strlen(original_str);
    char *copy = (char*)malloc(sizeof(char)*len+1);
    strncpy(copy, original_str, len);
    copy[len] = 0;

    char *ptr;
    int ptr_len;
    ptr = strtok(copy, delimeter);
    while(ptr!=NULL) {
        ptr_len = strlen(ptr);
        char *temp = (char*)g_malloc(sizeof(char)*ptr_len);
        strncpy(temp, ptr, ptr_len);
        temp[ptr_len] = 0;
        tct_ptr_ary_add(str_list, temp);
        ptr = strtok(NULL, delimeter);
    }
    free(copy);
}


//=====time======================================
char* get_cur_date(char delimeter) {
    time_t t = time(NULL);
    struct tm lt;
    localtime_r(&t, &lt);
    
    char year[5] = {0};
    char mon[3] = {0};
    char day[3] = {0};

    sprintf(year, "%04d", lt.tm_year+1900);
    sprintf(mon, "%02d", lt.tm_mon+1);
    sprintf(day, "%02d", lt.tm_mday);

    char *str_time = (char*)malloc(sizeof(char)*TCT_DATE_SIZE);
    memset(str_time, 0, sizeof(TCT_DATE_SIZE));

    strncat(str_time, year, 4);
    if(delimeter != '\0')
        strncat(str_time, &delimeter, 1);
    strncat(str_time, mon, 4);
    if(delimeter != '\0')
        strncat(str_time, &delimeter, 1);
    strncat(str_time, day, 4);
    
    return str_time;
}

char* get_cur_date_time(char delimeter1, char delimeter2, char delimeter3) {
    time_t t = time(NULL);
    struct tm lt;
    localtime_r(&t, &lt);
    
    char year[5] = {0};
    char mon[3] = {0};
    char day[3] = {0};
    char hour[3] = {0};
    char min[3] = {0};
    char sec[3] = {0};

    sprintf(year, "%04d", lt.tm_year+1900);
    sprintf(mon, "%02d", lt.tm_mon+1);
    sprintf(day, "%02d", lt.tm_mday);
    sprintf(hour, "%02d", lt.tm_hour);
    sprintf(min, "%02d", lt.tm_min);
    sprintf(sec, "%02d", lt.tm_sec);

    char *str_time = (char*)malloc(sizeof(char)*TCT_DATE_TIME_SIZE);
    memset(str_time, 0, sizeof(TCT_DATE_TIME_SIZE));

    strncat(str_time, year, 4);
    if(delimeter1 != '\0')
        strncat(str_time, &delimeter1, 1);
    strncat(str_time, mon, 4);
    if(delimeter1 != '\0')
        strncat(str_time, &delimeter1, 1);
    strncat(str_time, day, 4);
    if(delimeter2 != '\0')
        strncat(str_time, &delimeter2, 1);
    strncat(str_time, hour, 2);
    if(delimeter3 != '\0')
        strncat(str_time, &delimeter3, 1);
    strncat(str_time, min, 2);
    if(delimeter3 != '\0')
        strncat(str_time, &delimeter3, 1);
    strncat(str_time, sec, 2);

    return str_time;
}

void convert_date_time(struct tm *time, char * str_time, int is_delimeter) {
	char year[5], month[3], day[3], hour[3], min[3], sec[3];

	strncpy(year, &str_time[0], 4);
    year[4] = 0;
    if(is_delimeter) {
        strncpy(month, &str_time[5], 2);
        month[2] = 0;
        strncpy(day, &str_time[8], 2);
        day[2] = 0;
	    strncpy(hour, &str_time[11], 2);
        hour[2] = 0;
	    strncpy(min, &str_time[14], 2);
        min[2] = 0;
	    strncpy(sec, &str_time[17], 2);
        sec[2] = 0;
    }
    else {
	    strncpy(month, &str_time[4], 2);
        month[2] = 0;
        strncpy(day, &str_time[6], 2);
        day[2] = 0;
	    strncpy(hour, &str_time[8], 2);
        hour[2] = 0;
	    strncpy(min, &str_time[10], 2);
        min[2] = 0;
	    strncpy(sec, &str_time[12], 2);
        sec[2] = 0;
    }
	
	time->tm_year = atoi(year) - 1900;
	time->tm_mon = atoi(month) - 1;
	time->tm_mday = atoi(day);
	time->tm_hour = atoi(hour);
	time->tm_min = atoi(min);
	time->tm_sec = atoi(sec);
}

//is_delimeter ex)2021-01-23 01:23:45 is_delimeter is 1 //20210123012345 is_delimeteris 0
int diff_second(char *str_time1, char *str_time2, int is_delimeter) {
    struct tm tm_time1;
	struct tm tm_time2;
	time_t time1, time2;
	double sec;

    convert_date_time(&tm_time1, str_time1, is_delimeter);
    convert_date_time(&tm_time2, str_time2, is_delimeter);

    time1 = mktime(&tm_time1);
    time2 = mktime(&tm_time2);

    sec = difftime(time2, time1);

    return (int)sec;
}
//===============================================


//=====thread====================================
void tct_thread_init(TCT_THREAD_INFO *t_info, void *(*func)(void*), void *arg) {
    t_info->func = func;
    t_info->arg = arg;

    pthread_mutex_init(&t_info->mutex, NULL);
}

int tct_thread_create(TCT_THREAD_INFO *t_info) {
    int ret = pthread_create(&t_info->tid, NULL, t_info->func, (void*)t_info);
    return ret;
}

void tct_thread_join(TCT_THREAD_INFO *t_info) {
    pthread_join(t_info->tid, NULL);
}
//===============================================


//=====socket====================================
int tct_init_server_sock(int port) {
    int server_sock;
    struct sockaddr_in server_addr;

    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    int reuseflag=1;
    setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,(char*)&reuseflag,sizeof(reuseflag));

    if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        return -1;

    if(listen(server_sock, 5) < 0)
        return -1;

    return server_sock;
}

int tct_accept_server_sock(int server_sock) {
    int client_sock;
    struct sockaddr_in client_addr;
    unsigned int len;

    len = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, (socklen_t*)&len);

    return client_sock;
}

int tct_connect_client_sock(char *server_ip, int server_port) {
    int client_sock;
    struct sockaddr_in client_addr;

    client_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_addr.s_addr = inet_addr(server_ip);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(server_port);

    if(connect(client_sock, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0)
        return -1;

    return client_sock;
}

void tct_close_sock(int sock) {
    close(sock);
}

TCT_SOCK_PKT * tct_alloc_packet(void) {
    TCT_SOCK_PKT *pkt = (TCT_SOCK_PKT*)malloc(sizeof(TCT_SOCK_PKT));
    pkt->hdr.data_size = 0;
    pkt->data = NULL;
    
    return pkt;
}

void tct_free_packet(TCT_SOCK_PKT *pkt) {
    if(pkt->data) {
        free(pkt->data);
        pkt->data = NULL;
    }

    if(pkt) {
        free(pkt);
        pkt = NULL;
    }
}

void tct_make_packet(TCT_SOCK_PKT *pkt, int size, char *data) {
    pkt->hdr.data_size = size;
    pkt->data = (char*)malloc(sizeof(char)*size);
    strncpy(pkt->data, data, size);
}

int tct_sock_read(int sock, TCT_SOCK_PKT *pkt) {
    TCT_SOCK_PKT_HDR *hdr = &pkt->hdr;
    int header_size = sizeof(TCT_SOCK_PKT_HDR);
    
    int ret = 0;
    ret = recv(sock, (char*)hdr, header_size, 0);
    if(ret < 0)
        return -1;

    int size = hdr->data_size;
    pkt->data = (char*)malloc(sizeof(char*)*size);
    char *buf = pkt->data;
    int pos = 0;
    while(pos < size) {
        ret = recv(sock, buf+pos, size - pos, 0);
        if(ret < 0) {
            pos = -1;
            break;
        }
        pos += ret;
    }
    return pos < 0 ? -1 : pos;
}

int tct_sock_write(int sock, TCT_SOCK_PKT *pkt) {
    int size = pkt->hdr.data_size;
    TCT_SOCK_PKT_HDR *hdr = &pkt->hdr;
    int header_size = sizeof(TCT_SOCK_PKT_HDR);
    char *buf = pkt->data;
    int ret = 0, pos = 0;

    ret = send(sock, (char*)hdr, header_size, 0);
    if(ret < 0)
        return -1;

    while(pos < size) {
        ret = send(sock, buf+pos, size - pos, 0);
        if(ret < 0) {
            pos = -1;
            break;
        }
        pos += ret;
    }
    
    return pos < 0 ? -1 : pos;
}
//===============================================


//=====file io====================================
int tct_mkdir(char *path) {
    if(access(path, 0) == -1)
    {
        if(mkdir(path, 0755) < 0)
        {
            printf("mk dir fail\n");
            return -1;
        }
    }
    return 0;
}
//================================================


//=====GArray Wrapper=============================
//=====GArray Example=============================
//compare func example
//a-b 오름차순 b-a 내림차순
int tct_comp_ary_name(gpointer a, gpointer b) {
    return strcmp( ((TCT_FILE_INFO*)a)->name, ((TCT_FILE_INFO*)b)->name);
}
int tct_comp_ary_name_desc(gpointer a, gpointer b) {
    return strcmp( ((TCT_FILE_INFO*)b)->name, ((TCT_FILE_INFO*)a)->name);
}

void tct_print_ary(GArray *array) {
    int i;
    for(i=0;i<array->len;i++) {
        TCT_FILE_INFO file_info = tct_ary_index(array, TCT_FILE_INFO, i);
        printf("file name : %s\n", file_info.name);
    }
}

GArray* tct_get_file_list_ary(char *path) {
    GArray* file_list = tct_ary_new(TCT_FILE_INFO);

    tct_make_file_list_ary(file_list, path);

    return file_list;
}

int tct_make_file_list_ary(GArray* file_list, char *path) {
    struct dirent *de;
    struct stat file_stat;
    
    DIR *dr = opendir(path);
    if(dr == NULL) {
        printf("dir open error\n");
        return -1;
    }

    while( (de = readdir(dr)) != NULL) {
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        TCT_FILE_INFO file_info;
        memset(&file_info, 0, sizeof(file_info));

        strncpy(file_info.name, de->d_name, sizeof(file_info.name)-1);
        sprintf(file_info.path, "%s/%s", path, de->d_name);

        stat(file_info.path, &file_stat);
        file_info.size = file_stat.st_size;
        if(S_ISDIR(file_stat.st_mode)) {//디렉토리
            file_info.type = TCT_FILE_TYPE_DIR;
            tct_make_file_list_ary(file_list, file_info.path);
        }
        else {
            file_info.type = TCT_FILE_TYPE_FILE;
        }
        
        tct_ary_add(file_list, &file_info);
    }
    closedir(dr);

    return 0;
}

GArray* tct_get_file_content_ary(char *path) {
    FILE *fd = fopen(path, "rb");
    if(fd < 0)
        return NULL;

    GArray* file_contents = tct_ary_new(TCT_FILE_CONTENT);

    TCT_FILE_CONTENT fc;
    //구분문자 #
    while(fscanf(fd, "%d#%[^#]#%d", &fc.num, fc.name, &fc.val) > 0 ) {
        tct_ary_add(file_contents, &fc);
    }

    fclose(fd);

    return file_contents;
}
//================================================


//=====GPtrArray Wrapper==========================
void free_ptr_ary_item(gpointer data) {
    if (data)
        free(data);
}
//=====GPtrArray Example==========================
//compare func example
//a-b 오름차순 b-a 내림차순
int tct_comp_ptr_ary_name(gpointer a, gpointer b) {
    return strcmp( (*(TCT_FILE_INFO**)a)->name, (*(TCT_FILE_INFO**)b)->name);
}

int tct_comp_ptr_ary_name_desc(gpointer a, gpointer b) {
    return strcmp( (*(TCT_FILE_INFO**)b)->name, (*(TCT_FILE_INFO**)a)->name);
}

void tct_print_ptr_ary(GPtrArray *array) {
    int i;
    for(i=0;i<array->len;i++) {
        TCT_FILE_INFO *file_info = tct_ptr_ary_index(array, i);
        printf("file name : %s\n", file_info->name);
    }
}

void tct_print_ptr_ary_cb(GPtrArray *array, void (*cb)(GPtrArray *, int)) {
    int i;
    for(i=0;i<array->len;i++) {
        cb(array, i);
    }
}

void print_file_content(GPtrArray *file_contents, int i) {
    TCT_FILE_CONTENT *file_content = tct_ptr_ary_index(file_contents, i);
    printf("%s\n", file_content->name);
}

GPtrArray* tct_get_file_list_ptr_ary(char *path) {
    GPtrArray* file_list = tct_ptr_ary_new();

    tct_make_file_list_ptr_ary(file_list, path);

    return file_list;

}
int tct_make_file_list_ptr_ary(GPtrArray* file_list, char *path) {
    struct dirent *de;
    struct stat file_stat;
    
    DIR *dr = opendir(path);
    if(dr == NULL) {
        printf("dir open error\n");
        return -1;
    }

    while( (de = readdir(dr)) != NULL) {
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        TCT_FILE_INFO *file_info = (TCT_FILE_INFO*)g_malloc(sizeof(TCT_FILE_INFO));
        memset(file_info, 0, sizeof(TCT_FILE_INFO));

        strncpy(file_info->name, de->d_name, sizeof(file_info->name)-1);
        sprintf(file_info->path, "%s/%s", path, de->d_name);

        stat(file_info->path, &file_stat);
        file_info->size = file_stat.st_size;

        if(S_ISDIR(file_stat.st_mode)) {//디렉토리
            file_info->type = TCT_FILE_TYPE_DIR;
            tct_make_file_list_ptr_ary(file_list, file_info->path);
        }
        else {
            file_info->type = TCT_FILE_TYPE_FILE;
        }
        
        tct_ptr_ary_add(file_list, file_info);
    }
    closedir(dr);

    return 0;
}

GPtrArray* tct_get_file_content_ptr_ary(char *path) {
    FILE *fd = fopen(path, "rb");
    if(fd < 0)
        return NULL;

    GPtrArray* file_contents = tct_ptr_ary_new();

    //구분문자 #
    while(1) {
        TCT_FILE_CONTENT *fc = (TCT_FILE_CONTENT*)g_malloc(sizeof(TCT_FILE_CONTENT));
        memset(fc, 0, sizeof(TCT_FILE_CONTENT));

        if(fscanf(fd, "%d#%[^#]#%d", &fc->num, fc->name, &fc->val) <= 0) {
            g_free(fc);
            break;
        }
        
        tct_ptr_ary_add(file_contents, fc);
    }

    fclose(fd);

    return file_contents;
}
//================================================


//=====GQueue wrapper=============================
void tct_queue_item_free(gpointer data) {
    if (data)
        free(data);
}

GQueue* tct_queue_new(void) {
    GQueue* q = g_queue_new();
    //g_queue_init(q) //when must use static allocation
    return q;
}

void tct_queue_free(GQueue* q, int is_item_dynamic) {
    if (is_item_dynamic) {
        g_queue_free_full(q, tct_queue_item_free);
    }
    else {
        g_queue_free(q);
    }
}

void tct_queue_clear(GQueue* q, int is_item_dynamic) {
    if (!is_item_dynamic) {
        g_queue_clear(q);
    }
    else {
        while(q->length) {
            gpointer data = g_queue_pop_head(q);
            free(data);
        }
    }
}

void tct_queue_push(GQueue* q, gpointer data) {
    g_queue_push_tail(q, data);
}

void tct_queue_push_head(GQueue* q, gpointer data) {
    g_queue_push_head(q, data);
}

gpointer tct_queue_pop(GQueue* q) {
    return g_queue_pop_head(q);
}

gpointer tct_queue_pop_tail(GQueue* q) {
    return g_queue_pop_tail(q);
}

gpointer tct_queue_peek(GQueue* q) {
    return g_queue_peek_head(q);
}

gpointer tct_queue_peek_tail(GQueue* q) {
    return g_queue_peek_tail(q);
}

void tct_queue_sort(GQueue* q, GCompareDataFunc func) {
    return g_queue_sort(q, func, NULL);
}
//================================================


//=====json=======================================
char* tct_file_to_json_example(char *file_path) {
    struct stat file_info;
    FILE *fd;
        
    fd = fopen(file_path, "rb");
    if(!fd) {
        printf("file open error\n");
        return NULL;
    }

    stat(file_path, &file_info);
    //printf("file path : %s / file size : %d\n", file_path, file_info.st_size);
    int buf_size = file_info.st_size;
    char *buf = (char*)malloc(sizeof(char)*buf_size);
    
    fread(buf, 1, buf_size, fd);
    
    fclose(fd);

    json_object *myobj = json_object_new_object();
    json_object_object_add(myobj, "FilePath", json_object_new_string(file_path));
    json_object_object_add(myobj, "FileContent", json_object_new_string(buf));

    free(buf);

    const char *result_json = json_object_to_json_string_ext(myobj, JSON_C_TO_STRING_PLAIN);

    int len = strlen(result_json);
    char *result_str = (char*)malloc(sizeof(char)*(len+1));
    strncpy(result_str, result_json, len);
    result_str[len] = 0;

    json_object_put(myobj);

    return result_str;
}

char* tct_make_json_example()
{
    json_object *myobj = json_object_new_object();
    json_object_object_add(myobj, "name", json_object_new_string("spiderman"));
    json_object_object_add(myobj, "age", json_object_new_int(45));
    json_object_object_add(myobj, "married", json_object_new_boolean(1));

    json_object *specialty = json_object_new_array();
    json_object_array_add(specialty, json_object_new_string("martial art"));
    json_object_array_add(specialty, json_object_new_string("gun"));
    json_object_object_add(myobj, "specialty", specialty);

    json_object *vaccine = json_object_new_object();
    json_object_object_add(vaccine, "1st", json_object_new_string("done"));
    json_object_object_add(vaccine, "2nd", json_object_new_string("expected"));
    json_object_object_add(vaccine, "3rd", json_object_new_null());
    json_object_object_add(myobj, "vaccine", vaccine);

    json_object *children = json_object_new_array();
    json_object *child1 = json_object_new_object();
    json_object_object_add(child1, "name", json_object_new_string("spiderboy"));
    json_object_object_add(child1, "age", json_object_new_int(10));
    json_object *child2 = json_object_new_object();
    json_object_object_add(child2, "name", json_object_new_string("spidergirl"));
    json_object_object_add(child2, "age", json_object_new_int(8));
    json_object_array_add(children, child1);
    json_object_array_add(children, child2);
    json_object_object_add(myobj, "children", children);

    json_object_object_add(myobj, "address", json_object_new_null());

    const char *result_json = json_object_to_json_string_ext(myobj, JSON_C_TO_STRING_PLAIN);
    //printf("json: %s\n", result_json);
    //json_object_to_file("sample.json", myobj);

    int len = strlen(result_json);
    char *result_str = (char*)malloc(sizeof(char)*(len+1));
    strncpy(result_str, result_json, len);
    result_str[len] = 0;

    json_object_put(myobj);

    return result_str;
}

void tct_parse_json_example(char* json_str) {
    json_object *root, *name, *phone;
    root = json_tokener_parse(json_str);
    json_object_object_get_ex(root, "name", &name);
    json_object_object_get_ex(root, "phone", &phone);
    printf("name: %s, phone: %s\n", json_object_get_string(name), json_object_get_string(phone));
}
//================================================


//=====http client(curl)==========================
void tct_http_data_init(TCT_HTTP_DATA* data) {
    data->headers = tct_ptr_ary_new2();
    data->body = NULL;
    data->body_size = 0;
}
void tct_http_data_clear(TCT_HTTP_DATA* data) {
    tct_ptr_ary_free(data->headers);
    data->headers = NULL;

    if(data->body)
        free(data->body);
    data->body= NULL;
    data->body_size = 0;
}

size_t tct_curl_cb(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;

    TCT_HTTP_DATA* http_data = (TCT_HTTP_DATA*)userp;

    char* ptr = realloc(http_data->body, http_data->body_size + realsize + 1);
    if (ptr == NULL)
        return 0; /* out of memory! */

    http_data->body = ptr;
    memcpy(&(http_data->body[http_data->body_size]), data, realsize);
    http_data->body_size += realsize;
    http_data->body[http_data->body_size] = 0;

    return realsize;
}

size_t tct_curl_header_cb(void *data, size_t size, size_t nitems, void *userp) {
    /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    int len = nitems * size;


    //if linux \n\n 2, if window \r\n\r\n 4 header 경계
    if (len == 2) {
        return len;
        //0 리턴시 curl_easy_getinfo 에서 에러로 판단

    }

    GPtrArray* ptr_ary = (GPtrArray*)userp;

    char* header = (char*)malloc(sizeof(char)*len+1);
    memset(header, 0x00, len+1);

    strncpy(header, (char*)data, len);

    tct_ptr_ary_add(ptr_ary, header);


    return len;
}

struct curl_slist* tct_curl_set_header(CURL* curl, GPtrArray* ptr_ary) {

    struct curl_slist* header = NULL ;
    for (int i = 0; i < tct_ptr_ary_len(ptr_ary); i++) {
        header = curl_slist_append( header, tct_ptr_ary_index(ptr_ary, i));
    }
    //header = curl_slist_append( header , "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.0.3705) " ) ;
    //header = curl_slist_append( header , "Content-Type: application/x-www-form-urlencoded" ) ;
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER , header) ;
    return header;
}

int tct_curl_http_get(char* url, TCT_HTTP_DATA* req, TCT_HTTP_DATA* res) {
    CURL *curl;
    CURLcode result;

    int status_code = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, tct_curl_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)res);

        struct curl_slist* header = NULL ;
        if(req) {
            if(tct_ptr_ary_len(req->headers)) {
                tct_curl_set_header(curl, req->headers);
            }
        }
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, tct_curl_header_cb);
        /* pass in custom data to the callback */
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, res->headers);

        result = curl_easy_perform(curl);
        if (CURLE_OK == result) {
            //printf("response is OK: %d\n", result);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
            //printf("status code : %d\n", status_code);

        } else {
            printf("response is not OK: %d\n", result);
        }

        curl_slist_free_all( header ) ;

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return status_code;
}

int tct_curl_http_post(char* url, TCT_HTTP_DATA* req, TCT_HTTP_DATA* res) {
    CURL *curl;
    CURLcode result;

    int status_code = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);
        curl_easy_setopt(curl, CURLOPT_POST, 1L); // POST request

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, tct_curl_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)res);

        struct curl_slist* header = NULL;
        if (req) {
            if( req->body ) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req->body); // POST request payload
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(req->body)); // POST request payload size
            }

            if ( tct_ptr_ary_len(req->headers) ) {
                tct_curl_set_header(curl, req->headers);
            }
        }

        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, tct_curl_header_cb);
        /* pass in custom data to the callback */
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, res->headers);

        result = curl_easy_perform(curl);
        if (CURLE_OK == result) {
            //printf("response is OK : %d\n", result);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
            //printf("status : %d\n", status_code);
        } else {
            printf("response is not OK: %d\n", result);
        }

        curl_slist_free_all( header ) ;
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return status_code;
}

void tct_curl_example(void) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    int status = 0;

    char* url = "http://127.0.0.1:8080/helloworld";
    //get
    printf("request get\n");
    TCT_HTTP_DATA req;
    TCT_HTTP_DATA res;
    tct_http_data_init(&req);
    tct_http_data_init(&res);

    char* user = (char*)malloc(128);
    memset(user, 0x00, 128);
    strcpy(user, "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.0.3705)");
    tct_ptr_ary_add(req.headers, user);

    char* con_type = (char*)malloc(64);
    memset(con_type, 0x00, 64);
    strcpy(con_type, "Content-Type: application/x-www-form-urlencoded");
    tct_ptr_ary_add(req.headers, con_type);

    status = tct_curl_http_get(url, &req, &res);

    printf("header size : %d\n", tct_ptr_ary_len(res.headers));
    printf("[body] : %s\n", res.body);

    tct_http_data_clear(&res);


    //post
    printf("request post\n");

    tct_http_data_init(&res);
    req.body = (char*)malloc(64);
    memset(req.body, 0x00, 64);
    char* body_content = "hello world!";
    strcpy(req.body, body_content);
    req.body_size = 64;

    status = tct_curl_http_post(url, &req, &res);

    printf("header size : %d\n", tct_ptr_ary_len(res.headers));
    printf("[body] : %s\n", res.body);

    tct_http_data_clear(&req);
    tct_http_data_clear(&res);

}
//================================================
