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
void tct_thread_init(TCT_THREAD_INFO *t_info, void* (*func)(void*), void* arg) {
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
int tct_sock_init_server(int port) {
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

int tct_sock_accept(int server_sock) {
    int client_sock;
    struct sockaddr_in client_addr;
    unsigned int len;

    len = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, (socklen_t*)&len);

    return client_sock;
}

int tct_sock_connect(char* server_ip, int server_port) {
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

void tct_sock_close(int sock) {
    if (sock)
        close(sock);
}

TCT_SOCK_PKT * tct_sock_alloc_packet(void) {
    TCT_SOCK_PKT *pkt = (TCT_SOCK_PKT*)malloc(sizeof(TCT_SOCK_PKT));
    pkt->hdr.data_size = 0;
    pkt->data = NULL;
    
    return pkt;
}

void tct_sock_free_packet(TCT_SOCK_PKT* pkt) {
    if(pkt->data) {
        free(pkt->data);
        pkt->data = NULL;
    }

    if(pkt) {
        free(pkt);
        pkt = NULL;
    }
}

void tct_sock_make_packet(TCT_SOCK_PKT *pkt, int size, char *data) {
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
int tct_ary_comp_name(gpointer a, gpointer b) {
    return strcmp( ((TCT_FILE_INFO*)a)->name, ((TCT_FILE_INFO*)b)->name);
}
int tct_ary_comp_name_desc(gpointer a, gpointer b) {
    return strcmp( ((TCT_FILE_INFO*)b)->name, ((TCT_FILE_INFO*)a)->name);
}

void tct_ary_print(GArray* array) {
    int i;
    for(i=0;i<array->len;i++) {
        TCT_FILE_INFO file_info = tct_ary_index(array, TCT_FILE_INFO, i);
        printf("file path : %s\n", file_info.path);
    }
}

GArray* tct_ary_get_file_list(char* path) {
    GArray* file_list = tct_ary_new(TCT_FILE_INFO);

    tct_ary_make_file_list(file_list, path);

    return file_list;
}

int tct_ary_make_file_list(GArray* file_list, char* path) {
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
        if (path[strlen(path)-1] == '/')
            sprintf(file_info.path, "%s%s", path, de->d_name);
        else
            sprintf(file_info.path, "%s/%s", path, de->d_name);

        stat(file_info.path, &file_stat);
        file_info.size = file_stat.st_size;
        if(S_ISDIR(file_stat.st_mode)) {//디렉토리
            file_info.type = TCT_FILE_TYPE_DIR;
            tct_ary_make_file_list(file_list, file_info.path);
        }
        else {
            file_info.type = TCT_FILE_TYPE_FILE;
        }
        
        tct_ary_add(file_list, &file_info);
    }
    closedir(dr);

    return 0;
}

GArray* tct_ary_get_file_content(char* path) {
    FILE* fd = fopen(path, "rb");
    if(fd < 0)
        return NULL;

    GArray* file_contents = tct_ary_new(TCT_FILE_CONTENT);

    TCT_FILE_CONTENT fc;
    //win
    //while(fscanf(fd, "%d#%[^#]#%d%*c%*c", &fc.num, fc.name, &fc.val) > 0 ) {
    while(fscanf(fd, "%d#%[^#]#%d%*c", &fc.num, fc.name, &fc.val) > 0 ) {
        tct_ary_add(file_contents, &fc);
    }

    fclose(fd);

    return file_contents;
}

GArray* tct_ary_get_file_content2(char* path, int (*cb)(GArray*, FILE*)) {
    FILE* fd = fopen(path, "rb");
    if(fd < 0)
        return NULL;

    GArray* file_contents = tct_ary_new(TCT_FILE_CONTENT);

    while(1) {
        if (cb(file_contents, fd) <= 0)
            break;
    }

    fclose(fd);

    return file_contents;
}

int tct_ary_get_file_content_cb(GArray* file_contents, FILE* fd) {

    TCT_FILE_CONTENT fc;
    memset(&fc, 0x00, sizeof(fc));
    //win
    //int ret = fscanf(fd, "%d#%[^#]#%d%*c%*c", &fc.num, fc.name, &fc.val);
    int ret = fscanf(fd, "%d#%[^#]#%d%*c", &fc.num, fc.name, &fc.val);
    if (ret > 0)
        tct_ary_add(file_contents, &fc);

    return ret;
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
int tct_ptr_ary_comp_name(gpointer a, gpointer b) {
    return strcmp( (*(TCT_FILE_INFO**)a)->name, (*(TCT_FILE_INFO**)b)->name);
}

int tct_ptr_ary_comp_name_desc(gpointer a, gpointer b) {
    return strcmp( (*(TCT_FILE_INFO**)b)->name, (*(TCT_FILE_INFO**)a)->name);
}

void tct_ptr_ary_print(GPtrArray *array) {
    int i;
    for(i=0;i<array->len;i++) {
        TCT_FILE_INFO *file_info = tct_ptr_ary_index(array, i);
        printf("file name : %s\n", file_info->name);
    }
}

void tct_ptr_ary_print2(GPtrArray *array, void (*cb)(void*)) {
    int i;
    for(i=0;i<array->len;i++) {
        cb(tct_ptr_ary_index(array, i));
    }
}

void tct_ptr_ary_print_cb(void* item) {
    TCT_FILE_CONTENT* fc = (TCT_FILE_CONTENT*)item;
    printf("%d#%s%d\n", fc->num, fc->name, fc->val);
    
}

GPtrArray* tct_ptr_ary_get_file_list(char *path) {
    GPtrArray* file_list = tct_ptr_ary_new2();

    tct_ptr_ary_make_file_list(file_list, path);

    return file_list;

}
int tct_ptr_ary_make_file_list(GPtrArray* file_list, char *path) {
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
            tct_ptr_ary_make_file_list(file_list, file_info->path);
        }
        else {
            file_info->type = TCT_FILE_TYPE_FILE;
        }
        
        tct_ptr_ary_add(file_list, file_info);
    }
    closedir(dr);

    return 0;
}

GPtrArray* tct_ptr_ary_get_file_content(char *path) {
    FILE *fd = fopen(path, "rb");
    if(fd <= 0) {
        printf("file open error\n");
        return NULL;
    }

    GPtrArray* file_contents = tct_ptr_ary_new();
    tct_ptr_ary_set_free_func(file_contents, tct_file_content_free);

    while(1) {
        TCT_FILE_CONTENT *fc = (TCT_FILE_CONTENT*)g_malloc(sizeof(TCT_FILE_CONTENT));
        memset(fc, 0, sizeof(TCT_FILE_CONTENT));

        //win
        //if(fscanf(fd, "%d#%[^#]#%d%*c%*c", &fc->num, fc->name, &fc->val) <= 0) {
        if(fscanf(fd, "%d#%[^#]#%d%*c", &fc->num, fc->name, &fc->val) <= 0) {
            g_free(fc);
            break;
        }
        
        tct_ptr_ary_add(file_contents, fc);
    }

    fclose(fd);

    return file_contents;
}

GPtrArray* tct_ptr_ary_get_file_content2(char *path, int (*cb)(GPtrArray*, FILE*) ) {
    FILE *fd = fopen(path, "rb");
    if(fd <= 0) {
        printf("file open error\n");
        return NULL;
    }

    GPtrArray* file_contents = tct_ptr_ary_new();
    tct_ptr_ary_set_free_func(file_contents, tct_file_content_free);

    while(1) {
        if (cb(file_contents, fd) <= 0)
            break;
    }

    fclose(fd);

    return file_contents;
}

int tct_ptr_ary_get_file_content_cb(GPtrArray* file_content, FILE* fd) {
    TCT_FILE_CONTENT *fc = (TCT_FILE_CONTENT*)g_malloc(sizeof(TCT_FILE_CONTENT));
    memset(fc, 0, sizeof(TCT_FILE_CONTENT));

    //win
    //int ret = fscanf(fd, "%d#%[^#]#%d%*c%*c", &fc->num, fc->name, &fc->val);
    int ret = fscanf(fd, "%d#%[^#]#%d%*c", &fc->num, fc->name, &fc->val);
    if (ret <= 0)
        g_free(fc);
    else
        tct_ptr_ary_add(file_content, fc);

    return ret;
}

void tct_file_content_free(void* item) {
    if (item) {
        TCT_FILE_CONTENT* fc = (TCT_FILE_CONTENT*)item;
        free(fc);
    }
}
//================================================


//=====GQueue wrapper=============================
void tct_queue_item_free(gpointer data) {
    if (data)
        free(data);
}

GQueue* tct_queue_new(void) {
    GQueue* q = g_queue_new();
    //g_queue_init(q); //when must use static allocation
    return q;
}

void tct_queue_free(GQueue* q, void (*free_func)(void*)) {
    if (free_func) {
        g_queue_free_full(q, free_func);
    }
    else {
        g_queue_free(q);
    }
}

void tct_queue_clear(GQueue* q, void (*free_func)(void*)) {
    if (!free_func) {
        g_queue_clear(q);
    }
    else {
        while(q->length) {
            gpointer data = g_queue_pop_head(q);
            free_func(data);
        }
    }
}

void tct_queue_sort(GQueue* q, GCompareDataFunc func) {
    return g_queue_sort(q, func, NULL);
}
//================================================


//=====json=======================================
void tct_file_to_json_example(char *file_path) {
    json_object* obj = json_object_from_file(file_path);

    json_object* name = NULL;
    json_object* number = NULL;

    json_object_object_get_ex(obj, "name", &name);
    json_object_object_get_ex(obj, "number", &number);

    printf("name : %s, number : %d\n", json_object_get_string(name), json_object_get_int(number));

    json_object_put(obj);
}

void tct_json_to_file_example(char *file_path) {
    json_object* obj = json_object_new_object();
	
	json_object_object_add(obj, "name", json_object_new_string("park"));
	json_object_object_add(obj, "age", json_object_new_int(40));
	json_object_object_add(obj, "married", json_object_new_boolean(0)); 

	json_object* arr = json_object_new_array();
	json_object_array_add(arr, json_object_new_string("c"));
	json_object_array_add(arr, json_object_new_string("python"));
	json_object_object_add(obj, "skill", arr);

	json_object* addr = json_object_new_object();
	json_object_object_add(addr, "contry", json_object_new_string("korea"));
	json_object_object_add(addr, "city", json_object_new_string("seoul"));
	json_object_object_add(obj, "address", addr);

	json_object_object_add(obj, "child", json_type_null);

	json_object* item_arr = json_object_new_array();
	
	json_object* item1 = json_object_new_object();
	json_object_object_add(item1, "name", json_object_new_string("keybord"));
	json_object_object_add(item1, "price", json_object_new_int(10000));
	json_object_array_add(item_arr, item1);
	
	json_object* item2 = json_object_new_object();
	json_object_object_add(item2, "name", json_object_new_string("mouse"));
	json_object_object_add(item2, "price", json_object_new_int(20000));
	json_object_array_add(item_arr, item2);

	json_object_object_add(obj, "item", item_arr);

    json_object_to_file(file_path, obj);

    //const char *result_json = json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PLAIN);
	//printf("%s\n", result_json);

    json_object_put(obj);
}

void tct_json_iter_example(char* file_path) {
	json_object *myObj = json_object_from_file(file_path);

	struct json_object_iterator it;
	struct json_object_iterator itEnd;

	it = json_object_iter_begin(myObj);
	itEnd = json_object_iter_end(myObj);

	while (!json_object_iter_equal(&it, &itEnd)) {
		const char * key = json_object_iter_peek_name(&it);
		json_object *value = json_object_iter_peek_value(&it);
		json_type jt = json_object_get_type(value);
		const char * tn = json_type_to_name(jt);
		printf("Key : %s / Value Type : %s\n", key, tn);
		json_object_iter_next(&it);
	}
}
//================================================


//=====http common================================
TCT_HTTP_HEADER* tct_http_header_new(void) {
    TCT_HTTP_HEADER* header = (TCT_HTTP_HEADER*)malloc(sizeof(TCT_HTTP_HEADER));
    return header;
}
void tct_http_header_init(TCT_HTTP_HEADER* header) {
    if (header) {
        header->name = NULL;
        header->value = NULL;
    }
}
void tct_http_header_free(void* param) {
    TCT_HTTP_HEADER* header = (TCT_HTTP_HEADER*)param;
    if (header) {
        if (header->name)
            free(header->name);
        header->name = NULL;

        if (header->value)
            free(header->value);
        header->value = NULL;
    }
}

int tct_http_header_set_name(TCT_HTTP_HEADER* header, const char* name) {
    if (!header || !name)
        return -1;

    int name_len = strlen(name);
    
    header->name = (char*)malloc(name_len+1);
    memset(header->name, 0x00, name_len+1);
    strncpy(header->name, name, name_len);

    return name_len;
}
int tct_http_header_set_value(TCT_HTTP_HEADER* header, const char* value) {
    if (!header || !value)
        return -1;

    int val_len = strlen(value);
    
    header->value = (char*)malloc(val_len+1);
    memset(header->value, 0x00, val_len+1);
    strncpy(header->value, value, val_len);

    return val_len;
}


TCT_HTTP_DATA* tct_http_data_new(void) {
    TCT_HTTP_DATA* data = (TCT_HTTP_DATA*)malloc(sizeof(TCT_HTTP_DATA));
    return data;
}

void tct_http_data_init(TCT_HTTP_DATA* data) {
    if (data) {
        data->headers = tct_ptr_ary_new();
        tct_ptr_ary_set_free_func(data->headers, tct_http_header_free);
        data->body = NULL;
        data->body_size = 0;
    }
}
void tct_http_data_clear(TCT_HTTP_DATA* data) {
    if (data) {
        tct_ptr_ary_free(data->headers);
        data->headers = NULL;

        if(data->body)
            free(data->body);
        data->body= NULL;
        data->body_size = 0;
    }
}
void tct_http_data_free(TCT_HTTP_DATA* data) {
    if (data) {
        tct_http_data_clear(data);
        free(data);
    }
}
//================================================


//=====http client(curl)==========================
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
    TCT_HTTP_HEADER* header = tct_http_header_new();
    tct_http_header_init(header);
    
    char name[TCT_HEADER_NAME_LEN] = {0,};
    char value[TCT_HEADER_VAL_LEN] = {0,};

    sscanf(data, "%[^: ]:%[^\r/n]", name, value);
    
    tct_http_header_set_name(header, name);
    tct_http_header_set_value(header, value);

    tct_ptr_ary_add(ptr_ary, header);


    return len;
}

