#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "status.h"

int init_table(HashTable* hash_table, int num_slots) {
    int res = SUCCESS;

    hash_table->table = calloc(num_slots, sizeof(TableEntry*));

    if(hash_table->table == 0) {
        res = EMEM;
        goto out;
    }

    hash_table->num_slots = num_slots;

out:
    if(res == EMEM) printf("EMEM: Failed to allocate memory for HashTable.table!\n");
    return res;
}

static uint64_t encode(const char* key) {
    uint64_t code = 0;

    for(int i = 0; key[i] != 0; ++i) {
        code <<= 8;
        code += key[i];
    }    

    return code; 
}

static int hash(const char* key, HashTable* hash_table) {
    return (int)(encode(key) % (uint64_t)hash_table->num_slots);
}

int insert_entry(HashTable* hash_table, const char* key, void* value) {
    int res = SUCCESS;

    int table_index = hash(key, hash_table);

    TableEntry* new_entry = calloc(1, sizeof(TableEntry));
    if(new_entry== 0) {
        res = EMEM;
        goto out;
    }
    new_entry->key = strdup(key);
    new_entry->value = value;

    TableEntry* cur_entry = hash_table->table[table_index];
    if(cur_entry != 0) {
        cur_entry->prev = new_entry;
        new_entry->next = cur_entry;
    }

    hash_table->table[table_index] = new_entry;
    ++hash_table->size;

out:
    if(res == EMEM) printf("EMEM: Failed to allocate memory for TableEntry!\n");
    return res;
}

TableEntry* search_for_entry(HashTable* hash_table, const char* key) {
    TableEntry* sought_entry = 0;
    
    int table_index = hash(key, hash_table);

    TableEntry* cur_entry = hash_table->table[table_index];
    
    while(cur_entry != 0 && strcmp(cur_entry->key, key) != 0) {
        cur_entry = cur_entry->next;
    }

    sought_entry = cur_entry;

    return sought_entry;
}

int delete_entry(HashTable* hash_table, TableEntry* entry) {
    int res = SUCCESS;

    if(hash_table == 0 || entry == 0) {
        res = EINVARG;
        goto out; 
    }

    if(entry->prev == 0 && entry->next == 0) {
        int table_index = hash(entry->key, hash_table);
        if(hash_table->table[table_index] == entry) {
            hash_table->table[table_index] = 0;
        }
    } else {
        if(entry->prev != 0) entry->prev->next = entry->next;
        if(entry->next != 0) entry->next->prev = entry->prev;
    } 

    free(entry->key);
    free(entry);
    --hash_table->size;

out:
    if(res == EINVARG) printf("EINVARG: Both the `hash_table` and `entry` arguments must not be null!\n");
    return res;
}

int delete_table(HashTable* hash_table) {
    int res = SUCCESS;

    if(hash_table == 0) {
        res = EINVARG;
        goto out;
    }

    for(int i = 0; i < hash_table->num_slots; ++i) {
        TableEntry* cur_entry = hash_table->table[i];
        while(cur_entry != 0) {
            TableEntry* tmp = cur_entry->next;
            delete(hash_table, cur_entry);
            cur_entry = tmp;
        }
    }

    free(hash_table->table);

out:
    if(res == EINVARG) printf("EINVARG: hash_table argument must nut be null!\n");
    return res;
}
