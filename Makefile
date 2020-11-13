#
#	Makefile for IFJ compiler
#	Date: 06/10/2020
#

GCC_ARGS = -std=c99 -Wall -Wextra -Werror

MAIN_FILENAME = main
SOURCE_DIR = src
BINARY_DIR = bin
LIBS := src/memory.c $(wildcard src/**/*.c)

all: compile run

compile:
	@mkdir -p $(BINARY_DIR)
	@gcc $(GCC_ARGS) $(SOURCE_DIR)/$(MAIN_FILENAME).c $(LIBS) -o $(BINARY_DIR)/$(MAIN_FILENAME) && echo Compiled!
	@echo

run:
	@./$(BINARY_DIR)/$(MAIN_FILENAME)

try: compile
	@./$(BINARY_DIR)/$(MAIN_FILENAME) < $(FILE)
	@echo
	@echo EXIT CODE: $$?
