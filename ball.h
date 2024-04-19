#include <SDL.h>
#include <stdlib.h>

typedef struct Ball {
    float x;
    float y;
    int radius;
    float oldX;
    float oldY;
    float aX;
    float aY;
    int colR, colG, colB;
} Ball;

Ball* BallCreate(int x, int y, int radius, float aX, float aY) {
    Ball* newBall = (Ball*)malloc(sizeof (struct Ball));
    newBall->x = x;
    newBall->y = y;
    newBall->oldX = x;
    newBall->oldY = y;
    newBall->aX = aX;
    newBall->aY = aY;
    newBall->colR = rand() % 256;
    newBall->colG = rand() % 256;
    newBall->colB = rand() % 256;

    newBall->radius = radius;
    return newBall;
}

void BallUpdate(Ball* ball, double deltaTime) {
    float vX = ball->x - ball->oldX;
    float vY = ball->y - ball->oldY;

    ball->oldX = ball->x;
    ball->oldY = ball->y;

    ball->x += vX + ball->aX * deltaTime*deltaTime;
    ball->y += vY + ball->aY * deltaTime*deltaTime;
}

void BallRenderDraw(Ball* ball, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, ball->colR, ball->colG, ball->colB, 255);
    RenderFillCircle(renderer, ball->x, ball->y, ball->radius);
}
