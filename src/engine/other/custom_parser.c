#include "custom_parser.h"
#include <ctype.h>

// Common
/*****************************/


#define MAX_SECTION_SIZE 32

void skip_spaces(const char** text){
    while(**text == ' ' || **text == '\n' || **text == '\t' || **text == '\r'){
        (*text)++;
    }
}

char* load_file(const char* path){
    FILE* file = fopen(path, "r");
    if(file == NULL) return NULL;
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);
    char* data = (char*)malloc(size + 10);
    size_t bytes_read = fread(data, 1, size, file);
    data[bytes_read] = '\0';
    fclose(file);
    return data;
}

bool read_bool(const char** text){
    if(!strncmp((*text), "true", 4)){
        (*text) += 4;
        return TRUE;
    }
    if(!strncmp((*text), "false", 5)){
        (*text) += 5;
        return FALSE;
    }
    return FALSE;
}

double read_number(const char** text){
    char* end;
    double value = strtod(*text, &end);
    *text = end;
    return value;
}

char* read_string(const char** text){
    if(**text != '"') return NULL;
    (*text)++;

    const char* start = (*text);
    while(**text != '"' && **text != '\0'){
        (*text)++;
    }

    int len = (*text) - start;
    char* value = (char*)malloc(len+10);
    
    strncpy(value, start, len);
    value[len] = '\0';

    if(**text == '"') (*text)++;
    return value;
}

char* read_section(const char** text){
    if(**text != '[') return NULL;
    (*text)++;

    size_t size = 0;
    const char* start = (*text);
    while(**text != ']' && **text != '\0'){
        (*text)++;
        size++;
    }
    if(size >= MAX_SECTION_SIZE){
        (*text)++;
        return NULL;
    }

    int len = (*text) - start;
    char* value = (char*)malloc(len+10);
    
    strncpy(value, start, len);
    value[len] = '\0';

    if(**text == ']') (*text)++;
    return value;
}

/*****************************/


// JSON
/*****************************/

bool myjson_read_bool(const char** text){
    return read_bool(text);
}

double myjson_read_number(const char** text){
    return read_number(text);
}

char* myjson_read_string(const char** text){
    return read_string(text);
}

JsonValue* myjson_read_array(const char** text){

    return NULL;
}

JsonValue* myjson_read_object(const char** text){
    if(**text != '{') return NULL;
    (*text)++;
    skip_spaces(text);

    JsonValue* object = (JsonValue*)malloc(sizeof(JsonValue));
    object->type = JSON_TYPE_OBJECT;
    object->obj_values.pairs = (JsonPair*)malloc(sizeof(JsonPair) * 20);
    object->obj_values.pair_count = 0;

    while(**text != '}' && **text != '\0'){
        skip_spaces(text);
        char* key = myjson_read_string(text);
        skip_spaces(text);
        if(**text == ':') (*text)++;
        skip_spaces(text);
        JsonValue* value = myjson_read_value(text);

        JsonPair pair = {key, value};
        object->obj_values.pairs[object->obj_values.pair_count++] = pair;
        
        skip_spaces(text);
        if(**text == ','){
            (*text)++;
            skip_spaces(text);
        }
    }
    
    if(**text == '}') (*text)++;
    return object;
}

JsonValue* myjson_read_value(const char** text){
    skip_spaces(text);

    if(**text == '"'){
        JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
        value->type = JSON_TYPE_STRING;
        value->string_value = myjson_read_string(text);
        return value;
    } 
    if(isdigit(**text) || **text == '-'){
        JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
        value->type = JSON_TYPE_NUMBER;
        value->number_value = myjson_read_number(text);
        return value;
    }
    if(strncmp(*text, "true", 4) == 0 || strncmp(*text, "false", 5) == 0){
        JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
        value->type = JSON_TYPE_BOOL;
        value->bool_value = myjson_read_bool(text);
        return value;
    }
    if(strncmp(*text, "null", 4) == 0){
        JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
        value->type = JSON_TYPE_NULL;
        *text += 4;
        return value;
    }
    if(**text == '{'){
        return myjson_read_object(text);
    }

    return NULL;
}

char* myjson_load_json(const char* path){
    return load_file(path);
}


