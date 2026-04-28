COMP = gcc
FLAGS = -Wall -Wextra -Werror -std=c11

TARGET = s21_grep

SRCS = s21_grep.c

$(TARGET): $(SRCS)
	$(COMP) $(FLAGS) $(SRCS) -o $(TARGET)

style:
	clang-format -i *.c *.h

all: $(TARGET)

test: $(TARGET)
	bash ./bash_test.sh

rebuild: clean all

.PHONY: all clean rebuild style test