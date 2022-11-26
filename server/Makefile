CC=g++
CFLAGS=
DEPS=headers/json.hpp
OBJ=main.o
NAME=json

%.o: source/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	$(CC) $^ -o $@  $(CFLAGS) $(LDPATH)

.PHONY: clean
clean:
	rm -f *.o $(NAME)