JsonValue* myjson_get_value(JsonValue* root, const char* path){
    if(root == NULL || root->type != JSON_TYPE_OBJECT) return NULL;

    char* path_copy = strdup(path);
    char* token = strtok(path_copy, ".");
    JsonValue* curr = root;

    while(token != NULL && curr->type == JSON_TYPE_OBJECT){
        bool found = FALSE;
        for(size_t i = 0; i < curr->obj_values.pair_count; i++){
            if(strcmp(curr->obj_values.pairs[i].key, token) == 0){
                curr = curr->obj_values.pairs[i].value;
                found = TRUE;
                break;
            }
        }

        if(!found){
            free(path_copy);
            return NULL;
        }
        token = strtok(NULL, ".");
    }

    free(path_copy);
    return curr;
}

void myjson_print_value(JsonValue* value){
    switch(value->type){
        case JSON_TYPE_NULL:
            printf("null");
            break;
        case JSON_TYPE_OBJECT:
            for(size_t i = 0; i < value->obj_values.pair_count; i++){
                myjson_print_value(value->obj_values.pairs[i].value);
            }
            break;
        case JSON_TYPE_STRING:
            printf("%s", value->string_value);
            break;
        case JSON_TYPE_NUMBER:
            printf("%.2lf", value->number_value);
            break;
        case JSON_TYPE_BOOL:
            printf("%s", value->bool_value ? "true" : "false");
            break;
    }
    printf("\n");
}

void myjson_free_value(JsonValue* value){
    if(value->type == JSON_TYPE_STRING){
        free(value->string_value);
        free(value);
        return;
    }
    else if(value->type == JSON_TYPE_OBJECT){
        for(size_t i = 0; i < value->obj_values.pair_count; i++){
            myjson_free_value(value->obj_values.pairs[i].value);
        }
        free(value);
        return;
    }
}

/*****************************/




// CONFIG
/*****************************/

char* myconfig_load_config(const char* path){
    return load_file(path);
}

char* myconfig_read_key(const char** text){
    const char* start = (*text);
    while(**text != '\0' && **text != '\n' && **text != '\t' && **text != '\r' && **text != ' ' && **text != '='){
        (*text)++;
    }

    int len = (*text) - start;
    char* value = (char*)malloc(len+1);
    
    strncpy(value, start, len);
    value[len] = '\0';

    return value;
}

bool myconfig_read_bool(const char** text){
    return read_bool(text);
}

double myconfig_read_number(const char** text){
    return read_number(text);
}

char* myconfig_read_string(const char** text){
    return read_string(text);
}

char* myconfig_read_section(const char** text){
    return read_section(text);
}

void myconfig_read_comment(const char** text){
    if(**text != '#') return;
    (*text)++;

    skip_spaces(text);
    while(**text != '\n' && **text != '\0'){
        (*text)++;
    }
    skip_spaces(text);
}

ConfigValue* myconfig_read_value(const char** text){
    skip_spaces(text);
    
    if(**text == '#'){
        myconfig_read_comment(text);
    }
    if(**text == '['){
        ConfigValue* value = (ConfigValue*)malloc(sizeof(ConfigValue));
        value->type = CONFIG_TYPE_SECTION;
        value->string_value = myconfig_read_section(text);
        return value;
    }
    if(**text == '"'){
        ConfigValue* value = (ConfigValue*)malloc(sizeof(ConfigValue));
        value->type = CONFIG_TYPE_STRING;
        value->string_value = myconfig_read_string(text);
        return value;
    } 
    if(isdigit(**text) || **text == '-'){
        ConfigValue* value = (ConfigValue*)malloc(sizeof(ConfigValue));
        value->type = CONFIG_TYPE_NUMBER;
        value->number_value = myconfig_read_number(text);
        return value;
    }
    if(strncmp(*text, "true", 4) == 0 || strncmp(*text, "false", 5) == 0){
        ConfigValue* value = (ConfigValue*)malloc(sizeof(ConfigValue));
        value->type = CONFIG_TYPE_BOOL;
        value->bool_value = myconfig_read_bool(text);
        return value;
    }
    if(strncmp(*text, "null", 4) == 0){
        ConfigValue* value = (ConfigValue*)malloc(sizeof(ConfigValue));
        value->type = CONFIG_TYPE_NULL;
        *text += 4;
        return value;
    }
    
    return NULL;
}

