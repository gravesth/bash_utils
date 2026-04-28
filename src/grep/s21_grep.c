#include "s21_grep.h"
#include <stdio.h>
// ./s21_grep -e 'M' test.txt
//
int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 0;
  }

  char *file_name = NULL;
  if (argv[1][0] == '-') {
    if (argc < 3) {
      return 0;
    }
    file_name = argv[3];
    char *pattern = argv[2];

    if (argv[1][0] == '-') {
      switch (argv[1][1]) {
      case 'e':
        if (e_flag(pattern, file_name))
          return 0;
        break;
      }
    }
  }
  return 0;
}

// // длина строки
// int my_strlen(const char *str) {
//   int i = 0;
//   while (str[i])
//     i++;
//   return i;
// }

//Поиск паттерна в строке 
// ВОзвращает булево значение
int find_str(const char *line, const char *pattern) {
  int strlen = 0;
  int patternlen = 0;
  // i - пробег по строке
  // j - пробег по паттерну
  // ищем длину строки
  while (line[strlen] != '\0')
    strlen++;
  // ищем длину паттерна
  while (pattern[patternlen] != '\0')
    patternlen++;
  int j;
  for (int i = 0; i <= strlen - patternlen; i++) {
    for (j = 0; j < patternlen; j++) {
      if (line[i + j] != pattern[j])
        break; // выходим из верхнего цикла и перемещаем курсор строки на
               // следующую букву
    }
    if (j == patternlen)
      return 1; // если цикл не прервался возращаем True
  }
  return 0; // Не нашли совпадение паттерна для строки
}
// // функция для сравнения строк
// int streq(char *a, char *b) {
//   int i = 0;
//   while (a[i] && b[i]) {
//     if (a[i] != b[i])
//       return 0;
//     i++;
//   }
//   return a[i] == b[i];
// }

int e_flag(char *pattern, char *filename) {

  //работа с файлом
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("n/a");
    return 0;
  }
  // Обработка строк
  char line[1024]; // создаем буфер для хранения строк
  while (fgets(line, sizeof(line), file) != NULL) {
		if(find_str(line, pattern)){
			printf("%s", line); 
			// printf("\n"); 
		}
  }
	printf("\n");
	fclose(file); 
  return 1;
}

// int i_flag(char *pattern, char *filename) { return 0; }

// int v_flag() { return 0; }

// int c_flag() { return 0; }

// int I_flag() { return 0; }

// int n_flag() { return 0; }

// int h_flag() {return 0;}

// int s_flag() {return 0;}
