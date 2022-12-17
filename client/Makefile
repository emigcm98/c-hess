CC=g++
CFLAGS=-Wall -g
LDPATH=-lsfml-graphics -lsfml-window -lsfml-system
DEPS=
OBJ=pieza.o jugada.o partida.o user.o gameinfo.o main.o
NAME=chess-client

$(NAME): $(OBJ)
	$(CC) $^ -o $@  $(CFLAGS) $(LDPATH)

main.o: source/main.cpp source/gameinfo.cpp
	$(CC) -c source/main.cpp $(CFLAGS)

gameinfo.o: source/gameinfo.cpp source/gameinfo.cpp source/jugada.hpp source/user.hpp
	$(CC) -c source/gameinfo.cpp $(CFLAGS)

user.o: source/user.cpp source/user.hpp
	$(CC) -c source/user.cpp $(CFLAGS)

partida.o: source/partida.cpp source/partida.hpp source/pieza.hpp source/jugada.hpp
	$(CC) -c source/partida.cpp $(CFLAGS)

jugada.o: source/jugada.cpp source/jugada.hpp
	$(CC) -c source/jugada.cpp $(CFLAGS)

pieza.o: source/pieza.cpp source/pieza.hpp
	$(CC) -c source/pieza.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(NAME)