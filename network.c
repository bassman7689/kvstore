#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "network.h"

static int set_nonblock(int sockfd) {
  int flags;
  if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0) {
    perror("set_nonblock: fcntl: getfl");
    close(sockfd);
    return -1;
  }

  if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
    perror("set_nonoblock: fcntl: setfl");
    close(sockfd);
    return -1;
  }

  return 0;
}

int server_start(const char *address, const char *port) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *res;
  int ret;
  if ((ret = getaddrinfo(address, port, &hints, &res)) != 0) {
    printf("kvstore_run: getaddrinfo: %s\n", gai_strerror(ret));
    return -1;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd < 0) {
    freeaddrinfo(res);
    perror("kvstore_run: socket");
    return -1;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
    freeaddrinfo(res);
    close(sockfd);
    perror("kvstore_run: setsockopt");
    return -1;
  }

  if (set_nonblock(sockfd) < 0) {
    freeaddrinfo(res);
    close(sockfd);
    return -1;
  }

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    freeaddrinfo(res);    
    close(sockfd);    
    perror("kvstore_run: bind");
    return -1;
  }

  freeaddrinfo(res);
  
  if (listen(sockfd, SOMAXCONN) < 0) {
    close(sockfd);
    perror("kvstore_run: listen");
    return -1;
  }

  return sockfd;
}

int epoll_start(int sockfd, struct epoll_event **ev) {
  int epollfd;
  if ((epollfd = epoll_create1(0)) < 0) {
    perror("epoll_start: epoll_create");
    return -1;
  }

  *ev = malloc(sizeof(struct epoll_event));
  memset(*ev, 0, sizeof(struct epoll_event));
  (*ev)->events = EPOLLIN;
  (*ev)->data.fd = sockfd;

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, *ev) < 0) {
    perror("epoll_start: epoll_ctl");
    return -1;
  }

  return epollfd;
}

int accept_client(int sockfd) {
  struct sockaddr_in in;
  socklen_t sz = sizeof(in);
  int fd = accept(sockfd, (struct sockaddr*)&in, &sz);
  if (fd == -1) {
    perror("accept");
    return fd;
  }

  if(sizeof(in) == sz) {
    printf("connection received on fd %d from %s:%d\n", fd, inet_ntoa(in.sin_addr), (int)ntohs(in.sin_port));
  }

  return fd;
}

int epoll_add(int epollfd, int events, int fd) {
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = events;
  ev.data.fd = fd;
  int rc = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
  if (rc == -1) {
    perror("epoll_add");
  }

  return rc;
}

int epoll_del(int epollfd, int fd) {
  struct epoll_event ev;
  int rc = epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
  if (rc == -1) {
    perror("kvstore_run: epoll_ctl");
  }
  close(fd);

  return rc;
}

void server_stop(int sockfd) {
  close(sockfd);
}

void epoll_stop(int epollfd, struct epoll_event *ev) {
  close(epollfd);
  free(ev);
}
