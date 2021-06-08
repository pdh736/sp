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

int main(int argc, char *argv[]){

    json_object *myobj = json_object_new_object();
    json_object_object_add(myobj, "name", json_object_new_string("KIM"));
    json_object_object_add(myobj, "phone", json_object_new_string("010000000000"));
    const char *result_json = json_object_to_json_string_ext(myobj, JSON_C_TO_STRING_PLAIN);
    printf("json: %s\n", result_json);

    json_object *root, *name, *phone;
    root = json_tokener_parse(result_json);
    json_object_object_get_ex(root, "name", &name);
    json_object_object_get_ex(root, "phone", &phone);
    printf("name: %s, phone: %s\n", json_object_get_string(name), json_object_get_string(phone));

    const char *json_str = "{\"name\":\"park\", \"id_num\":3}";
    json_object *root2, *name2, *id_num;
    root = json_tokener_parse(json_str);
    json_object_object_get_ex(root, "name", &name2);
    json_object_object_get_ex(root, "id_num", &id_num);
    printf("name : %s, id_num : %s\n", json_object_get_string(name2), json_object_get_int(num_id));

    return EXIT_SUCCESS;
}

