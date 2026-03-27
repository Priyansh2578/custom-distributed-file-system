#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

// Hash function (djb2)
unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

HashMap* hashmap_create(int size) {
    HashMap *map = malloc(sizeof(HashMap));
    map->size = size;
    map->count = 0;
    map->buckets = calloc(size, sizeof(Entry*));
    return map;
}

void hashmap_set(HashMap *map, const char *key, const char *value) {
    unsigned long index = hash(key) % map->size;
    
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->value = strdup(value);
    new_entry->next = map->buckets[index];
    
    map->buckets[index] = new_entry;
    map->count++;
}

char* hashmap_get(HashMap *map, const char *key) {
    unsigned long index = hash(key) % map->size;
    
    Entry *current = map->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void hashmap_delete(HashMap *map, const char *key) {
    unsigned long index = hash(key) % map->size;
    
    Entry *current = map->buckets[index];
    Entry *prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                map->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            map->count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}