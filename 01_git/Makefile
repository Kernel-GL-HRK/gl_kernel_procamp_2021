NAME = scissors

SRC = scissors.c

CFLAGS = -Wall -Wextra -Werror

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

.PHONY = all clean re

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ)
	gcc $(CFLAGS) $^ -o $(NAME)

$(addprefix $(OBJ_DIR),/%.o): %.c
	gcc $(CFLAGS) $(DDFLAGS) -c $^ -o $@

clean:
	rm -rf $(OBJ_DIR) $(NAME)

re:
	make clean
	make all

$(OBJ_DIR):
	mkdir -p $@