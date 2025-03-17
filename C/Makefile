-include ./local.mak

TARGET?=app

SRC_DIR= .
INCLUDE_DIRS=include
LIB_DIRS=lib
OBJ_DIR=obj
BIN_DIR=bin
DEP_DIR=dep

DIRS:=$(SRC_DIR) $(INCLUDE_DIRS) $(LIB_DIRS) $(OBJ_DIR) $(BIN_DIR) $(DEP_DIR)

CC=gcc
CCFLAGS=-std=c23 -Wall -Wextra -ggdb

DEPS:=$(SRCS:%.c=$(DEPDIR)/%.d)
DEPFLAGS=-MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

PKGFLAGS=$(shell [ -n "$(PKGS)" ] && pkg-config --cflags $(PKGS))
PKGLIBS=$(shell [ -n "$(PKGS)" ] && pkg-config --libs $(PKGS))

COLOR_RESET=\033[0m
COLOR_GREEN=\033[32m
COLOR_YELLOW=\033[33m
COLOR_BLUE=\033[34m
COLOR_RED=\033[0;31m     

SRCS:=$(wildcard $(SRC_DIR)/*.c)
OBJS:=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

LIBS:=$(patsubst %,-l%,$(patsubst -l%,%,$(patsubst lib%.so,%,$(patsubst lib%.a,%,$(LIBS)))))

.PHONY: all clean clean-clangd compile_commands debug nolibc symbols watch test

target=all

all: $(DIRS) $(BIN_DIR)/$(TARGET)

nolibc: NOLIBC=-nostdlib
nolibc: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo -e "$(COLOR_GREEN)[Linking]$(COLOR_RESET) ./$@ <== $^"
	$(CC) $(OBJS) $(linkflags) $(NOLIBC) -o $@ $(LIB_DIRS:%=-L%) $(LIBS) $(PKGLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(DEP_DIR)
	@echo -e "$(COLOR_YELLOW)[Compiling]$(COLOR_RESET) $<"
	$(CC) $(CCFLAGS) $(compileflags) $(PKGFLAGS) $(DEPFLAGS) $(INCLUDE_DIRS:%=-I%) -c $< -o $@

run: $(target)
	./$(BIN_DIR)/$(TARGET)

preprocess:
	@$(CC) $(CCFLAGS) $(PKGFLAGS) $(DEPFLAGS) $(INCLUDE_DIRS:%=-I%) -E $$([[ -n "$(filename)" ]] && echo "$(filename)" || echo "$(SRCS)")

// TODO: Fix watch
ifeq "$(target)" "nolibc"
watch:
	@echo -e "\n$(COLOR_YELLOW)Impossible to run watch without libc...$(COLOR_RESET)\n"
else
watch:
	@while true; do \
		echo -e "\n$(COLOR_BLUE)------------------------------------------------$(COLOR_RESET)";\
		echo -e "$(COLOR_BLUE)--------------------RELOAD----------------------$(COLOR_RESET)";\
		echo -e "$(COLOR_BLUE)------------------------------------------------$(COLOR_RESET)";\
		make $(target) --quiet &&\
			echo -e "$(COLOR_YELLOW)[Running]$(COLOR_RESET) ./$(BIN_DIR)/$(TARGET)\n";\
			./$(BIN_DIR)/$(TARGET) &&\
			TARGETPID=$!;\
			echo $(TARGETPID);\
		inotifywait -qr -e delete,modify $(SRC_DIR) &> /dev/null;\
		pkill $(TARGET) &>/dev/null;\
	done;\
	pkill $(TARGET)
endif

debug: CCFLAGS += -DDEBUG
debug: all

symbols: $(OBJS)
	@nm $^

compile_commands: clean-clangd
	@echo -e "$(COLOR_BLUE)[Generating]$(COLOR_RESET) compile_commands.json with Bear"
	@bear -- make $(target) \
		&& echo -e "$(COLOR_BLUE)[Generated]$(COLOR_RESET) compile_commands.json" \
		|| (\
			echo -e "\n$(COLOR_RED)[Error]$(COLOR_RESET)" \
			&& rm compile_commands.json \
		)

clean:
	@echo -e "$(COLOR_BLUE)[Cleaning]$(COLOR_RESET)"
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(DEP_DIR)

clean-clangd:
	@echo -e "$(COLOR_BLUE)[Cleaning Clangd Configurations]$(COLOR_RESET)"
	rm -f compile_commands.json

$(SRC_DIR) $(INCLUDE_DIRS) $(LIB_DIRS) $(OBJ_DIR) $(BIN_DIR) $(DEP_DIR):
	@mkdir -p $@

$(DEPFILES):

include $(wildcard $(DEPFILES))
