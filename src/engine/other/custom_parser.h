#ifndef MYCUSTOM_PARSER_H_
#define MYCUSTOM_PARSER_H_

#include "core.h"
#include "da.h"

/*****************************/
// JSON

typedef enum{
    JSON_TYPE_NULL = 0,
    JSON_TYPE_OBJECT,
    JSON_TYPE_ARRAY,
    JSON_TYPE_NUMBER,
    JSON_TYPE_STRING,
    JSON_TYPE_BOOL
} JsonType;

#define STR_HEX_TO_NUMBER(str_hex) strtod((str_hex), NULL)

struct JsonPair;

typedef struct JsonValue{
    JsonType type;
    union{
        char* string_value;
        double number_value;
        bool bool_value;
        struct JsonValue* array_value;
        struct{
            struct JsonPair* pairs;
            size_t pair_count;
        } obj_values;
    };
} JsonValue;


#define PRINT_PAIR(pair) \
do{\
    if((pair).value.type == JSON_TYPE_STRING)\
        printf("%s : %s\n", (pair).key, (pair).value.string_value);\
    if((pair).value.type == JSON_TYPE_NUMBER)\
        printf("%s : %lf\n", (pair).key, (pair).value.number_value);\
    if((pair).value.type == JSON_TYPE_BOOL)\
        printf("%s : %s\n", (pair).key, (pair).value.bool_value ? "true" : "false");\
}while(0)

typedef struct JsonPair{
    char* key;
    JsonValue* value;
} JsonPair;


char* myjson_load_json(const char* path);
void myjson_free_value(JsonValue* value);
JsonValue* myjson_read_value(const char** text);
JsonValue* myjson_get_value(JsonValue* root, const char* path);
void myjson_print_value(JsonValue* value);

/*****************************/



/*****************************/
// CONFIG

typedef enum{
    CONFIG_TYPE_NULL = 0,
    CONFIG_TYPE_BOOL,
    CONFIG_TYPE_NUMBER,
    CONFIG_TYPE_STRING,
    CONFIG_TYPE_SECTION
} ConfigType;

typedef struct{
    ConfigType type;
    union{
        char* string_value;
        double number_value;
        bool bool_value;
    };
} ConfigValue;

typedef struct{
    char* section;
    char* key;
    ConfigValue* value;
} ConfigPair;

typedef struct{
    ConfigPair* items;
    size_t count;
    size_t capacity;
} ConfigPairs;



char* myconfig_load_config(const char* path);
ConfigValue* myconfig_read_value(const char** text);
ConfigPair myconfig_read_pair(const char** text);
ConfigPairs myconfig_read_all_pairs(const char** text);

ConfigValue* myconfig_get_value_key_only(ConfigPairs pairs, const char* key);
ConfigValue* myconfig_get_value(ConfigPairs pairs, const char* key, const char* section);

void myconfig_load_value_string(ConfigPairs pairs, const char* key, const char* section);
char* myconfig_get_value_string_new(ConfigPairs pairs, const char* key, const char* section);
char* myconfig_get_value_string(ConfigPairs pairs, const char* key, const char* section);
double myconfig_get_value_number(ConfigPairs pairs, const char* key, const char* section);
bool myconfig_get_value_bool(ConfigPairs pairs, const char* key, const char* section);

void myconfig_free_pairs(ConfigPairs pairs);

/*****************************/
#endif //MYCUSTOM_PARSER_H_