CC?=            cc
CFLAGS?=        -Wall -O
PREFIX?=        /usr/local

add-host6: add-host6.c
	${CC} ${CFLAGS} -o add-host6 add-host6.c

install: add-host6
	cp add-host6 ${PREFIX}/bin

strip: install
	strip ${PREFIX}/bin/add-host6

clean:
	@rm -f *~ *.core

clean-all: clean
	@rm -f add-host6
