CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
SRC     = $(wildcard src/*.c)

ifeq ($(OS),Windows_NT)
    # Windows (w64devkit). Pasta da raylib pode ser trocada sem editar o arquivo:
    #   make RAYLIB_DIR=D:/outra/pasta/raylib
    RAYLIB_DIR ?= C:/raylib
    BIN     = resident.exe
    CFLAGS += -I$(RAYLIB_DIR)/include
    LIBS    = -L$(RAYLIB_DIR)/lib -lraylib -lopengl32 -lgdi32 -lwinmm
else
    # Linux (raylib instalada pelo gerenciador de pacotes)
    BIN     = resident
    LIBS    = -lraylib -lm
endif

$(BIN): $(SRC) $(wildcard src/*.h)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LIBS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN) save.bin

.PHONY: run clean
