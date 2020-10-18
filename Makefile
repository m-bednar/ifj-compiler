#
#	Makefile for IFJ compiler
#	Date: 06/10/2020
#

GCC_ARGS = -std=c99 -Wall -Wextra -Werror 

MAIN_FILENAME = main
SOURCE_DIR = src
BINARY_DIR = bin
LIBS = memory.c symtable/binitem.c symtable/symbol.c symtable/symtable.c

# Mkdir command differs in windows/linux
ifeq ($(OS),Windows_NT)
	MKDIR_BIN = if not exist $(BINARY_DIR) mkdir $(BINARY_DIR)
else
	MKDIR_BIN = mkdir -p $(BINARY_DIR)
endif

all: compile run

compile:
	@$(MKDIR_BIN)
	@gcc $(GCC_ARGS) $(SOURCE_DIR)/$(MAIN_FILENAME).c $(foreach LIB,$(LIBS),$(SOURCE_DIR)/$(LIB)) -o $(BINARY_DIR)/$(MAIN_FILENAME)

run:
	@./$(BINARY_DIR)/$(MAIN_FILENAME)
