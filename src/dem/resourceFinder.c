#include "stdio.h"
#include "json-c/json.h"
#include "dirent.h"
#include "string.h"
struct json_object * readFile(char filePath[]);
void parseJson(struct json_object *jobj);
void traverseDirectory(char rootDir[]);
void findDatatype(char filePath[]);
void computerSystemCollection(char filePath[]);
void storageController(char filePath[]);
void storageControllerCollection(char filePath[]);
void volume(char filePath[]);
void volumeCollection(char filePath[]);
void storage(char filePath[]);
void storageCollection(char filePath[]);
void computerSystem(char filePath[]);



int main()
{
    char rootDir[] = "Systems";
    traverseDirectory(rootDir);
}

void traverseDirectory(char rootDir[]){
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    
    
    DIR *dr = opendir(rootDir); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory\n" ); 
        return 0; 
    } 

    while ((de = readdir(dr)) != NULL)
    {   
        printf("------\n");
        printf("%s\n", rootDir);
        int lenName = strlen(de->d_name);
        char currentItem[lenName+1];
        strcpy(currentItem, de->d_name);
        char slash[12] = "/";
        char index[] = "index.json";
        int lenRoot = strlen(rootDir);
        char rootClone[lenRoot + 1 + 1 + strlen(currentItem)];
        strcpy(rootClone, rootDir);
        printf("%s\n", currentItem);
        if(strcmp(currentItem, index) != 0 && strcmp(currentItem, ".") != 0 && strcmp(currentItem, "..") != 0){
            strcat(rootClone, slash);
            strcat(rootClone, currentItem);
            printf("%s\n", rootClone);
            traverseDirectory(rootClone);
        }
        else if (strcmp(currentItem, index)==0){
            strcat(slash, index);
            strcat(rootClone, slash);
            findDatatype(rootClone);
        }

    }
    closedir(dr);     
}
void findDatatype(char filePath[]){
    
    FILE *fp;
    char buffer[2048];
    fp = fopen(filePath, "r");
    fread(buffer, 2048, 1, fp);
    fclose(fp);


    struct json_object *jobj;
    struct json_object *data_type;
    jobj = json_tokener_parse(buffer);

    json_object_object_get_ex(jobj, "@odata.type", &data_type);
    int lenDataType = strlen(json_object_get_string(data_type));
    char data_type_string[lenDataType + 1];
    strcpy(data_type_string, json_object_get_string(data_type));
    if (strcmp(data_type_string, "#ComputerSystemCollection.ComputerSystemCollection") == 0){
        computerSystemCollection(filePath);
    }
    else if (strcmp(data_type_string,  "#ComputerSystem.v1_8_0.ComputerSystem") == 0){
        computerSystem(filePath);
    }
    else if (strcmp(data_type_string,  "#StorageCollection.StorageCollection") == 0){
        storageCollection(filePath);
    }
    else if (strcmp(data_type_string,  "#Storage.v1_10_0.Storage") == 0){
        storage(filePath);
    }
    else if (strcmp(data_type_string,  "#VolumeCollection.VolumeCollection") == 0){
        volumeCollection(filePath);
    }
    else if (strcmp(data_type_string,  "#Volume.v1_5_0.Volume") == 0){
        volume(filePath);
    }
    else if (strcmp(data_type_string,  "#StorageControllerCollection.StorageControllerCollection") == 0){
        storageControllerCollection(filePath);
    }
    else if (strcmp(data_type_string,  "#StorageController.v1_1_0.StorageController") == 0){
        storageController(filePath);
    }  
}
void computerSystemCollection(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("computerSystemCollection name value: %s\n", json_object_get_string(name));

}
void storageController(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("storageController name value: %s\n", json_object_get_string(name));

}
void storageControllerCollection(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("storageControllerCollection name value: %s\n", json_object_get_string(name));

}
void volume(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("volume name value: %s\n", json_object_get_string(name));

}
void volumeCollection(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("volumeCollection name value: %s\n", json_object_get_string(name));

}
void storage(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("storage name value: %s\n", json_object_get_string(name));

}
void storageCollection(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("storageCollection name value: %s\n", json_object_get_string(name));

}
void computerSystem(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath, "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("computerSystem name value: %s\n", json_object_get_string(name));

}
/*
void serviceRoot(char filePath[]){
    //open and read file
    FILE *fp;
    char buffer[1024];
    fp = fopen(filePath);
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    //set expected fields per data type
    struct json_object *jobj;
    struct json_object *name;

    jobj = json_tokener_parse(buffer);
    json_object_object_get_ex(jobj, "Name", &name);

    printf("serviceRoot name value: %s\n", json_object_get_string(name));

}
*/

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
