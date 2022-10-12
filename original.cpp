#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void survey_input(void);
int survey_question_read(char question[][100], char space[]);
void gotoxy(int x, int y);

int main(void)
{
    survey_input();
    return 0;
}

void survey_input(void)
{
    //[함수 10.5.1]의 정의 부분 참고
    int i, answer[20], count;
    char yn, question[10][100], space[10];
    FILE* fpw;
    count = survey_question_read(question, space);
    fpw = fopen("설문결과.txt", "a");
    do {
        system("cls");
        printf("각 설문에 대해 문항의 답을 입력하고Enter하시오.\n");
        printf("문항의 답은 숫자만 사용합니다.\n\n");
        for (i = 0; i < count; i++)
            printf("%d) %s\n", i + 1, question[i]);
        for (i = 0; i < count; i++) {
            fflush(stdin);
            gotoxy(space[i] + 4, i * 2 + 4);
            scanf("%d", &answer[i]);
            fprintf(fpw, "%d ", answer[i]);
        }
        fprintf(fpw, "\n");
        printf("\n");
        printf("설문조사가 끝났습니다. 계속은 1, 종료는 0을 입력 ");
        yn = getch();
    } while (yn != '0');
    fclose(fpw);
}
int survey_question_read(char question[][100], char space[])
{
    //[함수 10.5.2]의 정의 부분 참고
    char chr[100];
    FILE* fpr;
    int count = 0;
    fpr = fopen("설문내용.txt", "r");
    while (!feof(fpr)) {
        fgets(chr, 90, fpr);
        space[count] = strlen(chr);
        strcpy(question[count], chr);
        count++;
    }
    fclose(fpr);
    return count; //설문의 개수를 반환
}
void gotoxy(int x, int y)
{
    COORD Pos = { x - 1, y - 1 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