struct curl_slist* tct_curl_set_header(CURL* curl, GPtrArray* headers) {

    struct curl_slist* header = NULL ;
    char str_header[TCT_HEADER_STR_LEN] = {0,};
    for (int i = 0; i < tct_ptr_ary_len(headers); i++) {

        memset(str_header, 0x00, sizeof(str_header));

        TCT_HTTP_HEADER* tmp_header = tct_ptr_ary_index(headers, i);
        if (strlen(tmp_header->name) < 0) {
            printf("header name len < 0 \n");
            continue;
        }
        if (strlen(tmp_header->value) < 0) {
            printf("header value len < 0 \n");
            continue;
        }

        strncat(str_header, tmp_header->name, strlen(tmp_header->name));
        strncat(str_header, ": ", 1);
        strncat(str_header, tmp_header->value, strlen(tmp_header->value));

        header = curl_slist_append( header, str_header);
    }
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

    TCT_HTTP_HEADER* user = tct_http_header_new();
    tct_http_header_init(user);
    tct_http_header_set_name(user, "User-Agent");
    tct_http_header_set_value(user, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.0.3705)");
    tct_ptr_ary_add(req.headers, user);

    TCT_HTTP_HEADER* con_type = tct_http_header_new();
    tct_http_header_init(con_type);
    tct_http_header_set_name(con_type, "Content-Type");
    tct_http_header_set_value(con_type, "application/x-www-form-urlencoded");
    tct_ptr_ary_add(req.headers, con_type);

