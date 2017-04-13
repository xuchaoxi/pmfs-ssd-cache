# Makefile

CC = cc -g
RM = rm -f

ifndef NVM_SSD_CACHE_DIR
NVM_SSD_CACHE_DIR = .
endif

CFLAGS += -Wall
CPPFLAGS += -I$(NVM_SSD_CACHE_DIR) -I$(NVM_SSD_CACHE_DIR)/strategy -I$(NVM_SSD_CACHE_DIR)/raid

OBJS = main.o raid_5.o global.o nvm_buf_table.o nvm_cache.o trace2call.o lru.o lrustripe.o fifo.o nvm_stripe_table.o

all: $(OBJS) nvm-ssd-cache
	@echo 'Bulid Successfully nvm-ssd-cache...'

nvm-ssd-cache:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) -o $@
	
main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

raid_5.o: raid/raid_5.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

global.o: global.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

nvm_buf_table.o: nvm_buf_table.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

nvm_cache.o: nvm_cache.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

trace2call.o: trace2call.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

lru.o: strategy/lru.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

fifo.o: strategy/fifo.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

lrustripe.o: strategy/lrustripe.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

nvm_stripe_table.o: nvm_stripe_table.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?


.PHONY:clean
clean:
	$(RM) *.o
	$(RM) $(NVM_SSD_CACHE_DIR)/main

