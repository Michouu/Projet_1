# Makefile racine*
.Phony: clean mrproper


# Makefile racine*
.SUFFIXES:
.Phony: clean mrproper


CC?=gcc
EXEC=Generer
SOURCE= $(wildcard *.c) fonctions.h versions.h
CFLAGS = -W 



all: $(SOURCE)
	@echo "Compilation : "
	$(CC) $(SOURCE)  -o $(EXEC) $(CFLAGS) -DDATE 

	@file $(EXEC)

clean:
	rm -f $(EXEC)

install:
	install -d ${DESTDIR}/usr/bin
	install -m 0755 Generer ${DESTDIR}/usr/bin
