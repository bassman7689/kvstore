#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

struct hashtable *hashtable_new() {
  struct hashtable *ret = malloc(sizeof(struct hashtable));
  ret->capacity = BASE_TABLE_SIZE;
  ret->table = calloc(ret->capacity, sizeof(struct hashtable_element *));
  ret->size = 0;

  return ret;
}

// djb2 hash function. Created by d.j. bernstein
static unsigned long hash(struct hashtable *ht, const char *key) {
  unsigned long hash = 5381;
  int c;

  while ((c = *key++) != '\0') {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % ht->capacity;
}

static struct hashtable_element *hashtable_get_element(struct hashtable *ht, const char *key)
{
  unsigned long key_hash = hash(ht, key);
  struct hashtable_element *hte = ht->table[key_hash];
  for (; hte; hte = hte->next) {
    if (strncmp(hte->key, key, strlen(hte->key)) == 0) {
      return hte;
    }
  }
  return NULL;
}

static void hashtable_grow(struct hashtable *ht) {
  struct hashtable_element **cur_table = ht->table;
  size_t cur_capacity = ht->capacity;
  ht->capacity *= 2;
  ht->size = 0;
  ht->table = calloc(ht->capacity, sizeof(struct hashtable_element *));
  for (size_t i = 0; i < cur_capacity; i++) {
    struct hashtable_element *hte = cur_table[i];
    while (hte) {
      struct hashtable_element *tmp = hte;
      hashtable_set(ht, hte->key, hte->value);
      hte = hte->next;
      free(tmp);
    }
  }
  
  free(cur_table);
}

void hashtable_set(struct hashtable *ht, const char *key,  void *value) {
  unsigned long key_hash = hash(ht, key);
  if ((((double)ht->size) / ((double)ht->capacity)) > LOAD_FACTOR) {
    hashtable_grow(ht);
  }
  
  struct hashtable_element *hte = hashtable_get_element(ht, key);
  if (hte == NULL) {
    hte = malloc(sizeof(struct hashtable_element));
    hte->next = NULL;

    if (ht->table[key_hash] != NULL) {
      hte->next = ht->table[key_hash];
    }
    ht->table[key_hash] = hte;
  }
  
  hte->key = key;
  hte->value = value;
  ht->size++;
}

bool hashtable_key_exists(struct hashtable *ht, const char* key) {
  unsigned long key_hash = hash(ht, key);
  struct hashtable_element *hte = ht->table[key_hash];
  for (;hte; hte = hte->next) {
    if (strncmp(hte->key, key, strlen(hte->key)) == 0) {
      return true;
    }
  }
  return false;
}

void *hashtable_get(struct hashtable *ht, const char *key) {
  struct hashtable_element * hte = hashtable_get_element(ht, key);
  if (hte != NULL) {
    return hte->value;
  }

  return NULL;
}

void hashtable_delete(struct hashtable *ht, const char *key) {
  unsigned long key_hash = hash(ht, key);
  struct hashtable_element *hte_prev, *hte;
  hte = ht->table[key_hash];
  hte_prev = NULL;

  if (hte == NULL) {
    return;
  }
  
  for (; hte; hte_prev = hte, hte = hte->next) {
    if (strncmp(hte->key, key, strlen(hte->key)) == 0) {
      if (hte_prev == NULL) {
	ht->table[key_hash] = hte->next;
      } else {
	hte_prev->next = hte->next;
	return;
      }
    }
  }

  free(hte);
}

void hashtable_for_each(struct hashtable *ht, void(*func)(struct hashtable_element *)) {
  for (size_t i = 0; i < ht->capacity; i++) {
    if (ht->table[i] == NULL) {
      continue;
    }

    struct hashtable_element *hte = ht->table[i];
    for (; hte; hte = hte->next) {
      func(hte);
    }
  }
}

void hashtable_destroy(struct hashtable *ht) {
  for (size_t i = 0; i < ht->capacity; i++) {
    struct hashtable_element *hte = ht->table[i];
    while (hte) {
      struct hashtable_element *tmp = hte;
      hte = hte->next;
      free(tmp);
    }
  }
  free(ht->table);
  free(ht);
}
