TARGET = fig
SRC = $(shell find ./src/PostProcessor -name "*.c")
SRC+= $(shell find ./src/PreProcessor -name "*.c")
SRC+= src/main.c

PREFIX ?= /usr/local
CFLAGS += -Wall -W -pedantic -Ofast -std=c11 -lm

.PHONY: all install uninstall clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

run: fig
	./fig

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	$(RM) $(TARGET)

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
