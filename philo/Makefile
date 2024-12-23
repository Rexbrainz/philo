#*****************************************************************************************#
#                                   PHILOSOPHERS									  #
#*****************************************************************************************#

# Program name
NAME = philo

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Source files
SRC = main.c ft_atol.c ft_atoi.c
# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(NAME)

# Build the program
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Clean object files
clean:
	@rm -f $(OBJ)

# Clean object files and the program binary
fclean: clean
	@rm -f $(NAME)

# Rebuild the project
re: fclean all

# Specify dependencies
.PHONY: all clean fclean re

