# Makefile racine*
.Phony: clean mrproper


# Makefile racine*
.SUFFIXES:
.Phony: clean mrproper


CC?=gcc
EXEC=canSend
SOURCE= $(wildcard *.c) can_send.h versions.h
CFLAGS = -W 



all: $(SOURCE)
	@echo "Compilation : "
	$(CC) $(SOURCE)  -o $(EXEC) $(CFLAGS) 

	@file $(EXEC)

clean:
	rm -f $(EXEC)

install:
	install -d ${DESTDIR}/usr/bin
	install -m 0755 canSend ${DESTDIR}/usr/bin
