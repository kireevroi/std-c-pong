/*Copyright 2022 Kireev R., Morozov V., Agapova O. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int get_key();
void draw_temp(int posX, int PosY);
void winner(int x, int y, int winner);
void start(int x, int y);
int pos(int old_pos, int old_angle);
int collision(int pos, int angle, int edge);
int pCollision(int posBX, int posBY, int posP, int edge, int angle);
int field(int x, int y, int s1, int s2, int PX, int PY, int P1, int P2);
int pad(int pos, int edge, int Key);

// Запуск программы
int main(void) {
    start(81, 25);
    return 0;
}
// Считывание клавиши.
int get_key() {
    char key;
    scanf("%c", &key);
    if (key == 'z') {
        return 1;
    } else if (key == 'a') {
        return -1;
    } else if (key == 'm') {
        return 10;
    } else if (key == 'k') {
        return -10;
    } else if (key == ' ') {
        return 0;
    } else if (key == '0') {
       return 999;
    }
    return -999;
}
// Запуск самого Понга
void start(int x, int y) {
    int status = -999;

    // Задаем начальные очки
    int score1 = 0;
    int score2 = 0;

    // Задаем исходное положение ракеток
    int Pos1 = (y-1)/2;
    int Pos2 = (y-1)/2;

    // Задаем начальный рандом
    srand(time(NULL));
    // Задаем положение шарика
    int PosX = (x-1)/2;
    int PosY = (y-1)/2;
// Задаем начальные направления движения шарика
    int angleX = (rand()%2 == 0) ? 1 : -1;
    int angleY = (rand()%2 == 0) ? 1 : -1;

    // Задаем цикл с прерыванием при status = 999
    while (status != 999) {
        // отрисовка поля
        field(x, y, score1, score2, PosX, PosY, Pos1, Pos2);
        // считывание клавиши
        status = get_key();
        // проверка на ввод правильных символов
        if (status != -999) {
            // изменение положения ракетки в зависимости от статуса
            if (status == 1 || status == -1) {
                Pos1 = pad(Pos1, y, status);
            }
            if (status == 10 || status == -10) {
                Pos2 = pad(Pos2, y, status/10);
            }
            // проверка на коллизии с правым и левым краем/ракеткой
            angleX = pCollision(PosX, PosY, Pos1, x, angleX);
            angleX = pCollision(PosX, PosY, Pos2, x, angleX);
            // проверка на коллизии с верхом и низом
            angleY = collision(PosY, angleY, y);
            // если пробивает левый или правый край то
            if (angleX != 1 && angleX != -1) {
                // задаем опять начальное положение
                PosX = (x-1)/2;
                PosY = (y-1)/2;
                // добавляем очки в зависимости от того где пробил
                if (angleX == -2) {
                    score1+=1;
                } else if (angleX == 2) {
                    score2+=1;
                }
                // задаем случайное направление движения
                angleX = (rand()%2 == 0) ? 1 : -1;
                angleY = (rand()%2 == 0) ? 1 : -1;
                // проверяем на победу по очкам и выводим экран
                if (score1 == 21) {
                    winner(x, y, 1);
                    break;
                }
                if (score2 == 21) {
                    winner(x, y, 2);
                    break;
                }
            }
            // меняем положение шарика
            PosX = pos(PosX, angleX);
            PosY = pos(PosY, angleY);
        } else if (status == 0) {
        }
    }
}
// определяем положение шарика по направлению и старому положению
int pos(int old_pos, int old_angle) {
    if (old_angle == 1) {
        old_pos = old_pos + 1;
    } else if (old_angle == -1) {
        old_pos = old_pos - 1;
    }
    return old_pos;
}
// Проверка на касание с попадание в левый/правый край
// если попадает, проверяем наличие рядом ракетки
// если ракетка есть меняем направление, если нет - ставим угол такой
// что бы проверка показала получение очка
int pCollision(int posBX, int posBY, int posP, int edge, int angle) {
    if (posBX < 4 || posBX > edge - 3) {
        if (posBY > posP - 2 && posBY < posP + 2) {
            return angle*-1;
        }
        if (posBX < 2) {
            return 2;
        }
        if (posBX > edge - 2) {
            return -2;
        }
    }
    return angle;
}
// проверка на столкновение с верхом/низом
// если есть, меняем угол
int collision(int pos, int angle, int edge) {
    if (pos == 2) {
        angle*=-1;
    }
    if (pos == (edge - 1)) {
        angle*=-1;
    }
    return angle;
}
// Красиво выводим номер победителя
void winner(int x, int y, int winner) {
    for (int i = 1; i <= y; i++) {
        for (int j = 1; j <= x; j++) {
            if (i == 1+(y-1)/2) {
                if (j == ((x-1)/2)) {
                    printf("Player %d WON", winner);
                } else if (j < ((x-1)/2)-6 || j > ((x-1)/2)+5) {
                    printf("x");
                }
            } else {
                printf("_");
            }
        }
        printf("\n");
    }
}
// выводим поле
int field(int x, int y, int s1, int s2, int PX, int PY, int P1, int P2) {
    for (int i = 1; i <= y; i ++) {
        for (int j = 1; j <= x; j ++) {
            if (i == 1 || i == y) {
                printf("-");
            } else if (j == 1 || j == x || j == (x + 1) / 2) {
                printf("|");

            /* следующий участок работает только с одинарными цифрами
            потом съезжает */
            } else if (i == 5 && j == (x+1)/2-4) {
                printf("%2d", s1);
                j = j+1;
            } else if (i == 5 && j == (x+1)/2+3) {
                printf("%2d", s2);
                j = j+1;
            // - - - - - - - - - - - - - - - - - - - - - - - - - -

            // позиция шарика
            } else if (i == PY && j == PX) {
                 printf("@");
            // - - - - - - - - - - - - - - - - - - - - - - - - - -

            // позиция ракеток
            } else if ((j == 2 && (i == P1 || i == P1 - 1 || i == P1 + 1)) \
            || (j == x - 1 && (i == P2 || i == P2 - 1 || i == P2 + 1))) {
                printf("#");
            // - - - - - - - - - - - - - - - - - - - - - - - - - -

            } else {
                printf(" ");
                }
        }
        printf("\n");
    }
    return 0;
}
// двигаем ракетку с учетом краёв
int pad(int pos, int edge, int Key) {
    if (Key == 1 && pos < edge -2) {
        pos += 1;
    } else if (Key == -1 && pos > 3) {
        pos -= 1;
    } else {
    }
    return pos;
}
