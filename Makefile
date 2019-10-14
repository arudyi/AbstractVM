NAME = avm

SRC_DIR = ./src/
OBJ_DIR = ./obj/
SRC = main.cpp Factory.cpp Executer.cpp
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))
CC_FLAGS = -O2 -Wall -Werror -Wextra -std=c++11
CC := clang++

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all