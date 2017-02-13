# Makefile racine*
.SUFFIXES:
.Phony: clean mrproper


CC?=gcc
EXEC=canCheck
SOURCE= $(wildcard *.c) fonction.h versions.h
CFLAGS = -W 



all: $(SOURCE)
	@echo "Compilation : "
	$(CC) $(SOURCE)  -o $(EXEC) $(CFLAGS) -DDATE -DDEBUG

	@file $(EXEC)

clean:
	rm -f $(EXEC)



install:
	install -d ${DESTDIR}/usr/bin
	install -m 0755 Comp ${DESTDIR}/usr/bin
