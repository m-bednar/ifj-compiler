#
#	Makefile for IFJ compiler
#	Date: 06/10/2020
#

GCC_ARGS = -std=c99 -Wno-implicit
GCC_ARGS_EXT = -std=c99 -Wall -Wextra -Werror 

MAIN_FILENAME = main
SOURCE_DIR = src
BINARY_DIR = bin
LIBS = src/memory.c src/symtable/binitem.c src/symtable/symbol.c src/symtable/symtable.c

# Mkdir command differs in windows/linux
ifeq ($(OS),Windows_NT)
	MKDIR_BIN = if not exist ${BINARY_DIR} mkdir ${BINARY_DIR}
else
	MKDIR_BIN = mkdir -p ${BINARY_DIR}
endif

all: compile run

compile:
	@${MKDIR_BIN}
	@gcc ${GCC_ARGS} ${SOURCE_DIR}/${MAIN_FILENAME}.c ${LIBS} -o ${BINARY_DIR}/${MAIN_FILENAME}

run:
	@./${BINARY_DIR}/${MAIN_FILENAME}
