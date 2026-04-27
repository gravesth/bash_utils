COMP = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra

TARGET = s21_cat # то, что нужно получить - ЦЕЛЬ

SRCS = s21_cat.c # от чего зависим

all: style $(TARGET)	# сначала форматируем потом получаем TARGET 

$(TARGET): $(SRCS)
	$(COMP) $(FLAGS) $(SRCS) -o $(TARGET) 

clean: 
	rm -f $(TARGET)

rebuild: clean all

style:
	clang-format -i *.c *.h 

test: $(TARGET)
	bash ./bash_test.sh

.PHONY: all clean rebuild style test