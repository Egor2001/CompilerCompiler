SHELL= /bin/bash
CC= g++
CFLAGS= -std=c++17 -Ofast -Wall -ggdb 

LD= $(CC) 
LFLAGS=

SRCFILE= test.cpp

BINDIR= ./bin
EXECUTABLE= $(BINDIR)/test
OBJFILE= $(BINDIR)/test.o 

TAGS= ctags -R

.PHONY: tags clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJFILE)
	$(LD) $(LFLAGS) $(OBJFILE) -o $(EXECUTABLE)

$(OBJFILE): $(SRCFILE)
	$(CC) $(CFLAGS) -c $(SRCFILE) -o $(OBJFILE)

clean:
	rm $(BINDIR)/*.o 

tags:
	$(TAGS)
