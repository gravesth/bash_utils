#include "s21_cat.h" 

// ./main -b "text.txt"

int main(int argc, char *argv[]) {
    char line[1024]; 
    (void)argc; 
    // 0 - имя исполняемого файла
    // 1 - cat или grep 
    // 2 - флаг
    
    if(argv[1][0] == '-') {
        switch(argv[1][1]) {
        case 'b': 
            if (number_nonblank(argv[2], line)) { 
                return 0; 
            }
            break;
        case 'E': 
            if (e_gnu_flag(argv[2], line)) { 
                return 0; 
            }
            break; 
        case 'e': 
            if (e_posix_flag(argv[2], line)) { 
                return 0; 
            }
            break; 
        // case 'n':
        // case 's':
        // case 't':
        } 
    } else {  
        if (none_flags(argv[1], line)) { 
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

    while(fgets(line, 1024, file) != NULL) { 
        if (line[0] != '\n' && line[0] != '\0') {
            count++; 
            size_t len = strlen(line);
            if (len > 0 && line[len-1] == '\n') {
                line[len-1] = '\0';
            }
            printf("%6d  %s\n", count, line); 
        } else if (line[0] == '\n') {
            printf("\n");
        }
    }

    fclose(file);
    return 1; 
} 
int none_flags(char *str, char *line) { 
    FILE *file = fopen(str, "r"); 
    if (file == NULL) {
        printf("%s","n/a");
        return 0; 
    }

    while(fgets(line, 1024, file) != NULL) { 
        printf("%s", line); 
    }
    // line - буффер. 
    // читаем не более 1024 символов
    // входной поток

    fclose(file);
    return 1;
}

// Реализация флага -E без -v (GNU only)
// Отображаем только $ в конце
int e_gnu_flag (char *str, char *line) { 
    FILE *file = fopen(str, "r"); 
    if(file == NULL) {
        printf("%s", "n/a");
        return 0;
    }
    while(fgets(line, 1024, file)) { 
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        printf("%s$\n", line);
    }
    fclose(file); 
    return 1; 
}

// Реализация флага -е без -v (POSIX)
// Отображаем все управляющие символы
int e_posix_flag(char *str, char *line) {
    FILE *file = fopen(str, "r");
    if (file == NULL) {
        printf("n/a");
        return 0;
    }
    while (fgets(line, 1024, file)) {
        for (int i = 0; line[i] != '\0'; i++) {
            char ch = line[i]; 
            if (ch == '\n') {
                printf("$\n");
            } else if (ch == '\r') {
                printf("^M");
            } else {
                putchar(ch);
            }
        }
    }
    fclose(file);
    return 1;
}

