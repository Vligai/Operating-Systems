NAME=main
SRC=assignment1.cpp
OBJ=$(SRC:.cpp=.o)
RM = rm -f
all: $(OBJ)
	g++ $(OBJ) -lpthread -Wall -pedantic-errors -o $(NAME)

clean:
	-$(RM) *~
	-$(RM) *.o
	-$(RM) *.core
	-$(RM) \#*

fclean: clean
	-$(RM) $(LIBPATH)/$(NAME)

re: fclean all

