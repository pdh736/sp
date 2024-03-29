#ifndef __TCT_LIBRARY__
#define __TCT_LIBRARY__

#include <pthread.h>
#include <glib.h>
#include <json-c/json.h>
#include <curl/curl.h>
#include <microhttpd.h>

#define TCT_DATE_SIZE       16
#define TCT_DATE_TIME_SIZE  32

#define TCT_PATH_LEN        256 
#define TCT_NAME_LEN        64

#define TCT_URL_LEN         128

#define TCT_HEADER_NAME_LEN 64
#define TCT_HEADER_VAL_LEN  128
#define TCT_HEADER_STR_LEN  TCT_HEADER_NAME_LEN + TCT_HEADER_VAL_LEN


enum TCT_FILE_TYPE {
    TCT_FILE_TYPE_DIR = 0,
    TCT_FILE_TYPE_FILE
};

typedef struct _TCT_THREAD_INFO {
    pthread_t tid;
    pthread_mutex_t mutex;
    void* (*func)(void*);
    void* arg;
}TCT_THREAD_INFO;

typedef struct _TCT_SOCK_PKT_HDR {
    int data_size;
}TCT_SOCK_PKT_HDR;

typedef struct _TCT_SOCK_PKT {
    TCT_SOCK_PKT_HDR hdr;
    char* data;
}TCT_SOCK_PKT;


// struct for example
typedef struct _TCT_FILE_INFO{
    char path[TCT_PATH_LEN];
    char name[TCT_NAME_LEN];
    int size;
    int type;
}TCT_FILE_INFO;

typedef struct _TCT_FILE_CONTENT {
    int num;
    char name[TCT_NAME_LEN];
    int val;
}TCT_FILE_CONTENT;


/** string token
 * @param str_list save parsed string
 * @param original_str target string
 * @param delimeter original_str's delimeter
*/
void tct_string_token(GPtrArray* str_list, char* original_str, char* delimeter);


//=====time======================================
/** stringfy date to date format using delimeter.
 * @param delimeter delimeter in date division.
 * @return string date format using delimeter, must free return val.
*/
char* get_cur_date(char delimeter);

/** stringfy time to time format using delimeters.
 * @param delimeter1 delimeter in date division.
 * @param delimeter2 delimeter in between date and time.
 * @param delimeter3 delimeter in time division.
 * @return string time format using delimeters, must free return val.
*/
char* get_cur_date_time(char delimeter1, char delimeter2, char delimeter3);

/** convert time string to struct tm.
 * @param time pointer of destination.
 * @param str_time string time format.
 * @param is_delimeter if exist delimeter in str_time set 1, ex) 2021-07-10 12:12:10 set 1 
*/
void convert_date_time(struct tm* time, char* str_time, int is_delimeter);

/** calculation of time-to-time difference.
 * @param str_time1 string time format.
 * @param str_time2 string time format.
 * @param is_delimeter if exist delimeter in str_time set 1, ex) 2021-07-10 12:12:10 set 1 
 * @return difference of time to time.
*/
int diff_second(char* str_time1, char* str_time2, int is_delimeter);
//================================================


//=====thread=====================================
/** init TCT_THREAD_INFO
 * @param t_info init target thread info
 * @param func thread function
 * @param arg thread parameter 
*/
void tct_thread_init(TCT_THREAD_INFO* t_info, void* (*func)(void*), void* arg);

/** thread create wrapper
 * @param t_info thread info
 * @return thread create result
*/
int tct_thread_create(TCT_THREAD_INFO* t_info);

/** thread join wrapper
 * @param t_info thread info
*/
void tct_thread_join(TCT_THREAD_INFO* t_info);
//================================================


//=====socket=====================================
////server side
/** init server socket
 * @param port server port
 * @return server socket, if exist error return -1
*/
int tct_sock_init_server(int port);

/** init server socket
 * @param server_sock server socket
 * @return client socket
*/
int tct_sock_accept(int server_sock);

////client side
/** connect to server
 * @param server_ip  server ip
 * @param server_port server port
 * @return client sorcket, if connect fail return -1 
*/
int tct_sock_connect(char* server_ip, int server_port);

////common
/** close wrapper
 * @param socker sockert
*/
void tct_sock_close(int sock);

/** TCT_SOCK_PKT memory alloc and init
 * @return TCT_SOCK_PKT
*/
TCT_SOCK_PKT* tct_sock_alloc_packet(void);

/** free wrapper
 * @param pkt release target
*/
void tct_sock_free_packet(TCT_SOCK_PKT* pkt);

/** make TCT_SOCK_PKT
 * @param pkt release target
 * @param size data size
 * @param data packet real data
*/
void tct_sock_make_packet(TCT_SOCK_PKT* pkt, int size, char* data);

