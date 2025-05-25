C_NAME	=	client
C_SRC	=	client.c
C_OBJ	=	$(C_SRC:.c=.o)

S_NAME	=	server
S_SRC	=	server.c
S_OBJ	=	$(S_SRC:.c=.o)

$(C_NAME): $(C_OBJ)
	gcc -o $@ $^

$(S_NAME): $(S_OBJ)
	gcc -o $@ $^

%.o: %.c
	gcc -c $^ -Wall -Wextra -Werror

all: $(C_NAME) $(S_NAME)

clean:
	rm -f $(C_OBJ) $(S_OBJ)

fclean: clean
	rm -f $(C_NAME) $(S_NAME)

re: fclean all

.PHONY: all clean fclean re