NAME=main
SRC=main.c
OBJ=$(SRC:.c=.o)
RM = rm -f
all: $(OBJ)
	gcc $(OBJ) -lpthread -Wall -pedantic-errors -o $(NAME)

clean:
	-$(RM) *~
	-$(RM) *.o
	-$(RM) *.core
	-$(RM) \#*

fclean: clean
	-$(RM) $(LIBPATH)/$(NAME)

re: fclean all

