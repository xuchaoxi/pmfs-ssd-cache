# Makefile

CC = cc -g

CFLAGS += -Wall

OBJS = main.o raid-5.o global.o nvm-buf-table.o

#all: $(OBJS) build
#	@echo 'Bulid Successfully'

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	
main.o: main.c
	$(CC) $(CFLAGS) -c $?

raid-5.o: raid-5.c
	$(CC) $(CFLAGS) -c $?

global.o: global.c
	$(CC) $(CFLAGS) -c $?

nvm-buf-table.o: nvm-buf-table.c
	$(CC) $(CFLAGS) -c $?

.PHONY:clean
clean:
	rm *.o

