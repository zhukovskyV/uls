NAME = uls

INC = uls.h

SRC = 	mx_mychmod.c \
		mx_print_spaces.c \
		mx_substr.c \
		vivod.c \
		mx_intlen.c \
	

OBJ = 	mx_mychmod.o \
		mx_print_spaces.o \
		mx_substr.o \
		vivod.o \
		mx_intlen.o \


CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all: install clean

install:
	@make install -sC libmx
	@cp $(addprefix src/, $(SRC)) .
	@cp $(addprefix inc/, $(INC)) .
	@clang $(CFLAGS) -c $(SRC) -I $(INC)
	@clang $(CFLAGS) libmx/libmx.a $(OBJ) -o $(NAME)
	@mkdir -p obj
	@mv $(OBJ) ./obj

uninstall: clean
	@make uninstall -sC libmx
	@rm -rf $(NAME)

clean:
	@make clean -sC libmx
	@rm -rf $(INC) $(SRC) ./obj

reinstall: uninstall install