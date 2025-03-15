#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_WIDTH 3
#define BALL_SIZE 1
#define MAX_SCORE 21

typedef struct {
    int x, y;
    int score;
} Player;

typedef struct {
    int x, y;
    int vx, vy;
} Ball;

void draw(Player p1, Player p2, Ball ball) {
    system("clear"); // очищаем экран

    // рисуем поле
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1) {
                printf("|"); // границы
            } else if (y == 0 || y == HEIGHT - 1) {
                printf("="); // верхняя и нижняя границы
            } else if (x == 1 && y >= p1.y && y < p1.y + PADDLE_WIDTH) {
                printf("|"); // ракетка игрока 1
            } else if (x == WIDTH - 2 && y >= p2.y && y < p2.y + PADDLE_WIDTH) {
                printf("|"); // ракетка игрока 2
            } else if (x == ball.x && y == ball.y) {
                printf("*"); // мяч
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("| Player_1 | Score: %d |\n", p1.score);
    printf("| Player_2 | Score: %d |\n", p2.score);
}

void update_ball(Ball* ball, Player* p1, Player* p2) {
    ball->x += ball->vx;
    ball->y += ball->vy;

    // проверка на столкновение с верхней и нижней границей
    if (ball->y <= 1 || ball->y >= HEIGHT - 2) {
        ball->vy = -ball->vy; // отражение
    }

    // проверка на столкновение с ракетками
    if (ball->x == 2 && ball->y >= p1->y && ball->y < p1->y + PADDLE_WIDTH) {
        ball->vx = -ball->vx; // отражение от ракетки 1
    } else if (ball->x == WIDTH - 3 && ball->y >= p2->y && ball->y < p2->y + PADDLE_WIDTH) {
        ball->vx = -ball->vx; // отражение от ракетки 2
    }

    // проверка на выход за границы
    if (ball->x <= 1) {
        p2->score++; // игрок 2 забивает
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;
        ball->vx = 1; // начальное направление
    } else if (ball->x >= WIDTH - 2) {
        p1->score++; // игрок 1 забивает
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;
        ball->vx = -1; // начальное направление
    }
}

int main() {
    Player p1 = { 1, HEIGHT / 2 - 1, 0 }; // игрок 1
    Player p2 = { WIDTH - 2, HEIGHT / 2 - 1, 0 }; // игрок 2
    Ball ball = { WIDTH / 2, HEIGHT / 2, 1, 1 }; // мяч

    char command;
    while (p1.score < MAX_SCORE && p2.score < MAX_SCORE) {
        draw(p1, p2, ball);
        printf("Press A/Z for Player 1, K/M for Player 2, Space to step: ");
        command = getchar();
        getchar(); // игнорируем ввод новой строки

        switch (command) {
            case 'A':
            case 'a':
                if (p1.y > 1) p1.y--; // движение вверх
                break;
            case 'Z':
            case 'z':
                if (p1.y < HEIGHT - PADDLE_WIDTH - 1) p1.y++; // движение вниз
                break;
            case 'K':
            case 'k':
                if (p2.y > 1) p2.y--; // движение вверх
                break;
            case 'M':
            case 'm':
                if (p2.y < HEIGHT - PADDLE_WIDTH - 1) p2.y++; // движение вниз
                break;
            case ' ':
                break; // просто пропускаем ход
            default:
                printf("Invalid command!\n");
                break;
        }

        update_ball(&ball, &p1, &p2);
        usleep(10000); // задержка для управления скоростью игры
    }

    // выводим поздравление победителя
    if (p1.score >= MAX_SCORE) {
        printf("Player 1 Wins!\n");
    }
    else if (p2.score >= MAX_SCORE) {
        printf("Player 2 Wins!\n");
    }

    return 0;
}
