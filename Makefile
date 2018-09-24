GCC = gcc
CFLAGS =
OBJS = pg_iconv.o
INCLUDE = -I/usr/local/Cellar/postgresql/9.6.2/include/server
LIBS = -liconv
OUTPUT = pg_iconv.so
CC = $(GCC) $(INCLUDE) $(CFLAGS)
LD = $(GCC) $(LIBS)

all: clean compile test install
	make clean

clean:
	rm -rf *.o *.so converter

.c.o:
	$(CC) -c $<

compile: iconverter.o log.o
	$(LD) -shared iconverter.o log.o -o libiconverter.so

test: compile main.o
	$(LD) -L. -liconverter main.o -o pg_iconv
	./pg_iconv

install: compile text.o pg_iconv.o
	$(LD) -bundle -flat_namespace -undefined suppress -L. -liconverter text.o pg_iconv.o -o /usr/local/lib/pg_iconv.so
	cp libiconverter.so /usr/local/lib