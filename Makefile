SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
WEB_DIR := web/src/wasm

EXE := $(BIN_DIR)/main
WEB := $(WEB_DIR)/main.html
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

CC     := gcc
CFLAGS := -g -Wall
LDLIBS := -lSDL2 -lSDL2_ttf -lm

EM_CC      := emcc
EM_CFLAGS  := -Wall -g -lm -O3
EM_LDFLAGS := -s USE_SDL=2 -s USE_SDL_TTF=2 -sALLOW_MEMORY_GROWTH --preload-file assets


.PHONY: pc em clean

pc: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

em: $(WEB) 

$(WEB): | $(WEB_DIR)
	$(EM_CC) -o $(WEB) $(SRC) $(EMCFLAGS) $(EM_LDFLAGS)

$(WEB_DIR):
	mkdir -p $@

clean:
	$(RM) -rf $(BIN_DIR)/* $(OBJ_DIR)/* $(WEB_DIR)/*