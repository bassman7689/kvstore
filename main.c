#include <stdio.h>
#include "logging.h"
#include "kvstore.h"

int main() {
  struct kvstore *kv = kvstore_new("127.0.0.1", "9090");
  if (kvstore_run(kv) < 0) {
    printf("error while running kvstore.\n");
  }

  kvstore_destroy(kv);
}