    status = tct_curl_http_get(url, &req, &res);
    printf("http status : %d\n", status);

    printf("header size : %d\n", tct_ptr_ary_len(res.headers));
    for(int i = 0; i < tct_ptr_ary_len(res.headers); i++) {
        TCT_HTTP_HEADER* res_header = tct_ptr_ary_index(res.headers, i);
        printf("[header] : %s:%s\n", res_header->name, res_header->value);
    }
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

    printf("http status : %d\n", status);
    printf("header size : %d\n", tct_ptr_ary_len(res.headers));
    printf("[body] : %s\n", res.body);

    tct_http_data_clear(&req);
    tct_http_data_clear(&res);

}
//================================================



//=====http server================================
TCT_MHD_DATA* tct_mhd_data_new(void) {
    TCT_MHD_DATA* data = (TCT_MHD_DATA*)malloc(sizeof(TCT_MHD_DATA));
    return data;
}

void tct_mhd_data_init(TCT_MHD_DATA* data, TCT_MHD_PROCESS process_get, TCT_MHD_PROCESS process_post) {
    data->process_get = process_get;
    data->process_post = process_post;
    data->headers = tct_ptr_ary_new();
    tct_ptr_ary_set_free_func(data->headers, tct_http_header_free);
}

void tct_mhd_data_free(TCT_MHD_DATA* data) {
    if(data) {
        tct_ptr_ary_free(data->headers);
        data->headers = NULL;
        free(data);
    }
}

