#ifndef KVSTORE_H
#define KVSTORE_H

#include <sys/epoll.h>
#include "hashtable.h"

struct kvstore {
  struct hashtable *ht;
  const char *address;
  const char *port;
  struct epoll_event *ev;
  int sockfd;
  int epollfd;
};

struct kvstore *kvstore_new(const char *address, const char *port);
int kvstore_run(struct kvstore *kv);
void kvstore_destroy(struct kvstore *kv);

#endif
