#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <iso646.h>

#define MAX_TIME 300 // максимальное количество времени 
#define MAX_ERRORS 50 // максимальное количество ошибок

// функция help
void print_help() {
    printf("Usage: typing-test <difficulty> <time> <errors>\n");
    printf("Difficulty: easy, medium, hard\n");
    printf("Time: maximum time allowed to type %d(seconds)\n",MAX_TIME);
    printf("Errors: maximum allowed errors %d\n",MAX_ERRORS);
}

// функция статистики
void print_statistics(clock_t start_time, int correct, int incorrect, int total, int words) {
    clock_t end_time = clock();
    double time_elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nCorrect: %d\n", correct);
    printf("Incorrect: %d\n", incorrect);
    printf("Total: %d\n", total);
    printf("Avg. CPS: %.2f\n", total / time_elapsed);
    printf("Avg. WPM: %.2f\n", words / time_elapsed);
}


// функция перемешение курсора
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}


// главная функция 
int main(int argc, char *argv[]) {

    char *difficulty = argv[1]; // сложность
    int time_limit = atoi(argv[2]); //ограничение по времени 
    int max_errors = atoi(argv[3]); // ограничение по ошибкам 

    // командный аргумент help
    if (strcmp(difficulty, "help") == 0) {
        print_help();
        printf('\n');
        printf("input example: .\\main.exe easy 250 10");
        exit(1);
    }


    // проверяем количество входных аргументов
    if (argc != 4) {
        printf("Error: incorrect number of arguments\n");
        print_help();
        return 1;
    }
   

    // проверка ввода сложности  
    if (strcmp(difficulty, "easy") != 0 && strcmp(difficulty, "medium") != 0 && strcmp(difficulty, "hard") != 0) {
        printf("Error: incorrect difficulty level\n");
        print_help();
        return 1;
    }

    // проверка ввода времени 
    if (time_limit <= 0 || time_limit > MAX_TIME) {
        printf("Error: time limit should be between 1 and %d seconds\n", MAX_TIME);
        print_help();
        return 1;
    }

    // проверка ввода количества ошибок
    if (max_errors <= 0 || max_errors > MAX_ERRORS) {
        printf("Error: maximum number of errors should be between 1 and %d\n", MAX_ERRORS);
        print_help();
        return 1;
    }


    // открываем текст по уровни сложности
    char *filename;
    if (strcmp(difficulty, "easy") == 0) {
        filename = "easy.txt";
    } else if (strcmp(difficulty, "medium") == 0) {
        filename = "medium.txt";
    } else {
        filename = "hard.txt";
    }


    // читаем текст из выбранного файла
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename); // сообщение об ошибке
        return 1;
    }
    char buffer[1000]; // буфер
    int i =0; 
     while ((buffer[i] = fgetc(file)) != EOF) {
        i++;
    }
    buffer[i] = '\0';
    fclose(file);
    system("cls"); // очищаем консоль
    puts(buffer);
    gotoxy(0,0); // переводим курсор на начало текста
    char ch1,ch2;
    i=0;
    int incorrect = 0; // счётчи ошибок
    int correct = 0; // счётчик правильного вввода
    int words = 0, total = 0; //  в общем 
    time_t start_time =clock(); // засекаем время

    // начало игры 
    while((ch1 = buffer[i++]) != '\0'){
        ch2 = getch();
        if (ch1 == ' '){
            words++;
        }
        if (ch1 == ch2){
            printf("\033[32m%c\033[0m", ch1); // зелёный цвет 
            correct++; // увеличиваем счётчик правильного ввода
        }
        else{
             printf("\033[31m%c\033[0m", ch1); // в противном красный цвет
            incorrect++; // уеличиваем счетчик ошибок 
        }
        total++;

        // если число ошибок == мак_число_ошибок
        if (incorrect ==  max_errors){
            gotoxy(0,4);
            printf("\nToo many errors!\n");
            print_statistics(start_time, correct, incorrect, total, words);
            exit(1);
        }

        // жаль время вышло
        if ((double)(clock() - start_time) / CLOCKS_PER_SEC > time_limit) {
            gotoxy(0,4);
            printf("\nTime's up!\n");
            print_statistics(start_time, correct, incorrect, total, words);
            exit(1);
    }
    }
    // С ПОБЕДОЙ!
     printf("\nCongratulations! You typed the text correctly!\n");
    exit(0);
}