void tct_connection_info_init(TCT_CONNECTION_INFO* info) {
    info->req = tct_http_data_new();
    tct_http_data_init(info->req);
    info->res = tct_http_data_new();
    tct_http_data_init(info->res);
    memset(info->url, 0x00, sizeof(info->url));
}

void tct_connection_info_clear(TCT_CONNECTION_INFO* info) {
    if (info) {
        if (info->req) {
            tct_http_data_clear(info->req);
            free(info->req);
        }

        if (info->res) {
            tct_http_data_clear(info->res);
            free(info->res);
        }
    }
}

void tct_connection_info_free(TCT_CONNECTION_INFO* info) {
    if (info) {
        tct_connection_info_clear(info);
        free(info);
    }
}

//about request
int tct_mhd_get_http_header_key(struct MHD_Connection* connection, TCT_HTTP_DATA* data, const char* key) {
    const char* header_val = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, key);
    if (header_val) {
        TCT_HTTP_HEADER* header = tct_http_header_new();
        tct_http_header_set_name(header, key);
        tct_http_header_set_value(header, (char*)header_val);

        //printf("[header] %s:%s\n", header->name, header->value);

        tct_ptr_ary_add(data->headers, header);
    }

    return tct_ptr_ary_len(data->headers);
}

//MHD_KeyValueIterator get_header_cb(void *cls, enum MHD_ValueKind kind, const char *key, const char *value, size_t value_size) {
enum MHD_Result get_header_cb(void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
    GPtrArray* headers = (GPtrArray*)cls;
    TCT_HTTP_HEADER* header = tct_http_header_new();
    tct_http_header_set_name(header, key);
    tct_http_header_set_value(header, value);

    //printf("[header] %s:%s\n", header->name, header->value);

    tct_ptr_ary_add(headers, header);

    return MHD_YES;

}
int tct_mhd_get_http_header(struct MHD_Connection* connection, TCT_HTTP_DATA* data) {
    int header_cnt = MHD_get_connection_values(connection, MHD_HEADER_KIND, get_header_cb, data->headers);

    return header_cnt;
}

