CC = cc
CFLAGS = -Iinclude
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
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean