#ifndef NETWORK_H
#define NETWORK_H

#include <sys/epoll.h>

int server_start(const char *address, const char *port);
int epoll_start(int sockfd, struct epoll_event **ev);
int accept_client(int sockfd);
int epoll_add(int epollfd, int events, int fd);
int epoll_del(int epollfd, int fd);
void server_stop(int sockfd);
void epoll_stop(int epollfd, struct epoll_event *ev);

#endif
