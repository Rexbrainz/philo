#*************************************************************************************#
#                                   PHILOSOPHERS									  #
#*************************************************************************************#

# Program name
NAME = philo

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread

# Source files
SRC = main.c	ft_atol.c	ft_atoi.c\
	  make_table.c	 create_philos.c	simulation_utils.c\
	  main_thread.c
# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(NAME)

# Build the program
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

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

