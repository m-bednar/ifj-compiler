#
#	Makefile for IFJ compiler
#	Date: 06/10/2020
#

MAIN_FILENAME = main
SOURCE_DIR = src
BINARY_DIR = bin

all: compile run

compile:
   @gcc -std=c99 -Wall -Wextra -Werror ${SOURCE_DIR}\\${MAIN_FILENAME}.c -lm -o ${BINARY_DIR}\\${MAIN_FILENAME}

clean:
   # TODO: Clean bin directory (?)

run:
	@.\\${BINARY_DIR}\\${MAIN_FILENAME}
