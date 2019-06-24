#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdbool.h>

#define BASE_TABLE_SIZE 1
#define LOAD_FACTOR 0.75

struct hashtable_element {
  struct hashtable_element *next;
  const char *key;
  void *value;
};

struct hashtable {
  struct hashtable_element **table;
  size_t capacity;
  size_t size;
};

struct hashtable *hashtable_new();
void hashtable_set(struct hashtable *ht, const char *key, void *value);
bool hashtable_key_exists(struct hashtable *ht, const char *key);
void *hashtable_get(struct hashtable *ht, const char *key);
void hashtable_delete(struct hashtable *ht, const char *key);
void hashtable_for_each(struct hashtable *ht, void(*func)(struct hashtable_element *));
void hashtable_destroy(struct hashtable *ht);

#endif
