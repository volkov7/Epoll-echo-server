NAME = server

FLAGS = -Wall -Wextra -Werror

SRCS = server.c lists.c

all: $(NAME)

$(NAME):
	@tput setaf 7; tput bold; echo "Compiling server..."
	@gcc $(FLAGS) $(SRCS) -o $(NAME)
	@tput setaf 2; tput bold; echo "DONE server!"

clean:
	@rm -rf $(NAME)
	@tput setaf 6; tput bold; echo "DONE Clean!"

re: clean all
.PHONY: all clean re