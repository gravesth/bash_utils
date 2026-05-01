COMP = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra

TARGET = s21_cat # то, что нужно получить - ЦЕЛЬ

SRCS = s21_cat.c # от чего зависим

all: $(TARGET)

$(TARGET): $(SRCS)
	$(COMP) $(FLAGS) $(SRCS) -o $(TARGET) 

clean: 
	rm -f $(TARGET)

rebuild: clean all

test: $(TARGET)
	bash ./bash_test.sh

verter: 
	bash ../../materials/build/run.sh
	
.PHONY: all clean rebuild style test