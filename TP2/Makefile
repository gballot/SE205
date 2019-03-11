CC = gcc
CFLAGS = -pedantic -Werror -Wall -std=c11 -pthread -O3 -g
PROG = mutex

all : $(PROG)

run : all
	./$(PROG)

clean :
	$(RM) $(PROG) *.o
	$(RM) -r $(PROG).dSYM

.PHONY: all, run, clean
