CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
SRC     = $(wildcard src/*.c)
BIN     = resident

# Linux. No Windows (w64devkit) troque LIBS por: -lraylib -lopengl32 -lgdi32 -lwinmm
LIBS    = -lraylib -lm

$(BIN): $(SRC) $(wildcard src/*.h)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LIBS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN) save.bin

.PHONY: run clean
