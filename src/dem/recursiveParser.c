#include "stdio.h"
#include "json-c/json.h"
struct json_object * readFile(char filePath[]);
void parseJson(struct json_object *jobj);

int main(int argc, char **argv){
    char path[] = "test2.json";
    parseJson(readFile(path));
}

struct json_object * readFile(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    struct json_object *jobj = json_tokener_parse(buffer);
    return jobj;
}

void parseJson(struct json_object *jobj){
    enum json_type type;
    struct json_object *jobj2;
    json_object_object_foreach(jobj, key, val){
        type = json_object_get_type(val);
        switch(type){
            case json_type_object:
                printf("json_type_object\n");
                json_object_object_get_ex(jobj, key, &jobj2);
                parseJson(jobj2);
                break;
            case json_type_null:
                printf("json_type_null\n");
                break;
            case json_type_boolean:
                printf("json_type_boolean\n");
                break;
            case json_type_double: 
                printf("json_type_double\n");
                break;
            case json_type_int:
                printf("json_type_int\n");
                break;
            //fix arrays
            case json_type_array: 
                printf("json_type_array\n");
                break;
            case json_type_string: 
                printf("json_type_string\n");
                break;
        }
    }
}