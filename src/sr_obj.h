
#ifndef SR_OBJ_H
#define SR_OBJ_H

struct hash_table_entry*
hash_table_entry_alloc(char* k, char* v);

struct hash_table* 
hash_table_alloc();

void
hash_table_entry_free(struct hash_table_entry* e);

void
hash_table_free(struct hash_table* ht);

struct hash_table_entry {
    char* key;
    char* value;
};

struct hash_table {
    struct ht_entry** entries;
    uint64_t n_entries;
    size_t size;
};

#endif /* SR_OBJ_H */