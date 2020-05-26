##
## EPITECH PROJECT, 2019
## undefined
## File description:
## Makefile
##

OBJ	=	$(SRC:.c=.o)

SRC	=	main.c

NAME	=	103cipher

all :	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(OBJ) -lm

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)
	rm -f *~

re:	fclean all clean

debug:	$(OBJ)
	gcc -o $(NAME) $(OBJ) -Wall -g -lm

