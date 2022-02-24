#include "stdio.h"
#include "json-c/json.h"
struct json_object * readFile(char filePath[]);
void parseJson(struct json_object *jobj);

int main(int argc, char **argv){
    char path[] = "test.json";
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
    struct json_object *jobj2, *jarr, *jcurrent;
    type = json_object_get_type(jobj);
    if(type == json_type_object){
        json_object_object_foreach(jobj, key, val){
            struct json_object *value;
            type = json_object_get_type(val);
            switch(type){
                case json_type_object:
                    printf("json_type_object\n\n");
                    json_object_object_get_ex(jobj, key, &jobj2);
                    parseJson(jobj2);
                    break;
                case json_type_null:
                    printf("json_type_null\n\n");
                    break;
                //TODO diplay bool vals
                case json_type_boolean:
                    printf("json_type_boolean\n\n");
                    break;
                case json_type_double: 
                    printf("json_type_double\n");
                    printf("key: %s\n" , key);
                    json_object_object_get_ex(jobj,key, &value);
                    printf("value : %lf\n\n", json_object_get_double(value));
                    break;
                case json_type_int:
                    printf("json_type_int\n");
                    printf("key: %s\n" , key);
                    json_object_object_get_ex(jobj,key, &value);
                    printf("value : %d\n\n", json_object_get_int(value));
                    break;
                //TODO implement arrays
                case json_type_array:
                    printf("json_type_array\n");
                    printf("key: %s\n" , key);
                    json_object_object_get_ex(jobj, key, &jarr);
                    int i;
                    int array_len = json_object_array_length(jarr);
                    for(i = 0 ; i < array_len; i++){
                        jcurrent = json_object_array_get_idx(jarr, i);
                        parseJson(jcurrent);
                    }

                    printf("json_type_array\n\n");
                    break;
                case json_type_string:
                    printf("json_type_string\n");
                    printf("key: %s\n" , key);
                    json_object_object_get_ex(jobj,key, &value);
                    printf("value : %s\n\n", json_object_get_string(value));
                    break;
            }
        }
    }
    else{
        switch(type){
            case json_type_string:
                printf("json_type_string\n");
                printf("value: %s\n\n", json_object_get_string(jobj));
                break;
            case json_type_int:
                printf("json_type_int\n");
                printf("value: %d\n\n", json_object_get_int(jobj));
                break;
            case json_type_null:
                printf("json_type_null\n\n");
                break;
            case json_type_boolean:
                printf("json_type_boolean\n\n");
                break;
            case json_type_double:
                printf("json_type_boolean\n");
                printf("value: %lf\n\n", json_object_get_double(jobj));
                break;


        }
    }
}
