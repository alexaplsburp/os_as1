CC = gcc
CFLAGS = -Wall

all:	word	dir	date

date:	date.c
	$(CC) $(CFLAGS) -o date date.c

word:	word.c
	$(CC) $(CFLAGS) -o word word.c

dir:	dir.c
	$(CC) $(CFLAGS) -o dir dir.c
	
install:
	install -m 755 word /usr/local/bin/word
	install -m 755 dir /usr/local/bin/dir
	install -m 755 date /usr/local/bin/date


clean:	
	rm -f word
	rm -f dir
	rm -f date