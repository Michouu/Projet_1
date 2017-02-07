# Makefile racine*
.SUFFIXES:
.Phony: clean mrproper

#export ARCH:=arm
#export CROSS_COMPILE:=arm-linux-gnueabi-

CC?=gcc

EXEC=Lire
SOURCE= $(wildcard *.c)
CFLAGS = -W

#all: $(EXEC)


compiler: $(SOURCE)
	@echo "Compilation : "
	$(CC) $(SOURCE)  -o $(EXEC) $(CFLAGS) -DDATE

	@file $(EXEC)

clean:
	rm -f $(EXEC)