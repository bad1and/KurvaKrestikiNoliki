#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int key;
int width, height,znaki_riad;
int PosX = 1, PosY = 1;
int position = 0;
int n_button;
bool done = false;
int counter_X_O;

// хранение коорд
typedef struct {
    int x, y;
} Point;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};


void instruction (){
    printw("%s", "-------------------------------------------------------\n");
    printw("%s", "Используйте стрелки для перемещения\n");
    printw("%s", "Вы - $\n");
    printw("%s", " - установка стены, DELETE - удалить значение в клетке\n");
    printw("%s", "S - установка старта, E - установка конца\n");
    printw("%s", "P - расчет пути\n\n");

}

void menu() {
    printw("%s", "KrestikiNoliki by Tikhanov Oleg\n");
    printw("%s", "ver 0.1\n");
    printw("%s", "\n");
    printw("%s", "Перед использованием прочитайте инструкцию 2 п.\n");
    printw("%s", "-----------------\n");
    printw("%s", "\n");

    char screen[4][35] = {
        "1. Играть",
        "2. Инструкция",
        "3. Выход",
        "",
    };

    for (int i = 0; i <= 3; ++i) {
        if (i == position) {
            printw("%s  %s\n", screen[i], "<---");
        } else {
            printw("%s\n", screen[i]);
        }

    }
}

//экран и ошибки
void drawmain(int width, int height, char maze[height][width], int PosX, int PosY) {
    clear(); // Очищаем экран
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == PosX && y == PosY) {
                mvaddch(y, x, '$'); //Player
            } else {
                mvaddch(y, x, maze[y][x]); //Lab
            }
        }
    }
    printw("\n");
    if (done == true) {
        printw("Для выхода нажмите ESC");
    }
    if (key == 'x' && counter_X_O %2 != 0) { //начало
        printw("Следующий ход нолика (O).\n");
        // printw("Всего ходов: %i",counter_X_O);
    }
    if (key == 'o' && counter_X_O % 2 == 0) { //конец
        printw("Следующий ход крестика (X).\n");
        // printw("Всего ходов: %i",counter_X_O);
    }
}

// Заполняем начальный массив точками
void createlab(int width, int height, char maze[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze[y][x] = '.';
        }
    }
}

//отрисовка + обрвботка кнопок
void keywork(char maze[height][width], Point* start, Point* end) {
    drawmain(width, height, maze, PosX, PosY);

    key = getch();

    if (key == KEY_UP) {
        if (PosY > 0) PosY--;
    }
    if (key == KEY_DOWN) {
        if (PosY < height - 1) PosY++;
    }
    if (key == KEY_RIGHT) {
        if (PosX < width - 1) PosX++;
    }
    if (key == KEY_LEFT) {
        if (PosX > 0) PosX--;
    }
    if (key == 127) {
        if (maze[PosY][PosX] == 'X') {
            start->x = -1;
            start->y = -1;
            maze[PosY][PosX] = '.';
            counter_X_O--;
        }
        if (maze[PosY][PosX] == 'O') {
            end->x = PosX;
            end->y = PosY;
            maze[PosY][PosX] = '.';
            counter_X_O--;
        }
        maze[PosY][PosX] = '.';
    }

    if (key == 'x' && counter_X_O%2==0) { //начало
        start->x = PosX;
        start->y = PosY;
        maze[PosY][PosX] = 'X';
        counter_X_O++;
    }

    if (key == 'o' && counter_X_O%2!=0) { //конец
        end->x = PosX;
        end->y = PosY;
        maze[PosY][PosX] = 'O';
        counter_X_O++;
    }
    // if (key == 'p') { //решаем
    // }
}


int main() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, FALSE);
    Point start = {-1, -1}, end = {-1, -1}; // за границу заносим
    while (true) {
        menu();

        n_button = getch();

        if (n_button == 27) {
            // ESC
            n_button = getch();
            if (n_button == 91) {
                // enter
                n_button = getch();
                if (n_button == 65) {
                    // up
                    position = position - 1;
                    if (position < 0) {
                        position = 2;
                    }
                } else if (n_button == 66) {
                    // down
                    position = position + 1;
                    if (position > 2) {
                        position = 0;
                    }
                }
            }
        }

        //1 laba
        if ((position == 0 && n_button == 10) || n_button == 27) {
            counter_X_O = 0;
            done = false;
            keypad(stdscr, TRUE);
            clear();
            while (true) {
                printw("Введите размеры игрового поля (от 3 до 15): ");
                scanw("%d %d", &width, &height);
                if (width < 3 || height < 3 || (width < 3 && height < 3) || (width > 15 && height > 15) || width > 15 || height > 15) {
                    clear();
                    printw("Неверные значения!\n");
                }else {
                    break;
                }
            }
            clear();

            while (true) {
                printw("Введите количество знаков в ряду для победы (от 3 до 15): ");
                scanw("%d", &znaki_riad);
                if (znaki_riad < 3 || znaki_riad > 15) {
                    clear();
                    printw("Неверные значения!\n");
                }else {
                    break;
                }
            }
            clear();

            // Создаем лабиринт
            char maze[width][height];

            PosX = 1, PosY = 1;
            createlab(width, height, maze);

            while (1) {
                // Отображаем лаб
                keywork(maze, &start, &end);

                if (key == 27) {
                    keypad(stdscr, FALSE);
                    break;
                }
            }
        }

        // 2 пункт
        if ((position == 1 && n_button == 10)) {
            clear();
            instruction();
            n_button = getch();
            if (n_button != 27) {
                clear();
                menu();
            }
        }

        //exit
        if ((position == 2 && n_button == 10) || n_button == 27) {
            break;
        }

        clear();
        endwin();
    }
    return 0;
}
