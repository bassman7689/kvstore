#include <stdio.h>
#include "hashtable.h"

void print_hashtable_element(struct hashtable_element *hte) {
  printf("key: %s\nvalue: %s\n", hte->key, (char *)hte->value);
}

int main() {
  struct hashtable *ht = hashtable_new();
  hashtable_set(ht, "foo", (void *)"bar");
  hashtable_set(ht, "bar", (void *)"baz");
  hashtable_set(ht, "baz", (void *)"bux");

  printf("hashtable before deleting key baz:\n");
  hashtable_for_each(ht, print_hashtable_element);
  hashtable_delete(ht, "baz");
  printf("hashtable after deleting key baz:\n");  
  hashtable_for_each(ht, print_hashtable_element);
  hashtable_destroy(ht);
 }
