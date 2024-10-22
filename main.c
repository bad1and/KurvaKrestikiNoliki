#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int key;
int width, height;
int PosX = 1, PosY = 1;
int position = 0;
int n_button;
bool test = false;
bool done = false;

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
    if (test == true) {
        printw("Отсутствует старт/конец или нет решений");
    }
    else {
        printw("\n");
    }
    if (done == true) {
        printw("Для выхода нажмите ESC");
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

void keywork(char maze[height][width], Point* start, Point* end, bool* pathFound) {
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
        }
        if (maze[PosY][PosX] == 'O') {
            end->x = PosX;
            end->y = PosY;
            maze[PosY][PosX] = '.';
        }
        maze[PosY][PosX] = '.';
    }

    if (key == 'x') { //начало
        start->x = PosX;
        start->y = PosY;
        maze[PosY][PosX] = 'X';
    }
    if (key == 'o') { //конец
        end->x = PosX;
        end->y = PosY;
        maze[PosY][PosX] = 'O';
    }
    // if (key == 'p') { //показываем путь
    // }
}

int main() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, FALSE);
    Point start = {-1, -1}, end = {-1, -1}; // за границу заносим
    bool pathFound = false;
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
            int test1 = 0,test2 = 0;
            done = false;
            keypad(stdscr, TRUE);
            clear();
            printw("Введите размеры - ширину и высоту через пробел (если значения маленькие будет выведен кв 2x2): ");
            scanw("%d %d", &width, &height);
            clear();

            if (width <=1){
                width = 2;
                height = 2;
            }
            if (height <=1){
                width = 2;
                height = 2;
            }
            // Создаем лабиринт
            char maze[width][height];

            PosX = 1, PosY = 1;
            createlab(width, height, maze);

            while (1) {
                // Отображаем лаб
                keywork(maze, &start, &end, &pathFound);

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
