#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int key;
int width, height, znaki_riad;
int PosX = 1, PosY = 1;
int position = 0;
int n_button;
bool done = false;
int counter_X_O;
int win_counter;


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
    printw("%s", "Игра начинается с крестика (Х)\n");
    printw("%s", "X - установка крестика, O - установка нолика\n");
    printw("%s", "DELETE - удалить значение в клетке\n");
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

        if (i == 3 && win_counter == 10) {
            printw("X WIN!\n");
        }
        if (i == 3 && win_counter == 11) {
            printw("O WIN!\n");
        }
        if (i == 3 && win_counter == 12) {
            printw("Ничья!\n");
        }
    }
}



bool canWin(int coutForWin, int sizeX, int sizeY, char maze[sizeY][sizeX], char symbol) {
    // Проверка строк
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x <= sizeX - coutForWin; x++) {
            int cnt = 0;
            for (int k = 0; k < coutForWin; k++) {
                if (maze[y][x + k] == symbol || maze[y][x + k] == '.') {
                    cnt++;
                }
            }
            if (cnt == coutForWin) return true;
        }
    }

    // Проверка столбцов
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y <= sizeY - coutForWin; y++) {
            int cnt = 0;
            for (int k = 0; k < coutForWin; k++) {
                if (maze[y + k][x] == symbol || maze[y + k][x] == '.') {
                    cnt++;
                }
            }
            if (cnt == coutForWin) return true;
        }
    }

    // Проверка диагоналей
    for (int y = 0; y <= sizeY - coutForWin; y++) {
        for (int x = 0; x <= sizeX - coutForWin; x++) {
            int cnt1 = 0, cnt2 = 0;
            for (int k = 0; k < coutForWin; k++) {
                if (maze[y + k][x + k] == symbol || maze[y + k][x + k] == '.') cnt1++;
                if (maze[y + k][x + coutForWin - 1 - k] == symbol || maze[y + k][x + coutForWin - 1 - k] == '.') cnt2++;
            }
            if (cnt1 == coutForWin || cnt2 == coutForWin) return true;
        }
    }

    return false;
}

// Проверка победы
bool check_win(int x, int y, char maze[height][width], char symbol) {
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}}; // Вправо, вниз, диагонали
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int count = 1;

        //в одну сторону
        for (int step = 1; step < znaki_riad; step++) {
            int nx = x + step * dx;
            int ny = y + step * dy;
            if (nx >= 0 && ny >= 0 && nx < width && ny < height && maze[ny][nx] == symbol) {
                count++;
            } else {
                break;
            }
        }

        //в обратную сторону
        for (int step = 1; step < znaki_riad; step++) {
            int nx = x - step * dx;
            int ny = y - step * dy;
            if (nx >= 0 && ny >= 0 && nx < width && ny < height && maze[ny][nx] == symbol) {
                count++;
            } else {
                break;
            }
        }

        if (count >= znaki_riad) {
            return true;
        }
    }
    return false;
}

// Проверка ничьи
bool check_draw(char maze[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (maze[y][x] == '.') {
                return false; // Есть пустые клетки
            }
        }
    }
    return true; // Все клетки заняты
}

// экран и ошибки
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
        printw("Для выхода нажмите ESC\n");
    }

    if (key == 'x' && counter_X_O % 2 != 0) {
        printw("Следующий ход нолика (O).\n");
    } else if (key == 'o' && counter_X_O % 2 == 0) {
        printw("Следующий ход крестика (X).\n");
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

// отрисовка + обработка кнопок
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
        maze[PosY][PosX] = '.';
        counter_X_O--;
    }

    if (key == 'x' && counter_X_O % 2 == 0) {
        maze[PosY][PosX] = 'X';
        counter_X_O++;

        if (check_win(PosX, PosY, maze, 'X')) {
            drawmain(width, height, maze, PosX, PosY);
            win_counter = 10; // X win
            done = true;
        } else if ((!canWin(znaki_riad, width, height, maze, 'X') && !canWin(znaki_riad, width, height, maze, 'O')) || check_draw(maze)) {
            drawmain(width, height, maze, PosX, PosY);
            win_counter = 12; // nichya
            done = true;
        }
    }

    if (key == 'o' && counter_X_O % 2 != 0) {
        maze[PosY][PosX] = 'O';
        counter_X_O++;

        if (check_win(PosX, PosY, maze, 'O')) {
            drawmain(width, height, maze, PosX, PosY);
            win_counter = 11; // O win
            done = true;
        } else if ((!canWin(znaki_riad, width, height, maze, 'X') && !canWin(znaki_riad, width, height, maze, 'O')) || check_draw(maze)) {
            drawmain(width, height, maze, PosX, PosY);
            win_counter = 12; // nichya
            done = true;
        }
    }

}


int main() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, FALSE);
    Point start = {-1, -1}, end = {-1, -1}; // за границу заносим

    while (true) {
        menu();
        n_button = getch();

        //buttonЫ
        if (n_button == 27) {
            n_button = getch();
            if (n_button == 91) {
                n_button = getch();
                if (n_button == 65) {
                    position = position - 1;
                    if (position < 0) {
                        position = 2;
                    }
                } else if (n_button == 66) {
                    position = position + 1;
                    if (position > 2) {
                        position = 0;
                    }
                }
            }
        }

       //MAIN_Ы
        if ((position == 0 && n_button == 10) || n_button == 27) {
            win_counter = 0;
            counter_X_O = 0;
            done = false;
            keypad(stdscr, TRUE);
            clear();
            while (true) {
                printw("Введите размеры игрового поля через пробел (от 3 до 15): ");
                scanw("%d %d", &width, &height);
                if (width < 3 || height < 3 || width > 15 || height > 15) {
                    clear();
                    printw("Неверные значения!\n");
                } else {
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
                } else {
                    break;
                }
            }
            clear();

            char maze[width][height];
            createlab(width, height, maze);

            while (1) {
                keywork(maze, &start, &end);

                if (done || key == 27) {
                    keypad(stdscr, FALSE);
                    break;
                }
            }
        }

        //Instruction
        if ((position == 1 && n_button == 10)) {
            clear();
            instruction();
            n_button = getch();
            if (n_button != 27) {
                clear();
                menu();
            }
        }

        if ((position == 2 && n_button == 10) || n_button == 27) {
            break;
        }

        clear();
        endwin();
    }
    return 0;
}