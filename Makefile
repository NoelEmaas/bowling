CC = cc
CFLAGS =  -Wall -Wextra -Iinclude
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
# CFLAGS = $(shell pkg-config --cflags raylib) -Iinclude
# LIBS = $(shell pkg-config --libs raylib)
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES := $(wildcard $(SRCDIR)/*.c)

# Server-specific sources and objects
SERVER_SOURCES := $(filter-out $(SRCDIR)/client.c, $(SOURCES))
SERVER_OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SERVER_SOURCES))

# Client-specific sources and objects
CLIENT_SOURCES := $(filter-out $(SRCDIR)/server.c, $(SOURCES))
CLIENT_OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CLIENT_SOURCES))

all: server client

server: $(BINDIR)/server

client: $(BINDIR)/client

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/server: $(SERVER_OBJECTS) | $(BINDIR)
	$(CC) $(SERVER_OBJECTS) -o $@ $(LIBS)

$(BINDIR)/client: $(CLIENT_OBJECTS) | $(BINDIR)
	$(CC) $(CLIENT_OBJECTS) -o $@ $(LIBS)

$(OBJDIR):
	@if [ ! -d "$(OBJDIR)" ]; then mkdir -p $(OBJDIR); fi

$(BINDIR):
	@if [ ! -d "$(BINDIR)" ]; then mkdir -p $(BINDIR); fi

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean server client
