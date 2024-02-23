CC = gcc
CFLAGS = -Wall -std=c11 -g

all: listio.o liblistio.a a4.o pyprep db.o
	$(CC) $(CFLAGS) a4.o liblistio.a -L./liblistio -lmysqlclient -L/usr/lib/x86_64-linux-gnu/ -o a4 ;\
	$(CC) $(CFLAGS) db.o  -lmysqlclient -L/usr/lib/x86_64-linux-gnu/ -o db

a4: listio.o liblistio.a a4.o pyprep
	$(CC) $(CFLAGS) a4.o liblistio.a -L./liblistio -lmysqlclient -L/usr/lib/x86_64-linux-gnu/ -o a4

a4.o: a4.c listio.o
	$(CC) $(CFLAGS) a4.c -o a4.o -c

db: db.o
	$(CC) $(CFLAGS) db.o -lmysqlclient -L/usr/lib/x86_64-linux-gnu/ -o db

db.o: db.c
	$(CC) $(CFLAGS) db.c -o db.o -c

liblistio.a: listio.o
	ar cr liblistio.a listio.o

listio.o: listio.c listio.h
	$(CC) $(CFLAGS) listio.c -o listio.o -c

pyprep: a4.py
	chmod +x a4.py

clean:
	rm -rf a4 db liblistio.a *.o