/** read from socket
 * @param sock target socket
 * @param pkt 
 * @return read data length
*/
int tct_sock_read(int sock, TCT_SOCK_PKT* pkt);

/** write to socket
 * @param sock target socket
 * @param pkt 
 * @return write data length
*/
int tct_sock_write(int sock, TCT_SOCK_PKT* pkt);
//================================================


//=====file io====================================
/** exist check and make directory
 * @param path target directory path
*/
int tct_mkdir(char* path);
//================================================


//=====GArray wrapper=============================
//param t : TYPE                // zero terminate, (auto)clear, elements size
#define tct_ary_new(t)          g_array_new(FALSE, FALSE, sizeof(t))
//param x : GArray*
#define tct_ary_len(x)          x->len
//param x : GArray* / t : TYPE / i : index
#define tct_ary_index(x, t, i)  g_array_index(x, t, i)
//param x : GArray*             // garray, free(If TRUE the actual element data is freed as well)
#define tct_ary_free(x)         g_array_free(x, TRUE)
//param x : GArray* / d : data(address) // garray, data, len of data //item shallow copy
#define tct_ary_add(x, d)       g_array_append_vals(x, d, 1)
//param x : GArray* / i : index
#define tct_ary_del(x, i)       g_array_remove_index(x, i)
//param x : GArray* / f : compare func
#define tct_ary_sort(x, f)      g_array_sort(x, (GCompareFunc)f)

//========GArray example==========================
int     tct_ary_comp_name(gpointer a, gpointer b);
int     tct_ary_comp_name_desc(gpointer a, gpointer b);

void    tct_ary_print(GArray* array);

GArray* tct_ary_get_file_list(char* path);
int     tct_ary_make_file_list(GArray* file_list, char* path);

GArray* tct_ary_get_file_content(char* path);
GArray* tct_ary_get_file_content2(char* path, int (*cb)(GArray*, FILE*));
int     tct_ary_get_file_content_cb(GArray* file_contents, FILE* fd);
//================================================


// use when need dynamic alloc example string, pointer member
//=====GPtrArray wrapper==========================
#define tct_ptr_ary_new()               g_ptr_array_new()
// g_ptr_array_new + g_ptr_array_set_free_func
// use when data is string 
#define tct_ptr_ary_new2()              g_ptr_array_new_with_free_func(free_ptr_ary_item)
// g_ptr_array_new_with_free_func using param
#define tct_ptr_ary_new3(f)             g_ptr_array_new_with_free_func(f)
// set free func, use after ptr_array_new
#define tct_ptr_ary_set_free_func(x,f)  g_ptr_array_set_free_func(x, f)
// param x : GPtrArray*
#define tct_ptr_ary_len(x)              x->len
// param x : GPtrArray* / i : index
// return type is void*
#define tct_ptr_ary_index(x, i)         g_ptr_array_index(x, i)
// param x : GPtrArray*              // gptrarray, free(If TRUE the actual element data is freed as well)
#define tct_ptr_ary_free(x)             g_ptr_array_free(x, TRUE)
// param x : GPtrArray* / d : data  // gptrarray, data
#define tct_ptr_ary_add(x, d)           g_ptr_array_add(x, d)
// param x : GPtrArray* / i : index
#define tct_ptr_ary_del(x, i)           g_ptr_array_remove(x, i)
// param x : GPtrArray* / f : compare func
#define tct_ptr_ary_sort(x, f)          g_ptr_array_sort(x, (GCompareFunc)f)

void free_ptr_ary_item(gpointer data);

//========GPtrArray example=======================
int tct_ptr_ary_comp_name(gpointer a, gpointer b);
int tct_ptr_ary_comp_name_desc(gpointer a, gpointer b);

void tct_ptr_ary_print(GPtrArray* array);
void tct_ptr_ary_print2(GPtrArray* array, void (*cb)(void*));
void tct_ptr_ary_print_cb(void* item);

GPtrArray* tct_ptr_ary_get_file_list(char* path);
int tct_ptr_ary_make_file_list(GPtrArray* file_list, char* path);

GPtrArray* tct_ptr_ary_get_file_content(char* path);
GPtrArray* tct_ptr_ary_get_file_content2(char* path, int (*cb)(GPtrArray*, FILE*));
int tct_ptr_ary_get_file_content_cb(GPtrArray* file_content, FILE* fd);

void tct_file_content_free(void* item);
//================================================


//=====GQueue wrapper=============================
void tct_queue_item_free(gpointer data);

GQueue* tct_queue_new(void);

/** free queue
 * @param q target queue
 * @param is_item_dynamic when item dynamic allocated :1, static alllocated : 0
*/
void tct_queue_free(GQueue* q, void (*free_func)(void*));
void tct_queue_clear(GQueue* q, void (*free_func)(void*));

//param x : GQueue*
#define tct_queue_len(q)    q->length

