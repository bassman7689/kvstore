#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include "kvstore.h"
#include "network.h"
#include "logging.h"

#define DEFAULT_READ_BUFFER_LEN 1024*16

struct kvstore *kvstore_new(const char *address, const char *port) {
  struct kvstore *kv = malloc(sizeof(struct kvstore));
  kv->address = address;
  kv->port = port;
  kv->ht = hashtable_new();
  return kv;
}

int kvstore_run(struct kvstore *kv) {
  log_debug("server starting on: %s:%s\n", kv->address, kv->port);
  if ((kv->sockfd = server_start(kv->address, kv->port)) < 0) {
    return -1;
  }

  log_debug("starting epoll\n");
  if ((kv->epollfd = epoll_start(kv->sockfd, &kv->ev)) < 0) {
    return -1;
  }
  
  while (1) {
    struct epoll_event events[25];
    int nfds = epoll_wait(kv->epollfd, events, 25, 500);

    if (nfds < 0) {
      printf("error in epoll_waiti!\n");
      break;
    }

    for (int i = 0; i < nfds; i++) {
      int fd = events[i].data.fd;
      if (fd == kv->sockfd) {
	int clientfd = accept_client(kv->sockfd);
	if (clientfd == -1) {
	  continue;
	}

	if(epoll_add(kv->epollfd, EPOLLIN, clientfd) == -1) {
	  close(clientfd);
	}
      } else {
	char buf[DEFAULT_READ_BUFFER_LEN];

	int rc = read(fd, buf, sizeof(buf));
	if (rc == 0) {
	  epoll_del(kv->epollfd, fd);
	  printf("fd %d disconnected.\n", fd);
	  break;
	} else if (rc == -1) {
	  perror("kvstore_run: read");
	  break;
	}

	printf("received %d bytes\n", rc);
      }
    }
  }

  server_stop(kv->sockfd);
  epoll_stop(kv->epollfd, kv->ev);

  return 0;
}

void kvstore_destroy(struct kvstore *kv) {
  hashtable_destroy(kv->ht);
  free(kv);
}
