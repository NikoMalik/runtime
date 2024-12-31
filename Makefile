PROGRAM_NAME = runtime
LD_FLAGS = -lpthread
FLAGS = -std=c99 -Wextra  -Wall
files = goroutine.c queue.c scheduler.c main.c context.c
CC ?= gcc
OBJ= $(files:.c=.o)
MAYBE = -Werror

%.o: %.c
	$(CC) $(FLAGS) $(LD_FLAGS) -c $<

build: $(OBJ)
	$(CC) $(FLAGS) $(LD_FLAGS) $(OBJ) -o $(PROGRAM_NAME)


run: build
	./$(PROGRAM_NAME)

clean:
	rm $(OBJ)
	rm $(PROGRAM_NAME)