size_t tct_mhd_get_req_body(TCT_CONNECTION_INFO *con_info, const char* upload_data, size_t upload_data_size) {
    TCT_HTTP_DATA* req = con_info->req;

    char *ptr = realloc(req->body, req->body_size + upload_data_size + 1);
    if (ptr == NULL)
        return 0; //out of memory!

    req->body = ptr;
    memcpy(&(req->body[req->body_size]), upload_data, upload_data_size);
    req->body_size += (upload_data_size + 1);
    req->body[req->body_size] = 0;

    //printf("%s\n", req->body);

    return upload_data_size;
}


//about response
int tct_mhd_make_body(TCT_HTTP_DATA* data, json_object* json) {
    const char* json_str = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PLAIN);
    int len = strlen(json_str);

    if (data->body)
        free(data->body);

    data->body_size = len+1;
    data->body = (char*)malloc(data->body_size);
    memset(data->body, 0x00, data->body_size);
    strncpy(data->body, json_str, len);

    return len;
}

//입력된 Status Code로 응답 수행
enum MHD_Result tct_mhd_response(struct MHD_Connection *connection, TCT_HTTP_DATA *res, int status_code) {
    enum MHD_Result ret;
    struct MHD_Response *response = NULL;

    if (res) {
        if (res->body) {
            response = MHD_create_response_from_buffer(strlen(res->body), (void*)res->body, MHD_RESPMEM_MUST_COPY);

            int header_len = tct_ptr_ary_len(res->headers);
            if (header_len > 0) {
                for(int i = 0 ; i < header_len; i++) {
                    TCT_HTTP_HEADER* header = tct_ptr_ary_index(res->headers, i);
                    //printf("set header %s:%s\n", header->name, header->value);
                    MHD_add_response_header(response, header->name, header->value);
                }
            }
        }
        else {
            response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_MUST_COPY);
        }
    }
    else {
        response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_MUST_COPY);
    }

    if (!response)
        return MHD_NO;

    ret = MHD_queue_response(connection, status_code, response);

    MHD_destroy_response(response);

    return ret;
}

