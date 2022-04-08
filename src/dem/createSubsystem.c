#include "stdio.h"
#include "json-c/json.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include "unistd.h"
#include "sys/stat.h"

struct json_object *readFile(char filePath[]);
char *parseJson(struct json_object *jobj, int num_layers, bool arrayElement, char *buffer, int size_of_buffer, char property[], struct json_object *newValue, char operation[], int found);
void tabControl(int num_layers);
char *createSubsystem(char *subsystem_name, char *nvmeof_namespace, char *ipAddress, char *protocol, char *transport_service_id, char *namespace);
char *test(char *subsystem_name);


char *createSubsystem(char *subsystem_name, char *nvmeof_namespace, char *ipAddress, char *protocol, char *transport_service_id, char *namespace){
    struct stat st = {0};

    if(stat(("Systems/Sys-1/Storage/%s", subsystem_name), &st) == -1){
        mkdir("Systems/Sys-1/Storage/%s", subsystem_name);
        return("Pass!");
    }
}

char *test(char *subsystem_name){
    struct stat st = {0};
    char *path = strcat("Systems/Sys-1/Storage/", subsystem_name);
    if(stat(path, &st) == -1){
        mkdir(path, 0700);
        return("Pass!");
    }
}

















int main(int argc, char **argv)
{
    printf(test("test"));
}
struct json_object *readFile(char filePath[])
{
    // open and read file
    FILE *fp;
    char file_buffer[1024];
    fp = fopen(filePath, "r");
    fread(file_buffer, 1024, 1, fp);
    fclose(fp);
    struct json_object *jobj = json_tokener_parse(file_buffer);
    return jobj;
}
void tabControl(int num_layers)
{
    for (int i = 0; i < num_layers; i++)
    {
        printf("\t");
    }
}
char *parseJson(struct json_object *jobj, int num_layers, bool arrayElement, char *buffer, int size_of_buffer, char property[], struct json_object *newValue, char operation[], int found)
{
    enum json_type type;
    struct json_object *jobj2, *jarr, *jcurrent;
    type = json_object_get_type(jobj);
    if (type == json_type_object)
    {
        // tabControl(num_layers);
        for (int i = 0; i < num_layers; i++)
        {
            size_of_buffer += 1;
            buffer = realloc(buffer, size_of_buffer * sizeof(char));
            strcat(buffer, "\t");
        }
        size_of_buffer += 15;
        buffer = realloc(buffer, size_of_buffer * sizeof(char));
        strcat(buffer, "{\n");

        // printf("{\n");
        json_object_object_foreach(jobj, key, val)
        {
            struct json_object *value;
            if (strcmp(key, property) == 0 && strcmp("put", operation) == 0)
            {
                found = 1;
            }
            if (strcmp(key, property) == 0 && strcmp("patch", operation) == 0)
            {
                type = json_object_get_type(newValue);
                switch (type)
                {
                case json_type_object:
                    size_of_buffer += (strlen(key) + 8);
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t\"");
                    strcat(buffer, key);
                    strcat(buffer, "\" : ");
                    // printf("\t\"%s\" : \n\n", key);
                    json_object_object_get_ex(jobj, key, &jobj2);
                    parseJson(jobj2, num_layers + 1, arrayElement, buffer, size_of_buffer, property, newValue, operation, found);
                    break;
                case json_type_null:
                    size_of_buffer += (strlen(key) + 10);
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t\"");
                    strcat(buffer, key);
                    strcat(buffer, "\" : \"\"");
                    // printf("\t\"%s\" : \"\"\n\n", key);
                    break;
                case json_type_boolean:
                    size_of_buffer += (strlen(key) + 6);
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t\"");
                    strcat(buffer, key);
                    strcat(buffer, "\" : ");
                    // printf("\t\"%s\" : ", key);
                    if (json_object_get_boolean(value) == true)
                    {
                        size_of_buffer += (strlen(key) + 9);
                        buffer = realloc(buffer, size_of_buffer * sizeof(char));
                        strcat(buffer, "\"true\"");
                    }
                    else
                    {
                        size_of_buffer += (strlen(key) + 10);
                        buffer = realloc(buffer, size_of_buffer * sizeof(char));
                        strcat(buffer, "\"false\"");
                    }

                    break;
                case json_type_double:
                    json_object_object_get_ex(jobj, key, &value);
                    char double_str[50];
                    double num = json_object_get_double(value);
                    sprintf(double_str, "%f", num);
                    size_of_buffer += (strlen(key) + 10 + strlen(double_str));
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t\"");
                    strcat(buffer, key);
                    strcat(buffer, "\" : \"");
                    strcat(buffer, double_str);
                    strcat(buffer, "\"");
                    // printf("\t\"%s\" : \"%lf\"\n\n", key, json_object_get_double(value));
                    break;
                case json_type_int:
                    size_of_buffer += strlen(key);
                    char int_str[50];
                    int numInt = json_object_get_int(newValue);
                    sprintf(int_str, "%d", numInt);
                    size_of_buffer += (10 + strlen(int_str));
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t\"");
                    strcat(buffer, key);
                    strcat(buffer, "\" : ");
                    strcat(buffer, int_str);
                    strcat(buffer, "");
                    // printf("\t\"%s\" : \"%d\"\n\n", key, json_object_get_int(value));
                    break;
                case json_type_array:
                    size_of_buffer += (strlen(key) + 8);
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t\"");
                    strcat(buffer, key);
                    strcat(buffer, "\" : [\n");
                    // printf("\t\"%s\" : [\n", key);
                    json_object_object_get_ex(jobj, key, &jarr);
                    int i;
                    int array_len = json_object_array_length(jarr);
                    for (i = 0; i < array_len; i++)
                    {
                        jcurrent = json_object_array_get_idx(jarr, i);
                        parseJson(jcurrent, num_layers + 1, 1, buffer, size_of_buffer, property, newValue, operation, found);
                        strcat(buffer, ",\n\n");
                    }
                    strcpy(&buffer[strlen(buffer) - 3], &buffer[strlen(buffer) - 2]);
                    for (int i = 0; i < num_layers; i++)
                    {
                        size_of_buffer += 1;
                        buffer = realloc(buffer, size_of_buffer * sizeof(char));
                        strcat(buffer, "\t");
                    }
                    // tabControl(num_layers);
                    size_of_buffer += 3;
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t]\n");
                    // printf("\t]\n");
                    break;
                case json_type_string:
                    size_of_buffer += (strlen(key) + strlen(json_object_get_string(newValue)) + 10);
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t\"");
                    strcat(buffer, key);
                    strcat(buffer, "\" : \"");
                    strcat(buffer, json_object_get_string(newValue));
                    strcat(buffer, "\"");
                    // printf("\t\"%s\" : \"%s\"\n\n", key, json_object_get_string(value));
                    break;
                }
                strcat(buffer, ",\n\n");
                continue;
            }
            // tabControl(num_layers);
            for (int i = 0; i < num_layers; i++)
            {
                size_of_buffer += 1;
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                strcat(buffer, "\t");
            }

            type = json_object_get_type(val);
            switch (type)
            {
            case json_type_object:
                size_of_buffer += (strlen(key) + 8);
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                strcat(buffer, "\t\"");
                strcat(buffer, key);
                strcat(buffer, "\" : ");
                // printf("\t\"%s\" : \n\n", key);
                json_object_object_get_ex(jobj, key, &jobj2);
                parseJson(jobj2, num_layers + 1, arrayElement, buffer, size_of_buffer, property, newValue, operation, found);
                break;
            case json_type_null:
                size_of_buffer += (strlen(key) + 10);
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                strcat(buffer, "\t\"");
                strcat(buffer, key);
                strcat(buffer, "\" : \"\"");
                // printf("\t\"%s\" : \"\"\n\n", key);
                break;
            case json_type_boolean:
                size_of_buffer += (strlen(key) + 6);
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                strcat(buffer, "\t\"");
                strcat(buffer, key);
                strcat(buffer, "\" : ");
                // printf("\t\"%s\" : ", key);
                if (json_object_get_boolean(value) == true)
                {
                    size_of_buffer += (strlen(key) + 9);
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\"true\"");
                }
                else
                {
                    size_of_buffer += (strlen(key) + 10);
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\"false\"");
                }

                break;
            case json_type_double:
                json_object_object_get_ex(jobj, key, &value);
                char double_str[50];
                double num = json_object_get_double(value);
                sprintf(double_str, "%f", num);
                size_of_buffer += (strlen(key) + 10 + strlen(double_str));
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                strcat(buffer, "\t\"");
                strcat(buffer, key);
                strcat(buffer, "\" : \"");
                strcat(buffer, double_str);
                strcat(buffer, "\"");
                // printf("\t\"%s\" : \"%lf\"\n\n", key, json_object_get_double(value));
                break;
            case json_type_int:
                json_object_object_get_ex(jobj, key, &value);
                char int_str[50];
                int numInt = json_object_get_int(value);
                sprintf(int_str, "%d", numInt);
                size_of_buffer += (strlen(key) + 10 + strlen(int_str));
                printf("%s", buffer);
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                if (buffer == NULL)
                {
                    printf("NULL BUFFER");
                }
                strcat(buffer, "\t\"");
                strcat(buffer, key);
                strcat(buffer, "\" : ");
                strcat(buffer, int_str);
                strcat(buffer, "");
                // printf("\t\"%s\" : \"%d\"\n\n", key, json_object_get_int(value));
                break;
            case json_type_array:
                size_of_buffer += (strlen(key) + 8);
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                strcat(buffer, "\t\"");
                strcat(buffer, key);
                strcat(buffer, "\" : [\n");
                // printf("\t\"%s\" : [\n", key);
                json_object_object_get_ex(jobj, key, &jarr);
                int i;
                int array_len = json_object_array_length(jarr);
                for (i = 0; i < array_len; i++)
                {
                    jcurrent = json_object_array_get_idx(jarr, i);
                    parseJson(jcurrent, num_layers + 1, 1, buffer, size_of_buffer, property, newValue, operation, found);
                    strcat(buffer, ",\n\n");
                }
                strcpy(&buffer[strlen(buffer) - 3], &buffer[strlen(buffer) - 2]);
                for (int i = 0; i < num_layers; i++)
                {
                    size_of_buffer += 1;
                    buffer = realloc(buffer, size_of_buffer * sizeof(char));
                    strcat(buffer, "\t");
                }
                // tabControl(num_layers);
                size_of_buffer += 3;
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                strcat(buffer, "\t]\n");
                // printf("\t]\n");
                break;
            case json_type_string:
                json_object_object_get_ex(jobj, key, &value);
                size_of_buffer += (strlen(key) + strlen(json_object_get_string(value)) + 10);
                buffer = realloc(buffer, size_of_buffer * sizeof(char));
                strcat(buffer, "\t\"");
                strcat(buffer, key);
                strcat(buffer, "\" : \"");
                strcat(buffer, json_object_get_string(value));
                strcat(buffer, "\"");
                // printf("\t\"%s\" : \"%s\"\n\n", key, json_object_get_string(value));
                break;
            }
            strcat(buffer, ",\n\n");
        }
        strcpy(&buffer[strlen(buffer) - 3], &buffer[strlen(buffer) - 2]);

        for (int i = 0; i < num_layers; i++)
        {
            size_of_buffer += 1;
            buffer = realloc(buffer, size_of_buffer * sizeof(char));
            strcat(buffer, "\t");
        }
        // tabControl(num_layers);
        size_of_buffer = size_of_buffer + 1;
        buffer = realloc(buffer, size_of_buffer * sizeof(char));
        strcat(buffer, "}");
        // printf("}");
        if (arrayElement == true)
        {
            size_of_buffer += 1;
            buffer = realloc(buffer, size_of_buffer * sizeof(char));
            strcat(buffer, ",");
            // printf(",");
        }
        size_of_buffer += 1;
        buffer = realloc(buffer, size_of_buffer * sizeof(char));
        strcat(buffer, "\n");
        // printf("\n");
    }
    else
    {
        // tabControl(num_layers);
        for (int i = 0; i < num_layers; i++)
        {
            size_of_buffer += 1;
            buffer = realloc(buffer, size_of_buffer * sizeof(char));
            strcat(buffer, "\t");
        }
        switch (type)
        {
        case json_type_string:
            // printf("\t\"%s\",\n\n", json_object_get_string(jobj));
            size_of_buffer += strlen(json_object_get_string(jobj)) + 6;
            buffer = realloc(buffer, size_of_buffer * sizeof(char));
            strcat(buffer, "\t\"");
            strcat(buffer, json_object_get_string(jobj));
            strcat(buffer, "\"");
            break;
        case json_type_int:
            printf("\t\"%d\",\n\n", json_object_get_int(jobj));
            break;
        case json_type_null:
            printf("\tjson_type_null\n\n");
            break;
        case json_type_boolean:
            if (json_object_get_boolean(jobj) == true)
            {
                printf("\t\"true\",\n\n");
            }
            else
            {
                printf("\t\"false\",\n\n");
            }

            break;
        case json_type_double:
            printf("\t\"%lf\",\n\n", json_object_get_double(jobj));
            break;
        }
    }
    if (num_layers == 0)
    {
        if (found == 0 && strcmp("put", operation) == 0)
        {
            strcpy(&buffer[strlen(buffer) - 4], &buffer[strlen(buffer) - 1]);
            size_of_buffer += (strlen(property) + strlen(json_object_get_string(newValue)) + 15);
            buffer = realloc(buffer, size_of_buffer * sizeof(char));
            strcat(buffer, ",\n\t\"");
            strcat(buffer, property);
            strcat(buffer, "\" : \"");
            strcat(buffer, json_object_get_string(newValue));
            strcat(buffer, "\"\n}\n");
        }
        return (buffer);
    }
}
