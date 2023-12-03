CC = cc
CFLAGS = $(shell pkg-config --cflags raylib) -Iinclude
LIBS = $(shell pkg-config --libs raylib) 
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

all: $(BINDIR)/main

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/main: $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

$(OBJDIR):
	@if [ ! -d "$(OBJDIR)" ]; then mkdir -p $(OBJDIR); fi

$(BINDIR):
	@if [ ! -d "$(BINDIR)" ]; then mkdir -p $(BINDIR); fi

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean