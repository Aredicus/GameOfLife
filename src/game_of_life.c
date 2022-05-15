#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void game(int** First, int** Next, int** Prev);
void start(int** First);
void check_in(int** st, int** fi);
void clone(int** ot, int** ku);
int check(int i, int j, int** A);
void cell_is(int sum, int** A, int i, int j, int** fi);
void hands_in(int** First);
void auto_in(int** First);
void show(int** First);
void keyListen(float* time);
int is_Finish(int** Next, int** Prev);
void file1(int** First);
void file2(int** First);
void file3(int** First);
void file4(int** First);
void file5(int** First);
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define reset "\e[0m"
#define W 80
#define L 25

int main() {
    int** First = calloc(W, sizeof(int*)), ** Next = calloc(W, sizeof(int*));
    int** Prev = calloc(W, sizeof(int*));
    for (int i = 0; i < W; i++) {
        First[i] = calloc(L, sizeof(int));
        Next[i] = calloc(L, sizeof(int));
        Prev[i] = calloc(L, sizeof(int));
    }
    Prev[0][0] = 1;
    game(First, Next, Prev);
    return 0;
}

void game(int** First, int** Next, int** Prev) {
    FILE* f;
    start(First);
    f = freopen("/dev/tty", "r", stdin);
    system("stty cbreak");
    float time = 1;
    int out = 1;
    while (out) {
        keyListen(&time);
        show(First);
        if (time == 0) {
            out = 0;
        }
        usleep(time * 1000000);
        check_in(First, Next);
        if (is_Finish(Next, Prev)) {
            out = 0;
        }
        clone(First, Prev);
        clone(Next, First);
    }
    show(First);
    printf("\nGame Over!");
    for (int i = 0; i < W; i++) {
        free(First[i]);
        free(Next[i]);
        free(Prev[i]);
    }
    fclose(f);
    free(First);
    free(Next);
    free(Prev);
}

int is_Finish(int** Next, int** Prev) {
    int an = 0;
    int sum = 0;
    int out = 0;
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            if (Next[i][j] == 1) sum++;
            if (Prev[i][j] == Next[i][j] && Prev[i][j] == 1) an++;
        }
    }
    if (sum == an) {
        out = 1;
    }
    return out;
}

void keyListen(float* time) {
    char command;
    int old = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, old | O_NONBLOCK);
    command = getchar();
    fcntl(0, F_SETFL, old);
    if (command == ' ') {
        command = 'a';
        while (command != ' ') {
            command = getchar();
        }
    }
    else if (command == '1') {
        *time = 1;
    }
    else if (command == '2') {
        *time = 0.5;
    }
    else if (command == '3') {
        *time = 0.05;
    }
    else if (command == '4') {
        *time = 0.005;
    }
    else if (command == 'z') {
        *time = 0;
    }
}

void clone(int** otkuda, int** kuda) {
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            kuda[i][j] = otkuda[i][j];
        }
    }
}

void check_in(int** st, int** fi) {
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            int sum = check(i, j, st);
            cell_is(sum, st, i, j, fi);
        }
    }
}

int check(int i, int j, int** A) {
    int sum = 0;
    for (int k = i - 1; k <= i + 1; k++) {
        for (int g = j - 1; g <= j + 1; g++) {
            if (k == i && g == j) {
                continue;
            }
            sum += A[(k + W) % W][(g + L) % L];
        }
    }
    return sum;
}

void cell_is(int sum, int** A, int i, int j, int** fi) {
    if (A[i][j] == 1) {
        if (sum == 2 || sum == 3) {
            fi[i][j] = 1;
        }
        else {
            fi[i][j] = 0;
        }
    }
    else {
        if (sum == 3) {
            fi[i][j] = 1;
        }
        else {
            fi[i][j] = 0;
        }
    }
}

void start(int** First) {
    printf("Select game start mode:\n 1 - input yourself\n 2 - launching the prepared file \n");
    int reg = 0;
    char c;
    while (scanf("%d%c", &reg, &c) != 2
        || (reg != 1 && reg != 2)
        || (c != ' ' && c != '\n')) {
        printf("Oh oh! You entered something wrong! Push Enter!\n");
        scanf("%c", &c);
    }
    if (reg == 1) {
        printf("Manual entry rules:\n");
        printf("Enter numbers from 1 to 80 and 25 separated by a space and enter\n");
        printf("To finish, enter any non-natural number.\n");
        hands_in(First);
    }
    else if (reg == 2) {
        auto_in(First);
    }
}

void auto_in(int** First) {
    char  r = ' ', c = '0';
    printf("Select a file: \n1 - glider \n2 - the beauty");
    printf("\n3 - a gun \n4 - pentomino \n5 - Something strange \n");
    while ((r < '1' || r > '5') || (c != '\n')) {
        r = getchar();
        c = getchar();
        printf("Chet net! \n");
    }
    switch (r) {
    case '1':
        file1(First);
        break;
    case '2':
        file2(First);
        break;
    case '3':
        file3(First);
        break;
    case '4':
        file4(First);
        break;
    case '5':
        file5(First);
        break;
    default:
        break;
    }
}

