CC=g++
CFLAGS=-Wall -g -I/usr/include/mysql -I/usr/include/mysql++
DEPS=
LDPATH= -lmysqlpp -lmysqlclient -lssl -lcrypto -luuid
OBJ=main.o mysqlconnector.o userDAO.o
NAME=chess-server

$(NAME): $(OBJ)
	$(CC) $^ -o $@  $(CFLAGS) $(LDPATH)

main.o: source/main.cpp source/mysqlconnector.hpp source/json.hpp
	$(CC) -c source/main.cpp $(CFLAGS)

mysqlconnector.o: source/mysqlconnector.cpp source/mysqlconnector.hpp source/userDAO.hpp
	$(CC) -c source/mysqlconnector.cpp $(CFLAGS)

userDAO.o: source/userDAO.cpp source/userDAO.hpp
	$(CC) -c source/userDAO.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(NAME)