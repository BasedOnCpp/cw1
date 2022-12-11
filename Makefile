COMPILER=gcc
CFLAGS=-c -Wall

all: build clean

build: handler.o logger.o memory.o process.o reader.c text.o main.o
	$(COMPILER) handler.o logger.o memory.o process.o reader.c text.o main.o -o course_work
handler.o: handler.c handler.h process.h logger.h reader.h text.h
	$(COMPILER) $(CFLAGS) handler.c
logger.o: logger.c logger.h text.h
	$(COMPILER) $(CFLAGS) logger.c
memory.o: memory.c memory.h text.h logger.h
	$(COMPILER) $(CFLAGS) memory.c
process.o: process.c process.h text.h logger.h
	$(COMPILER) $(CFLAGS) process.c
reader.o: reader.c reader.h text.h memory.h
	$(COMPILER) $(CFLAGS) reader.c
text.o: text.c memory.h text.h
	$(COMPILER) $(CFLAGS) text.c
main.o: main.c handler.h reader.h logger.h text.h process.h
	$(COMPILER) $(CFLAGS) main.c
clean: build
	rm -rf *.o
