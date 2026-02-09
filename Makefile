VERSION = 0.6.1
CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -DVERSION=\"$(VERSION)\"

# Installation paths
PREFIX ?= $(HOME)/.local
INSTALL_BIN = $(PREFIX)/bin

# Build paths
SRCDIR = ./cyfer
BUILDDIR = ./bin
TESTDIR = ./tests

# Source files
COMMON = $(SRCDIR)/cyfer_utils.c $(SRCDIR)/config.c $(SRCDIR)/logging.c
SRC = $(SRCDIR)/cyfer.c $(SRCDIR)/base64.c $(SRCDIR)/bytes.c \
      $(SRCDIR)/help.c $(SRCDIR)/runners.c $(SRCDIR)/commands.c \
	  $(SRCDIR)/detect.c $(COMMON)
OBJ = $(SRC:.c=.o)
TARGET = $(BUILDDIR)/cyfer

.PHONY: all lsp-config test install uninstall clean tag

all: $(TARGET)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

lsp-config:
	@echo "-Wall" > compile_flags.txt
	@echo "-Wextra" >> compile_flags.txt
	@echo "-std=c17" >> compile_flags.txt
	@echo "-DVERSION=\"$(VERSION)\"" >> compile_flags.txt
	@echo "✔ Updated compile_flags.txt"

$(TARGET): $(OBJ) | $(BUILDDIR) lsp-config
	$(CC) $(CFLAGS) -o $@ $^

test: $(TARGET)
	@echo "Launching the test suite..."
	@bash $(TESTDIR)/cyfer_test

install: $(TARGET)
	@mkdir -p $(INSTALL_BIN)
	cp $(TARGET) $(INSTALL_BIN)/

uninstall:
	rm -f $(INSTALL_BIN)/cyfer

clean:
	rm -f $(OBJ) $(TARGET)

tag:
	git tag -a v$(VERSION) -m "Release v$(VERSION)"
	git push origin v$(VERSION)
	@echo "📗 Tagged v$(VERSION)"
