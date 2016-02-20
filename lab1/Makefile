NAME=dp_one
SRC=lab1.c
OBJ=$(SRC:.c=.o)
RM = rm -f
INCLPATH = ../../include
LIBPATH = ../../lib
CFLAGS += -I$(INCLPATH)
all: $(OBJ)
	gcc $(OBJ) -Wall -pthread -pedantic-errors -o $(NAME)

clean:
	-$(RM) *~
	-$(RM) *.o
	-$(RM) *.core
	-$(RM) \#*

fclean: clean
	-$(RM) $(LIBPATH)/$(NAME)

re: fclean all
