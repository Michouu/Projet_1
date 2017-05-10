# Makefile racine*
.SUFFIXES:
.Phony: clean mrproper


CC?=gcc
EXEC=canCheck
SOURCE= $(wildcard *.c) can_check.h versions.h
CFLAGS = -W -lm



all: $(SOURCE)
	@echo "Compilation : "
	$(CC) $(SOURCE)  -o $(EXEC) $(CFLAGS) 

	@file $(EXEC)

clean:
	rm -f $(EXEC)



install:
	install -d ${DESTDIR}/usr/bin
	install -m 0755 canCheck ${DESTDIR}/usr/bin
