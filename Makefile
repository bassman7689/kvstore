HASH_TEST_HEADERS=hashtable.h
HASH_TEST_SOURCES=hashtable.c hash_test.c
KVSTORE_HEADERS=logging.h network.h hashtable.h kvstore.h
KVSTORE_SOURCES=logging.c network.c hashtable.c kvstore.c main.c
CFLAGS=-g -DLOG_LEVEL=LOG_LEVEL_DEBUG

all: hash_test kvstore

hash_test: $(HASH_TEST_SOURCES) $(HASH_TEST_HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(HASH_TEST_SOURCES) -o $@

kvstore: $(KVSTORE_SOURCES) $(KVSTORE_HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(KVSTORE_SOURCES) -o $@

.PHONY: clean
clean:
	rm hash_test kvstore
