# Makefile racine*
.Phony: clean mrproper


# Makefile racine*
.SUFFIXES:
.Phony: clean mrproper


CC?=gcc
EXEC=Generer
SOURCE= $(wildcard *.c) 
CFLAGS = -W 



compiler: $(SOURCE)
	@echo "Compilation : "
	$(CC) $(SOURCE)  -o $(EXEC) $(CFLAGS) -DDATE 

	@file $(EXEC)

clean:
	rm -f $(EXEC)