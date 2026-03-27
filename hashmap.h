#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct Entry {
    char *key;
    char *value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry **buckets;
    int size;
    int count;
} HashMap;

HashMap* hashmap_create(int size);
void hashmap_set(HashMap *map, const char *key, const char *value);
char* hashmap_get(HashMap *map, const char *key);
void hashmap_delete(HashMap *map, const char *key);

#endif