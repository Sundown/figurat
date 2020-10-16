CFLAGS += -Wall -W -pedantic -Ofast -std=c89
.PHONY: fig
fig:
	$(CC) $(CFLAGS) $(shell find . -name "*.c") -o fig
run: fig
	./fig