//about mhd callback
//접속을 처리하는 callback
//return MHD_YES 하면 다시 call됨
enum MHD_Result tct_mhd_access_handler_cb(void* cls, struct MHD_Connection* connection,
                                        const char* url, const char* method, const char* version,
                                        const char* upload_data, size_t* upload_data_size, void** con_cls) {
    //printf("[start line] Method: %s\n", method);
    //printf("[start line] Request URI: %s\n", url);

    //int status_code = MHD_HTTP_OK; //200 OK
    int status_code = 500; // 500 Internel server error


    TCT_CONNECTION_INFO* con_info = (TCT_CONNECTION_INFO*)*con_cls;
    // 최초 접속의 경우
    if (con_info == NULL) {
        con_info = (TCT_CONNECTION_INFO*)calloc(1, sizeof(TCT_CONNECTION_INFO));
        if (con_info == NULL) {
            return MHD_NO;
        }

        tct_connection_info_init(con_info);
        
        strncpy(con_info->url, url, TCT_URL_LEN-1);
        
        *con_cls = con_info;

        return MHD_YES;
    }

    TCT_MHD_DATA* data = (TCT_MHD_DATA*)cls;

    //get all headers
    tct_mhd_get_http_header(connection, con_info->req);

    //GPtrArray* headers = con_info->req->headers;
    //for(int i = 0; i < tct_ptr_ary_len(headers); i++) {
    //    TCT_HTTP_HEADER* header = tct_ptr_ary_index(headers, i);
    //    printf("[%s] : %s\n", header->name, header->value);
    //}

    if (strcmp(method, "POST") == 0) {
        if (*upload_data_size != 0) {
            //printf("upload data size : %ld\n", *upload_data_size);
            tct_mhd_get_req_body(con_info, upload_data, *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        }
        else {
            //printf("[body] %s\n", con_info->buffer);
            if (data && data->process_post)
                status_code = data->process_post(con_info);
        }
    }
    if (strcmp(method, "GET") == 0) {
        if (data && data->process_get)
            status_code = data->process_get(con_info);
    }

    return tct_mhd_response(connection, con_info->res, status_code);
}


//접속이 완료되는 경우 호출되는 callback
void tct_mhd_complated(void* cls, struct MHD_Connection* connection,
                                void** con_cls, enum MHD_RequestTerminationCode toe) {

    TCT_CONNECTION_INFO* con_info = *con_cls;

    tct_connection_info_free(con_info);
*con_cls = NULL;
}

//example
size_t tct_mhd_process_get_example(TCT_CONNECTION_INFO* info) {
    int status_code = MHD_HTTP_OK;
    //process header
    TCT_HTTP_HEADER* header = tct_http_header_new();
    tct_http_header_init(header);
    tct_http_header_set_name(header, "TEST");
    tct_http_header_set_value(header, "TEST");
    tct_ptr_ary_add(info->res->headers, header);

    //process body
    json_object *root = json_object_new_object();
    json_object_object_add(root, "name", json_object_new_string("PARK"));
    json_object_object_add(root, "phone", json_object_new_string("010000100"));
    json_object_object_add(root, "num", json_object_new_int(3));

    tct_mhd_make_body(info->res, root);

    //unref json object
    json_object_put(root);

    return status_code;
}

size_t tct_mhd_process_post_example(TCT_CONNECTION_INFO* info) {
    int status_code = MHD_HTTP_OK;

    //process header
    TCT_HTTP_HEADER* header = tct_http_header_new();
    tct_http_header_init(header);
    tct_http_header_set_name(header, "TEST");
    tct_http_header_set_value(header, "TEST");
    tct_ptr_ary_add(info->res->headers, header);

    //process body
    json_object *root = json_object_new_object();
    json_object_object_add(root, "name", json_object_new_string("PARK"));
    json_object_object_add(root, "phone", json_object_new_string("010000100"));
    json_object_object_add(root, "num", json_object_new_int(3));

    tct_mhd_make_body(info->res, root);

    //unref json object
    json_object_put(root);

    return status_code;
}

void tct_mhd_example(void) {
    TCT_MHD_DATA* data = tct_mhd_data_new();
    tct_mhd_data_init(data, tct_mhd_process_get_example, tct_mhd_process_post_example);
    
    TCT_HTTP_HEADER* header = tct_http_header_new();
    tct_http_header_init(header);
    tct_http_header_set_name(header, "User");
    tct_http_header_set_value(header, "user1");
    tct_ptr_ary_add(data->headers, header);

    struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
        8080,
        NULL, //MHD_AcceptPolicyCallback apc
        NULL, //extra argument to apc /callback to call to check which clients will be allowed to connect; you can pass NULL in which case connections from any IP will be accepted
        &tct_mhd_access_handler_cb, //MHD_AccessHandlerCallback dh /  default handler for all URIs
        data, //extra argument to dh
        MHD_OPTION_NOTIFY_COMPLETED,
        tct_mhd_complated,
        NULL,
        MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "MHD_start_daemon() error\n");
        //return EXIT_FAILURE;
    }
    while (true) {
        getc(stdin);
    }

    // HTTP 데몬을 종료한다
    MHD_stop_daemon(daemon);

    tct_mhd_data_free(data);
}
//================================================
