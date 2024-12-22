#define _CRT_SECURE_NO_WARNINGS
#define MAX_CHILDREN 100000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <time.h>

// Sample JSON string 
const char* sample_json_string = "{\n"
"    \"id\": 123,\n"
"    \"name\": \"test-obj\",\n"
"    \"children\": [\n"
"    {\n"
"        \"id\": 1,\n"
"        \"name\": \"test-child1\",\n"
"        \"parameters\": {\n"
"        \"param1\": \"val1\",\n"
"        \"param2\": \"val2\",\n"
"        \"param3\": \"val3\"\n"
"        }\n"
"    },\n"
"    {\n"
"        \"id\": 2,\n"
"        \"name\": \"test-child2\",\n"
"        \"parameters\": {\n"
"        \"param4\": \"val1\",\n"
"        \"param5\": \"val2\",\n"
"        \"param6\": \"val3\"\n"
"        }\n"
"    }\n"
"    ],\n"
"    \"additional\": {\n"
"    \"a1\": \"v1\",\n"
"    \"a2\": \"v2\"\n"
"    }\n"
"}\n";

/**
 * Generates a large JSON string with a specified number of child elements.
 * This function creates a JSON object with a "children" array.
 */
char* generate_large_json(size_t num_children) {
    json_t* root = json_object();
    json_t* children = json_array();
    json_object_set_new(root, "id", json_integer(123));
    json_object_set_new(root, "name", json_string("test-obj"));
    json_object_set_new(root, "additional", json_pack("{s:s, s:s}", "a1", "v1", "a2", "v2"));
    json_object_set_new(root, "children", children);

    for (size_t i = 0; i < num_children; ++i) {
        json_t* child = json_object();
        char name[20];
        sprintf(name, "test-child%zu", i + 1);

        json_t* parameters = json_pack("{s:s, s:s, s:s}",
            "param1", "val1",
            "param2", "val2",
            "param3", "val3"
        );
        json_object_set_new(child, "id", json_integer(i + 1));
        json_object_set_new(child, "name", json_string(name));
        json_object_set_new(child, "parameters", parameters);
        json_array_append_new(children, child);
    }

    char* json_string = json_dumps(root, JSON_INDENT(2));
    json_decref(root);
    return json_string;
}

/**
 * Recursively flattens a JSON object into a single-level JSON object.
 * This function handles both objects and arrays by creating a key for each
 * nested element, combining parent and child keys with a separator.
 */
void flatten_json_recursion(json_t* input, json_t* output, const char* prefix) {
    if (json_is_object(input)) {
        const char* key;
        json_t* value;
        json_object_foreach(input, key, value) {
            size_t prefix_len = strlen(prefix);
            size_t key_len = strlen(key);
            size_t new_prefix_len = prefix_len + key_len + 2;  // 1 for '_' and 1 for '\0'
            char* new_prefix = (char*)malloc(new_prefix_len);
            if (!new_prefix) {
                fprintf(stderr, "Memory allocation failed!\n");
                return;
            }
            if (prefix[0] == '\0') { 
                snprintf(new_prefix, new_prefix_len, "%s", key);
            }
            else {
                snprintf(new_prefix, new_prefix_len, "%s_%s", prefix, key);
            }
            flatten_json_recursion(value, output, new_prefix);
            free(new_prefix); 
        }
    }
    else if (json_is_array(input)) {
        for (size_t i = 0; i < json_array_size(input); ++i) {
            size_t prefix_len = strlen(prefix);
            size_t new_prefix_len = prefix_len + 20 + 1;  // Maximum bytes for size_t + 1 for \0
            char* new_prefix = (char*)malloc(new_prefix_len);
            if (!new_prefix) {
                fprintf(stderr, "Memory allocation failed!\n");
                return;
            }
            snprintf(new_prefix, new_prefix_len, "%s_%zu", prefix, i);         
            flatten_json_recursion(json_array_get(input, i), output, new_prefix);
            free(new_prefix);  
        }
    }
    else {
        if (json_object_set(output, prefix, input) != 0) {
            fprintf(stderr, "Failed to set key: %s\n", prefix);
        }
    }
}

int main() {
    char* json_string = generate_large_json(MAX_CHILDREN);
    json_t* root;
    json_error_t error;

    // Parse the JSON string
    root = json_loads(json_string, 0, &error);
    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        free(json_string);
        return 1;
    }

    // Initialize an empty JSON object for the flattened data
    json_t* flat_json = json_object();

    // Start measuring time
    clock_t start_time = clock();

    // Flatten the JSON using recursion
    flatten_json_recursion(root, flat_json, "");

    // End measuring time
    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    char* flattened_json_string = json_dumps(flat_json, JSON_INDENT(2));
    if (flattened_json_string) {
        printf("\nFlattened JSON Output:\n%s\n", flattened_json_string);
    }
    printf("Time taken In C: %f seconds\n", duration);

    // Clean up
    free(json_string);
    free(flattened_json_string);
    json_decref(root);
    json_decref(flat_json);
    return 0;
}
