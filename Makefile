CC = cc
CFLAGS = -Wall -Wextra -Iinclude 
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
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
