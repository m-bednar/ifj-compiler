#
#	Makefile for IFJ compiler
#	Date: 06/10/2020
#

GCC_ARGS = -std=c99 -Wall -Wno-format -Wextra -Werror

MAIN_FILENAME = main
SOURCE_DIR = src
LIBS := src/memory.c $(wildcard src/**/*.c)

compile:
	@gcc $(GCC_ARGS) $(SOURCE_DIR)/$(MAIN_FILENAME).c $(LIBS) -o $(MAIN_FILENAME) && echo Compiled!
	@echo

run: compile
	@./$(MAIN_FILENAME)

try: compile
	@./$(MAIN_FILENAME) < $(FILE)
	@echo
	@echo EXIT CODE: $$?

gen: compile
	@./$(MAIN_FILENAME) < $(IN) > $(OUT)
	@echo
	@echo EXIT CODE: $$?