void hands_in(int** First) {
    int x = 1, y = 0, deb = 0;
    char c;
    while (1) {
        if (deb >= 5) {
            printf("Now I know for sure that you are not a computer. Can bring out the rules again? y | n\n");
            scanf("%c%c", &c, &c);
            if (c == 'y') {
                printf("Manual entry rules:\n");
                printf("Enter numbers from 1 to 80 and 25 separated by a space and enter\n");
                printf("To finish, enter any non-natural number.\n");
                deb = 0;
            }
            else if (c == 'n') {
                printf("Well look\n");
                deb = 0;
            }
            else {
                printf("You didn't even understand it?! OMG!\n");
            }
        }
        else if (scanf("%d%c", &x, &c) == 2 && x > 0 && (c == ' ' || c == '\n') && x <= 80) {
            if (scanf("%d%c", &y, &c) == 2 && y > 0 && (c == ' ' || c == '\n') && y <= 25) {
                First[x - 1][y - 1] = 1;
            }
            else {
                printf("Something is wrong! try again!\n Click to continue Enter\n");
                if (scanf("%d", &y) == 0) {
                    scanf("%c", &c);
                }
                deb++;
            }
        }
        else if (x <= 0) {
            break;
        }
        else {
            printf("Something is wrong! try again!\n Click to continue Enter\n");;
            if (scanf("%d", &x) == 0) {
                scanf("%c", &c);
            }
            deb++;
        }
    }
}


void show(int** First) {
    printf("\033[2J\033[1;1H");
    printf("\n");
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < W; j++) {
            if (First[j][i] == 0) {
                printf(RED "0" reset);
            }
            else {
                printf(CYN "1" reset);
            }
        }
        if (i != L - 1) {
            printf("\n");
        }
    }
}

void file1(int** First) {
    First[4][2] = 1;
    First[5][3] = 1;
    First[6][1] = 1;
    First[6][2] = 1;
    First[6][3] = 1;
}

void file2(int** First) {
    First[1][1] = 1;
    First[1][2] = 1;
    First[2][2] = 1;
    First[2][3] = 1;
    First[3][2] = 1;
    First[3][1] = 1;
}

void file3(int** First) {
    First[25][1] = 1, First[1][5] = 1;
    First[25][2] = 1, First[23][2] = 1;
    First[21][3] = 1, First[22][3] = 1;
    First[36][3] = 1, First[13][3] = 1;
    First[21][4] = 1, First[22][4] = 1;
    First[21][5] = 1, First[22][5] = 1;
    First[35][4] = 1, First[36][4] = 1;
    First[16][4] = 1, First[12][4] = 1;
    First[2][5] = 1, First[11][5] = 1;
    First[11][6] = 1, First[15][6] = 1;
    First[11][7] = 1, First[17][7] = 1;
    First[1][6] = 1, First[2][6] = 1;
    First[17][6] = 1, First[18][6] = 1;
    First[17][5] = 1, First[13][9] = 1;
    First[14][9] = 1, First[12][8] = 1;
    First[16][8] = 1, First[25][6] = 1;
    First[25][7] = 1, First[23][6] = 1;
    First[35][3] = 1, First[14][3] = 1;
}

void file4(int** First) {
    First[2][2] = 1;
    First[2][1] = 1;
    First[2][3] = 1;
    First[3][2] = 1;
    First[1][3] = 1;
}

void file5(int** First) {
    First[24][1] = 1, First[21][2] = 1, First[22][2] = 1, First[23][2] = 1;
    First[24][2] = 1, First[29][2] = 1, First[12][3] = 1, First[20][3] = 1;
    First[21][3] = 1, First[22][3] = 1, First[23][3] = 1, First[29][3] = 1;
    First[11][4] = 1, First[13][4] = 1, First[20][4] = 1, First[23][4] = 1;
    First[33][4] = 1, First[34][4] = 1, First[10][5] = 1, First[14][5] = 1;
    First[15][5] = 1, First[20][5] = 1, First[21][5] = 1, First[22][5] = 1;
    First[23][5] = 1, First[33][5] = 1, First[34][5] = 1, First[0][6] = 1;
    First[1][6] = 1, First[10][6] = 1, First[14][6] = 1, First[15][6] = 1;
    First[21][6] = 1, First[22][6] = 1, First[23][6] = 1, First[24][6] = 1;
    First[35][6] = 1, First[36][6] = 1, First[0][7] = 1, First[1][7] = 1;
    First[10][7] = 1, First[14][7] = 1, First[15][7] = 1, First[24][7] = 1;
    First[35][7] = 1, First[36][7] = 1, First[40][7] = 1, First[11][8] = 1;
    First[13][8] = 1, First[39][8] = 1, First[41][8] = 1, First[12][9] = 1;
    First[40][9] = 1, First[42][9] = 1, First[42][10] = 1, First[42][11] = 1;
    First[43][11] = 1;
}
