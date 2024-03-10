#include <stdio.h>

// Fonction pour vérifier si le feu se propage autour de la cellule (x, y)
int burn_around(int x, int y, int N, int M, char map[N][M]) {
    if (y >0 && map[y-1][x] == '.') map[y-1][x] = 'A';
    if (x > 0 && map[y][x-1] == '.') map[y][x-1] = 'A';
    if (y < N-1 && map[y+1][x] == '.') map[y+1][x] = 'A';
    if (x < M-1 && map[y][x+1] == '.') map[y][x+1] = 'A';

    if (y > 0 && (map[y-1][x] == 'S' || map[y-1][x] == 'D')) return 1;
    if (y < N-1 && (map[y+1][x] == 'S' || map[y+1][x] == 'D')) return 1;
    if (x > 0 && (map[y][x-1] == 'S' || map[y][x-1] == 'D')) return 1;
    if (x < M-1 && (map[y][x+1] == 'S' || map[y][x+1] == 'D')) return 1;

    return 0;
}

// Fonction pour vérifier si le prisonnier peut se déplacer dans une direction donnée
int can_move_dir(int x, int y, char dir, int N, int M, char map[N][M]) {
    if (dir == 'T') return y > 0 && map[y-1][x] == '.';
    else if (dir == 'B') return y < N-1 && map[y+1][x] == '.';
    else if (dir == 'L') return x > 0 && map[y][x-1] == '.';
    else if (dir == 'R') return x < M-1 && map[y][x+1] == '.';
    else return 0;
}

// Fonction pour vérifier le nombre de directions possibles de déplacement
int can_move(int x, int y, int N, int M, char map[N][M]) {
    int top = can_move_dir(x, y, 'T', N, M, map);
    int bottom = can_move_dir(x, y, 'B', N, M, map);
    int left = can_move_dir(x, y, 'L', N, M, map);
    int right = can_move_dir(x, y, 'R', N, M, map);
    return top + left + right + bottom;
}

// Fonction pour vérifier si le prisonnier peut gagner en se déplaçant dans une direction donnée
int win_move(int x, int y, int N, int M, char map[N][M]) {
    int top = y > 0 && map[y-1][x] == 'S';
    int bottom = y < N-1 && map[y+1][x] == 'S';
    int left = x > 0 && map[y][x-1] == 'S';
    int right = x < M-1 && map[y][x+1] == 'S';
    return top || left || right || bottom;
}

// Fonction pour déplacer le prisonnier
int move_prisoner(int N, int M, char map[N][M]) {
    int prisoner_position[2]={-1,1};
    int exit_position[2]={-1,1};
    
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < M; k++) {
            if (map[j][k] == 'S') {
                exit_position[0] = k;
                exit_position[1] = j;
            } else if (map[j][k] == 'D') {
                prisoner_position[0] = k;
                prisoner_position[1] = j;
            }
        }
    }

    if (prisoner_position[0] == exit_position[0] && prisoner_position[1] == exit_position[1]) {
        // Le prisonnier est déjà à la sortie
        return 1;
    }

    int delta_x = prisoner_position[0] - exit_position[0];
    int delta_y = prisoner_position[1] - exit_position[1];

    int win = win_move(prisoner_position[0], prisoner_position[1], N, M, map);
    if (win) return 1;
    else {
        int nb_move = can_move(prisoner_position[0], prisoner_position[1], N, M, map);
        if (nb_move > 0) {
            int x = prisoner_position[0];
            int y = prisoner_position[1];
            int top = can_move_dir(x, y, 'T', N, M, map);
            int bottom = can_move_dir(x, y, 'B', N, M, map);
            int left = can_move_dir(x, y, 'L', N, M, map);
            int right = can_move_dir(x, y, 'R', N, M, map);

            if (nb_move == 1) {
                map[y][x] = 'L';
                if (top) map[y-1][x] = 'D';
                else if (bottom) map[y+1][x] = 'D';
                else if (right) map[y][x+1] = 'D';
                else if (left) map[y][x-1] = 'D';
            }
            else {
                if (delta_x > 0 && left) map[y][x-1] = 'D';
                else if (delta_x < 0 && right) map[y][x+1] = 'D';
                else if (delta_y < 0 && bottom) map[y+1][x] = 'D';
                else if (delta_y > 0 && top) map[y-1][x] = 'D';
                else if (top) map[y-1][x] = 'D';
                else if (bottom) map[y+1][x] = 'D';
                else if (right) map[y][x+1] = 'D';
                else if (left) map[y][x-1] = 'D';
            }

            return 1;
        } else {
            return 0;
        }
    }
}

char run_instance(int N, int M, char map[N][M]) {
    int turn = 0;
    while (turn < N * M) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < M; k++) {
                if (map[j][k] == 'A') {
                    map[j][k] = 'F';
                }
            }
        }

        for (int j = 0; j < N; j++) {
            for (int k = 0; k < M; k++) {
                if (map[j][k] == 'F') {
                    if (burn_around(k, j, N, M, map)) return 'N';
                }
            }
        }

        if (move_prisoner(N, M, map)) return 'Y';

        turn++;
    }
    return 'N';
}

int main(void) {
    int T;
  scanf("%d",&T);

    for (int t = 0; t < T; t++) {
        int N, M;
        scanf("%d %d", &N, &M);

        char map[N][M];
        for (int i = 0; i < N; i++) {
            scanf("%s", map[i]);
        }

        char resultat = run_instance(N, M, map);
        printf("%c\n", resultat);
    }

    return 0;
}