# Makefile

CC = cc -g

ifndef NVM_SSD_CACHE_DIR
NVM_SSD_CACHE_DIR = .
endif

CFLAGS += -Wall
CPPFLAGS += -I$(NVM_SSD_CACHE_DIR) -I$(NVM_SSD_CACHE_DIR)/strategy -I$(NVM_SSD_CACHE_DIR)/raid

OBJS = main.o raid-5.o global.o nvm-buf-table.o nvm-cache.o trace2call.o lru.o

#all: $(OBJS) build
#	@echo 'Bulid Successfully'

main: $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) -o $@
	
main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

raid-5.o: raid/raid-5.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

global.o: global.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

nvm-buf-table.o: nvm-buf-table.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

nvm-cache.o: nvm-cache.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

trace2call.o: trace2call.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

lru.o: strategy/lru.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

.PHONY:clean
clean:
	rm *.o
	rm $(NVM_SSD_CACHE_DIR)/main

