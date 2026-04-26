#include "s21_cat.h"

// ./main -b "text.txt"

int main(int argc, char* argv[]) {
  char line[1024];
  (void)argc;
  // 0 - имя исполняемого файла
  // 1 - флаг
  // 2 - ad

  // 2 - имя файла
  if (argv[1][0] ==
      '-') {  // обработать отдельно случай без аргументов - вывести ошибку
    switch (argv[1][1]) {
      case 'b':
        if (number_nonblank(argv[2], line)) return 0;
        break;
      case 'E':
        if (e_gnu_flag(argv[2])) return 0;
        break;
      case 'e':
        if (e_posix_flag(argv[2])) return 0;
        break;
      case 'n':
        if (number(argv[2], line)) return 0;
        break;
      case 's':

        // case 't':
    }
  } else {
    if (none_flags(argv[1], line)) return 0;
  }
}

int is_control_sym(int ch) {
  // шестрадцатеричное представление управляющих символов от 0 до 31 и 127
  if (ch == 127) return 1;
  if (ch >= 0x00 && ch <= 0x1F) return 1;
  return 0;
}

void print_control_sym(int ch) {
  if (ch == 0x7F) printf("?");  // Отдельно обработаем DEL
  if (ch == 0x09)
    printf("\t");  // Отдельно отображаем таб, как как он не входит в -v
  // ASCII начинается с 65. Выводим ^ + управляющий символ в ASCII
  else
    printf("^%c", ch + 64);
}

// Flag -b
int number_nonblank(char* str, char* line) {
  FILE* file = fopen(str, "r");
  int count = 0;
  if (file == NULL) {
    printf("%s", "n/a");
    return 0;
  }

  while (fgets(line, 1024, file) != NULL) {
    if (line[0] != '\n' && line[0] != '\0') {
      count++;
      size_t len = strlen(line);
      if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
      }
      printf("%6d\t%s\n", count, line);
    } else if (line[0] == '\n') {
      printf("\n");
    }
  }

  fclose(file);
  return 1;
}

// Реализация флага -E без -v (GNU only).
// Отображаем только $ если \n
int e_gnu_flag(char* str) {
  FILE* file = fopen(str, "r");
  if (file == NULL) {
    printf("%s", "n/a");
    return 0;
  }
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      printf("$\n");
      continue;
    }
    printf("%c", (char)ch);
  }
  fclose(file);
  return 1;
}
// Реализация флага -е c -v (POSIX)
// Отображаем все управляющие(непечатаемые) символы кроме таба
// В конце строки также отображаем $
int e_posix_flag(char* str) {
  FILE* file = fopen(str, "r");
  if (file == NULL) {
    printf("n/a");
    return 0;
  }
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      printf("$\n");
      continue;
    } else if (is_control_sym(ch)) {
      print_control_sym(ch);
      continue;
    }
    printf("%c", (char)ch);
  }
  fclose(file);
  return 1;
}

// Flag -n
int number(char* str, char* line) {
  FILE* file = fopen(str, "r");
  int count = 0;
  if (file == NULL) {
    printf("%s", "n/a");
    fclose(file);
    return 0;
  }
  while (fgets(line, 1024, file) != NULL) {
    count++;
    printf("%6d\t%s", count, line);
  }
  fclose(file);
  return 1;
}

// Flag -s
int squeeze_blank(char* str) {
  FILE* file = fopen(str, "r");
  if (file == NULL) return 1;
  char line[1024];
  int prev_empty = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    int is_empty = (line[0] == '\n');
    if (is_empty && prev_empty) {
      continue;  // пропускаем вторую и более пустую строку
    }
    printf("%s", line);
    prev_empty = is_empty;
  }

  fclose(file);
  return 0;
}

// Flag -t posix with -v
int t_posix_flag(char* str) {
  FILE* file = fopen(str, "r");
  if (file == NULL) {
    printf("n/a");
    return 0;
  }
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\t') {
      printf("^I");  // отображаем табы
      continue;
    } else if (is_control_sym(ch)) {
      print_control_sym(ch);  // + управляющие символы
      continue;
    }
    printf("%c", (char)ch);
  }
  fclose(file);
  return 1;
}

// Flag -T gnu
int t_gnu_flag(char* str) {
  FILE* file = fopen(str, "r");
  if (file == NULL) {
    printf("%s", "n/a");
    return 0;
  }
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\t') {
      printf("^I");  // отображаем табы
      continue;
    }
    printf("%c", (char)ch);
  }
  fclose(file);
  return 1;
}
// None flags
int none_flags(char* str, char* line) {
  FILE* file = fopen(str, "r");
  if (file == NULL) {
    printf("%s", "n/a");
    return 0;
  }

  while (fgets(line, 1024, file) != NULL) {
    printf("%s", line);
  }
  // line - буффер.
  // читаем не более 1024 символов
  // входной поток

  fclose(file);
  return 1;
}
