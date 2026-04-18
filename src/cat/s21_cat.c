#include <stdio.h> 
// ./main -b "text.txt"

int none_flags(char *str, char *line); 
int number_nonblank(char *str, char *line);

int main(int argc, char *argv[]) {
    char line[4096]; 
    // 0 - имя исполняемого файла
    // 1 - cat или grep 
    // 2 - флаг

    if(argv[1][0] == '-') {
        switch(argv[1][1]) {
        case 'b': 
            if(number_nonblank(argv[2], line)) { 
                return 0; 
            }
            break;
        case 'n':
        case 's':
        case 't':
        } 
    } else {  
        if(none_flags(argv[1], line)) { 
            return 0; 
        } 
    }

}

// Flag -b
int number_nonblank(char *str, char *line) {
    FILE *file = fopen(str, "r"); 
    int count = 0;
    if (file == NULL) {
        printf("%s","n/a");
        return 0; 
    }

    while(fgets(line, sizeof(line), file) != NULL) { 
        // Перезаписываем буффер line каждую итерацию
        if (line[0] == '\n') {
            printf("%s", "\n"); 
        } else {
            count++; 
            printf("     %d  %s", count, line); // выводим на экран
        }
    }
    // line - буффер. 
    // sizeof(line) - читаем не более Н символов
    // файловый входной поток

    fclose(file);
    return 1; 
}

int none_flags(char *str, char *line) { 
    FILE *file = fopen(str, "r"); 
    if (file == NULL) {
        printf("%s","n/a");
        return 0; 
    }

    while(fgets(line, sizeof(line), file) != NULL) { 
        // Перезаписываем буффер line каждую итерацию
        printf("%s", line); // выводим на экран
    }
    // line - буффер. 
    // sizeof(line) - читаем не более Н символов
    // входной поток

    fclose(file);
    return 1;
}