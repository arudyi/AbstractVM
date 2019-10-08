NAME = avm

SRC_DIR = ./src/
OBJ_DIR = ./obj/
INCLUDE_DIR = ./headers/
SRC = main.cpp Factory.cpp Executer.cpp
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
CC_FLAGS = -Wall -Werror -Wextra
CC := g++

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(FRAMEWORK) $(LINK_FLAGS)

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

vpath %.c $(SRC_DIR)