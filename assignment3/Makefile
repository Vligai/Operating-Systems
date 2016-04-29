NAME=main
SRC=main.cpp
OBJ=$(SRC:.cpp=.o)
RM = rm -f

all: $(OBJ)
	g++ $(OBJ) -o $(NAME)

clean:
	-$(RM) *~
	-$(RM) *.o
	-$(RM) *.core
	-$(RM) \#*

fclean: clean
	-$(RM) $(NAME)

re: fclean all
