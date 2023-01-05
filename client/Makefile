CC=g++
CFLAGS=-Wall -g
LDPATH=-lsfml-graphics -lsfml-window -lsfml-system
DEPS=
OBJ=pieceTextures.o piece.o move.o chessgame.o algorithm.o user.o gameinfo.o promotionComponent.o chessgameHandler.o main.o
NAME=chess-client

$(NAME): $(OBJ)
	$(CC) $^ -o $@  $(CFLAGS) $(LDPATH)

main.o: source/main.cpp
	$(CC) -c source/main.cpp $(CFLAGS)

chessgameHandler.o: source/chessgameHandler.cpp source/chessgameHandler.hpp
	$(CC) -c source/chessgameHandler.cpp

promotionComponent.o: source/promotionComponent.cpp source/promotionComponent.hpp
	$(CC) -c source/promotionComponent.cpp $(CFLAGS)

gameinfo.o: source/gameinfo.cpp source/gameinfo.hpp source/move.hpp source/user.hpp 
	$(CC) -c source/gameinfo.cpp  $(CFLAGS)

user.o: source/user.cpp source/user.hpp
	$(CC) -c source/user.cpp $(CFLAGS)

algorithm.o: source/algorithm.cpp source/algorithm.hpp source/chessgame.hpp
	$(CC) -c source/algorithm.cpp $(CFLAGS)

chessgame.o: source/chessgame.cpp source/chessgame.hpp source/piece.hpp source/move.hpp
	$(CC) -c source/chessgame.cpp $(CFLAGS)

move.o: source/move.cpp source/move.hpp
	$(CC) -c source/move.cpp $(CFLAGS)

piece.o: source/piece.cpp source/piece.hpp
	$(CC) -c source/piece.cpp $(CFLAGS)

pieceTextures.o: source/pieceTextures.cpp source/pieceTextures.hpp
	$(CC) -c source/pieceTextures.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(NAME)