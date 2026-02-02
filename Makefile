CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = mysh
SRC = mysh.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET)

re: clean all

.PHONY: all clean re