#define tct_queue_push(q, data)         g_queue_push_tail(q, data)
#define tct_queue_push_head(q, data)    g_queue_push_head(q, data)
#define tct_queue_pop(q)                g_queue_pop_head(q)
#define tct_queue_pop_nth(q, n)         g_queue_pop_nth(q, n)
#define tct_queue_pop_tail(q)           g_queue_pop_tail(q)
#define tct_queue_peek(q)               g_queue_peek_head(q)
#define tct_queue_peek_nth(q, n)        g_queue_peek_nth(q)
#define tct_queue_peek_tail(q)          g_queue_peek_tail(q)

void tct_queue_sort(GQueue* q, GCompareDataFunc func);
//================================================


//=====json=======================================
void tct_file_to_json_example(char* file_path);
void tct_json_to_file_example(char* file_path);
void tct_json_iter_example(char* file_path);
//================================================


//=====http common================================
typedef struct _TCT_HTTP_HEADER {
    char* name;
    char* value;
}TCT_HTTP_HEADER;

typedef struct _TCT_HTTP_DATA {
    GPtrArray* headers;
    char* body;
    int body_size;
}TCT_HTTP_DATA;

TCT_HTTP_HEADER* tct_http_header_new(void);
void    tct_http_header_init(TCT_HTTP_HEADER* header);
void    tct_http_header_free(void* param);
int     tct_http_header_set_name(TCT_HTTP_HEADER* header, const char* name);
int     tct_http_header_set_value(TCT_HTTP_HEADER* header, const char* value);


TCT_HTTP_DATA* tct_http_data_new(void);
void    tct_http_data_init(TCT_HTTP_DATA* data);
void    tct_http_data_clear(TCT_HTTP_DATA* data);
void    tct_http_data_free(TCT_HTTP_DATA* data);
//================================================


//=====http client(curl)==========================
//for curl

//for res
size_t  tct_curl_cb(void *data, size_t size, size_t nmemb, void *userp);
size_t  tct_curl_header_cb(void *data, size_t size, size_t nitems, void *userp);

//set req header
struct curl_slist* tct_curl_set_header(CURL* curl, GPtrArray* headers);

/**
 * @param url "http://127.0.0.1:8080/helloworld"
 * @param req req data. can NULL. use after init .
 * @param res res data. use after init .
 * @return http status code. 
*/
int tct_curl_http_get(char* url, TCT_HTTP_DATA* req, TCT_HTTP_DATA* res);
int tct_curl_http_post(char* url, TCT_HTTP_DATA* req, TCT_HTTP_DATA* res);

void tct_curl_example(void);

//================================================


//=====http server================================
typedef struct _TCT_CONNECTION_INFO {
    TCT_HTTP_DATA* req;
    TCT_HTTP_DATA* res;
    char url[TCT_URL_LEN];
}TCT_CONNECTION_INFO;

typedef size_t (*TCT_MHD_PROCESS)(TCT_CONNECTION_INFO* info);

//pass MHD data 
typedef struct _TCT_MHD_DATA {
    TCT_MHD_PROCESS process_get;
    TCT_MHD_PROCESS process_post;
    GPtrArray* headers;
}TCT_MHD_DATA;

TCT_MHD_DATA* tct_mhd_data_new(void);
void tct_mhd_data_init(TCT_MHD_DATA* data, TCT_MHD_PROCESS process_get, TCT_MHD_PROCESS process_post);
void tct_mhd_data_free(TCT_MHD_DATA* data);

void tct_connection_info_init(TCT_CONNECTION_INFO* info);
void tct_connection_info_clear(TCT_CONNECTION_INFO* info);
void tct_connection_info_free(TCT_CONNECTION_INFO* info);

//about request
int     tct_mhd_get_http_header_key(struct MHD_Connection* connection, TCT_HTTP_DATA* data, const char* key);
int     tct_mhd_get_http_headers(struct MHD_Connection* connection, TCT_HTTP_DATA* data, GPtrArray* check_list);
size_t  tct_mhd_get_req_body(TCT_CONNECTION_INFO *con_info, const char* upload_data, size_t upload_data_size);

//about response 
int tct_mhd_make_body(TCT_HTTP_DATA* data, json_object* json);
enum MHD_Result tct_mhd_response(struct MHD_Connection *connection, TCT_HTTP_DATA *res, int status_code);

//mhd about callback
enum MHD_Result tct_mhd_access_handler_cb(void* cls, struct MHD_Connection* connection,
    const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls);

void tct_mhd_complated(void* cls, struct MHD_Connection* connection, void** con_cls, enum MHD_RequestTerminationCode toe);


//example
size_t tct_mhd_process_get_example(TCT_CONNECTION_INFO* info);
size_t tct_mhd_process_post_example(TCT_CONNECTION_INFO* info);
void tct_mhd_example(void);
//================================================



#endif
