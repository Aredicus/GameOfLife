#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void game(int **First, int **Next);
void start(int **First);
void check_in(int **st, int **fi);
void clone(int **ot, int **ku);
int check(int i, int j, int **A);
void cell_is(int sum, int**A, int i, int j, int **fi);
void hands_in(int **First);
void auto_in(int **First);
void show(int **First);
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

int main(){
    int **First = calloc(W, sizeof(int *)), **Next = calloc(W, sizeof(int *));
    for (int i = 0; i < W; i++) {
        First[i] = calloc(L, sizeof(int));
        Next[i] = calloc(L, sizeof(int));
    }
    game(First, Next);
    return 0;
}

void game(int **First, int **Next){
    start(First);
    system("stty cbreak");
    char command = 'a';
    float time = 1;
    char c = 'a';
    while (1) {
        int old = fcntl(0, F_GETFL);
        fcntl(0, F_SETFL, old | O_NONBLOCK);
        command = getchar();
        fcntl(0, F_SETFL, old);
        if(command == ' '){
            command = 'a';
            while (command != ' ') {
                int old = fcntl(0, F_GETFL);
                fcntl(0, F_SETFL, old | O_NONBLOCK);
                command = getchar();
                fcntl(0, F_SETFL, old);
            }
        } else if (command == '1') {
            time = 1;
        } else if (command == '2') {
            time = 0.5;
        } else if (command == '3') {
            time = 0.01;
        } else if (command == '4') {
            time = 0.005;
        }
        show(First);
        usleep(time * 1000000);
        check_in(First, Next);
        clone(Next, First);
    }
    for (int i =0; i < W; i++) {
        free(First[i]);
        free(Next[i]);
    }
    free(First);
    free(Next);
}

void clone(int **ot, int **ku) {
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            ku[i][j] = ot[i][j];
        }
    }
}

void check_in(int **st, int **fi) {
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            int sum = check(i, j, st);
            cell_is(sum, st, i, j, fi);
        }
    }
}
    
int check(int i, int j, int **A){
    int sum = 0;
    for(int k =  i-1; k <= i+1; k++){
        for(int g = j-1; g <= j+1; g++){
            if(k == i && g == j){
                continue;
            }
            sum += A[(k + W) % W][(g + L) % L];
        }
    }
    return sum;
}

void cell_is(int sum, int**A, int i, int j, int **fi){
    if(A[i][j] == 1){
        if(sum == 2 || sum == 3){
            fi[i][j] = 1;
        } else {
            fi[i][j]  = 0;
        }
    } else {
        if(sum == 3) {
            fi[i][j] = 1;
        } else {
            fi[i][j] = 0;
        }
    }
}

void start(int **First){
    printf("Выберите режим начала игры:\n 1 - ввод самомстоятельно\n 2 - запуск заготовленного файла \n");
    int reg = 0;
    char c;
    while (scanf("%d%c", &reg, &c) != 2
           || (reg != 1 && reg != 2)
           || (c != ' ' && c != '\n')) {
        printf("Ой-ой! Вы что-то не то ввели! Попробуйте ещё раз!");
    }
    if (reg == 1) {
        hands_in(First);
    } else if (reg == 2) {
        auto_in(First);
    }
}

void auto_in(int **First) {
    int x, y;
    char c;
    while (1) {
        if (scanf("%d%c", &x, &c) == 2 && x  > 0 && (c == ' ' || c == '\n') && x <=80) {
            scanf("%d%c", &y, &c);
            First[x-1][y-1] = 1;
        } else if (x <= 0) {
            break;
        }
    }
}

void hands_in(int **First) {
    printf("Правила ручного ввода:\n Введите целые коордианты x и y в пределах от 1 до 80 и 25 соответственно.\n Каждый новвый ввод оформляйте через пробел или на новой строке, для вашего удобтсва.\n В случви ошибки, наш ИИ сообщит вам о некоректном вооде.\n Как только вы поймете, что выставили всё, что хотели, введите любое не натуральное число.\n");
    int x = 0, y = 0, deb = 0;
    char c;
    while (1){
        if (deb >= 5) {
            printf("Теперь я точно знаю, что ты не компьютер. Может вывести правила еще раз? y | n\n");
            scanf("%c", &c);
                if(c == 'y'){
                    printf("Правила ручного ввода:\n Введите коордианты x и y в пределах от 1 80 и 25 соответственно.\n Каждый новвый ввод оформляйте через пробел или на новой строке, для вашего удобтсва.\n В случви ошибки, наш ИИ сообщит вам о некоректном вооде.\n Как только вы поймете, что выставили всё, что хотели, введите любое не натуральное число.\n");
                    deb = 0;
                }  else if(c == 'n') {
                    printf("Ну смотри\n");
                    deb = 0;
                } else {
                    printf("Ты даже это не понял?! OMG!\n");
                }
            }
        else if(scanf("%d%c", &x, &c) == 2 && x > 0 && (c == ' ' || c == '\n') && x <= 80) {
            if(scanf("%d%c", &y, &c) == 2 && y > 0 && (c == ' ' || c == '\n') && y <= 25) {
            First[x-1][y-1] = 1;
            } else {
                printf("Что-то не так! Попробуйте еще раз!\n");
                deb++;
            }
        } else if (x <= 0) {
            break;
        } else {
            printf("Что-то не так! Попробуйте еще раз!\n");
            deb++;
        }
    }
}


void show(int **First) {
    printf("\033[2J\033[1;1H");
    printf("\n");
    for (int i =0; i < L; i++){
        for (int j = 0; j < W; j++) {
            if (First[j][i] == 0) {
                printf(RED "□" reset);
            } else {
                printf(BLU "■" reset);
            }
        }
        if( i != L - 1){
        printf("\n");
        }
    }
}
