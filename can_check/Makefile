# Makefile racine*
.SUFFIXES:
.Phony: clean mrproper


CC?=gcc
EXEC=Comp
SOURCE= $(wildcard *.c) fonction.h versions.h
CFLAGS = -W 



compiler: $(SOURCE)
	@echo "Compilation : "
	$(CC) $(SOURCE)  -o $(EXEC) $(CFLAGS) -DDATE -DDEBUG

	@file $(EXEC)

clean:
	rm -f $(EXEC)



install:
	install -d ${DESTDIR}/usr/bin
	install -m Comp ${DESTDIR}/usr/bin
