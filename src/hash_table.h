#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct table_entry {
    const char* key;
    void* value;
    struct table_entry* prev;
    struct table_entry* next;
} TableEntry;

typedef struct {
    TableEntry** table;
    int size;
    int num_slots;
} HashTable;

int init_table(HashTable* hash_table, int num_slots);
int insert_entry(HashTable* hash_table, const char* key, void* value); 
TableEntry* search_for_entry(HashTable* hash_table, const char* key);
int delete_entry(HashTable* hash_table, TableEntry* entry);
int delete_table(HashTable** hash_table_ref);

#endif