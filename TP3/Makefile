TEACHER=false
CC=gcc
CFLAGS=-g -Wall -DTEACHER=$(TEACHER)
LDFLAGS=-g -DTEACHER=$(TEACHER) -lpthread

PRESOURCES_1=\
main_join.c\
protected_buffer.c\
protected_buffer.h\

PRESOURCES_2=\
main_protected_buffer.c\
protected_buffer.c\
protected_buffer.h\

PRESOURCES_3=\
readers_writers_mutexes.c\
readers_writers_mutexes.h\

PRESOURCES_4=\
readers_writers_mutexes_2.c\
readers_writers_mutexes_2.h\

SOURCES_1 = \
circular_buffer.c\
circular_buffer.h\

SOURCES_2 = \
circular_buffer.c\
circular_buffer.h\

SOURCES_3 = \
main_readers_writers.c

SOURCES_4 = \
main_readers_writers_2.c

OBJECTS_1 = \
circular_buffer.o\
protected_buffer.o\
main_join.o

OBJECTS_2 = \
circular_buffer.o\
protected_buffer.o\
main_protected_buffer.o\

OBJECTS_3 = \
circular_buffer.o\
readers_writers_mutexes.o\
main_readers_writers.o

OBJECTS_4 = \
circular_buffer.o\
readers_writers_mutexes_2.o\
main_readers_writers_2.o

PRESOURCES = \
$(PRESOURCES_1)\
$(PRESOURCES_2)\
$(PRESOURCES_3)\
$(PRESOURCES_4)\

SOURCES = \
$(SOURCES_1)\
$(SOURCES_2)\
$(SOURCES_3)\
$(SOURCES_4)\

OBJECTS = \
$(OBJECTS_1)\
$(OBJECTS_2)\
$(OBJECTS_3)\
$(OBJECTS_4)\

PROGS = \
main_join\
main_protected_buffer\
main_readers_writers\
main_readers_writers_2

%.c: %.p.c
	awk -f presources.awk -v TEACHER=$(TEACHER) $< >$@

%.h: %.p.h
	awk -f presources.awk -v TEACHER=$(TEACHER) $< >$@

.c.o:
	$(CC) -c $(CFLAGS) $<

default : $(PROGS)

clean : 
	$(RM) $(OBJECTS) *~

veryclean: clean
	$(RM) $(PRESOURCES)

main_join : $(PRESOURCES_1) $(OBJECTS_1)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_1) 

main_protected_buffer : $(PRESOURCES_2) $(OBJECTS_2)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_2) 

main_readers_writers : $(PRESOURCES_3) $(OBJECTS_3)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_3) 

main_readers_writers_2 : $(PRESOURCES_4) $(OBJECTS_4)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_4) 

deps: $(SOURCES) $(PRESOURCES)
	$(CC) -M $(SOURCES) $(PRESOURCES) >deps

student:
	@make veryclean
	@make $(PRESOURCES)

teacher:
	@make veryclean
	@make TEACHER=true $(PRESOURCES)

index.html: index.texi
	makeinfo \
	        --no-headers --html --ifinfo --no-split \
		--css-include=style.css $< > $@

error :
	$(error "PREFIX variable not set")

install : veryclean index.html
	@if test -z "$(PREFIX)"; then \
	   make error; \
	fi
	@make student
	-mkdir -p $(PREFIX)
	chmod og=u-w $(PREFIX)
	tar zcf src.tar.gz `ls $(SOURCES) $(PRESOURCES) | uniq` Makefile
	chmod og=u-w style.css index.html src.tar.gz
	cp -p style.css index.html src.tar.gz $(PREFIX)

-include deps