ConfigPair myconfig_read_pair(const char** text){
    ConfigPair pair = {NULL, NULL, NULL};
    skip_spaces(text);
    if(**text == '\0') return pair;
    if(**text == '#'){
        myconfig_read_comment(text);
        return pair;
    }
    if(**text == '['){
        ConfigValue* value = (ConfigValue*)malloc(sizeof(ConfigValue));
        value->type = CONFIG_TYPE_SECTION;
        value->string_value = myconfig_read_section(text);
        pair.value = value;
        return pair;
    }

    // pair.section = NULL;
    // char* section = myconfig_read_section(text);
    // if(section != NULL){
    //     printf("works\n");
    //     pair.key = NULL;
    //     pair.value->type = CONFIG_TYPE_SECTION;
    //     pair.value->string_value = section;
    // } else {
    pair.section = (char*)malloc(MAX_SECTION_SIZE);
    skip_spaces(text);
    pair.key = myconfig_read_key(text);
    skip_spaces(text);
    if(**text == '=') (*text)++;
    skip_spaces(text);
    pair.value = myconfig_read_value(text);
    skip_spaces(text);
    // }

    return pair;
}

ConfigPairs myconfig_read_all_pairs(const char** text){
    ConfigPairs pairs = {0};
    char *current_section = (char*)malloc(MAX_SECTION_SIZE);
    current_section[0] = '\0';

    skip_spaces(text);
    while(**text != '\0'){
        skip_spaces(text);
        ConfigPair pair = myconfig_read_pair(text);
        if(pair.key == NULL){
            if(pair.value == NULL){
                continue;
            }
            strcpy(current_section, pair.value->string_value);
        } else{
            strcpy(pair.section, current_section);
            da_append(pairs, pair);
        }

        skip_spaces(text);
    }
    return pairs;
}

ConfigValue* myconfig_get_valuekey_only(ConfigPairs pairs, const char* key){
    ConfigPair* pair;
    da_foreach(pair, pairs){
        if(strcmp((*pair).key, key) == 0){
            return (*pair).value;
        }
    }
    return NULL;
}

ConfigValue* myconfig_get_value(ConfigPairs pairs, const char* key, const char* section){
    ConfigPair* pair;
    da_foreach(pair, pairs){
        if(strcmp((*pair).key, key) == 0 && strcmp((*pair).section, section) == 0){
            return (*pair).value;
        }
    }
    return NULL;
}

char* myconfig_get_value_string(ConfigPairs pairs, const char* key, const char* section){
    ConfigPair* pair;
    da_foreach(pair, pairs){
        if(strcmp((*pair).key, key) == 0 && strcmp((*pair).section, section) == 0){
            return (*pair).value->string_value;
        }
    }
    return "";
}

char* myconfig_get_value_string_new(ConfigPairs pairs, const char* key, const char* section){
    char* cstr = NULL;
    char* str = myconfig_get_value_string((pairs), (key), (section));
    if(str[0] == '\0'){
        cstr = (char*)malloc(1);
        if(cstr)
            cstr[0] = '\0';
    } else {
        cstr = (char*)malloc(strlen(str) + 1);
        if(cstr)
            strcpy(cstr, str);
    }
    return cstr;
}

double myconfig_get_value_number(ConfigPairs pairs, const char* key, const char* section){
    ConfigPair* pair;
    da_foreach(pair, pairs){
        if(strcmp((*pair).key, key) == 0 && strcmp((*pair).section, section) == 0){
            return (*pair).value->number_value;
        }
    }
    return 0.0;
}

bool myconfig_get_value_bool(ConfigPairs pairs, const char* key, const char* section){
    ConfigPair* pair;
    da_foreach(pair, pairs){
        if(strcmp((*pair).key, key) == 0 && strcmp((*pair).section, section) == 0){
            return (*pair).value->bool_value;
        }
    }
    return FALSE;
}



void myconfig_free_pairs(ConfigPairs pairs){
    for(size_t i = 0; i < pairs.count; i++){
        ConfigPair pair = pairs.items[i];
        free(pair.section);
        free(pair.key);
        if(pair.value->type == CONFIG_TYPE_STRING){
            free(pair.value->string_value);
        }
        free(pair.value);
    }
    free(pairs.items);
}

void myconfig_print_pair(ConfigPair pair){
    if(pair.value->type == CONFIG_TYPE_STRING)
        printf("key: %s, value: %s\n", pair.key, pair.value->string_value);
    if(pair.value->type == CONFIG_TYPE_NUMBER)
        printf("key: %s, value: %.2lf\n", pair.key, pair.value->number_value);
    if(pair.value->type == CONFIG_TYPE_BOOL)
        printf("key: %s, value: %s\n", pair.key, pair.value->bool_value ? "true" : "false");
}

/*****************************/



/*****************************/
// XML





/*****************************/