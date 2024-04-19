

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL.h>

#include "helper.h"
#include "linked_list.h"
#include "ball.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int sliderX = SCREEN_WIDTH / 2; 
int sliderY = SCREEN_HEIGHT - 50;
int sliderWidth = 300;
int sliderHeight = 20; 
int knobWidth = 10;
int knobX = sliderX;
int minRadius = 10;
int maxRadius = 100; 
bool draggingKnob = false; 

Node* head = NULL;
Ball* ball;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
void RenderSlider(SDL_Renderer* renderer) {
   
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255); 
    SDL_Rect sliderBar = {sliderX - sliderWidth / 2, sliderY, sliderWidth, sliderHeight};
    SDL_RenderFillRect(renderer, &sliderBar);

   
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    SDL_Rect knob = {knobX - knobWidth / 2, sliderY, knobWidth, sliderHeight};
    SDL_RenderFillRect(renderer, &knob);
}
void HandleSliderEvent(SDL_Event* ev) {
    if (ev->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = ev->button.x;
        int mouseY = ev->button.y;
        if (mouseX >= knobX - knobWidth / 2 && mouseX <= knobX + knobWidth / 2 &&
            mouseY >= sliderY && mouseY <= sliderY + sliderHeight) {
            draggingKnob = true;
        }
    } else if (ev->type == SDL_MOUSEBUTTONUP) {
        draggingKnob = false;
    } else if (ev->type == SDL_MOUSEMOTION && draggingKnob) {
        knobX = ev->motion.x;
        if (knobX < sliderX - sliderWidth / 2) {
            knobX = sliderX - sliderWidth / 2;
        } else if (knobX > sliderX + sliderWidth / 2) {
            knobX = sliderX + sliderWidth / 2;
        }
    }
}
void ApplyWall() {
    float centerX = SCREEN_WIDTH / 2;
    float centerY = SCREEN_HEIGHT / 2;
    float radius = 250;

    Node* tmp = head;
    while(tmp) {
        float ballX = ((Ball*)tmp->value)->x;
        float ballY = ((Ball*)tmp->value)->y;

        float dist = EuclideanDistance(ballX, ballY, centerX, centerY);
        float toX = ballX - centerX;
        float toY = ballY - centerY;

        if(dist > radius) {
            toX = (toX / dist) * radius;
            toY = (toY / dist) * radius;

            ((Ball*)tmp->value)->x = centerX + toX;
            ((Ball*)tmp->value)->y = centerY + toY;

            
        }
        tmp = tmp->next;
    }
}

void ApplyCollisions() {
    Node* tmpi = head;
    while(tmpi && tmpi->next) {
        Node* tmpj = tmpi->next;
        while(tmpj) {
            Ball* ball1 = (Ball*)tmpi->value;
            Ball* ball2 = (Ball*)tmpj->value;

            float toX = ball2->x - ball1->x;
            float toY = ball2->y - ball1->y;

            float dist = EuclideanDistance(ball1->x, ball1->y, ball2->x, ball2->y);
            float delta = (ball1->radius + ball2->radius) - dist;

            float nX = toX / dist;
            float nY = toY / dist;

            if(dist < (ball1->radius + ball2->radius)) {
                ball1->x -= 0.5 * delta * nX;
                ball1->y -= 0.5 * delta * nY;

                ball2->x += 0.5 * delta * nX;
                ball2->y += 0.5 * delta * nY;
            }
            tmpj = tmpj->next;
        }
        tmpi = tmpi->next;
    }
}

void update(double deltaTime) {
    int subStep = 4;
    double subDeltaTime = deltaTime / subStep;
    for(int i=0; i<subStep; i++) {
        Node* tmp = head;
        while(tmp) {
            BallUpdate(tmp->value, subDeltaTime);
            tmp = tmp->next;
        }
        ApplyWall();
        ApplyCollisions();
    }
}

void render(double deltaTime) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    Node* tmp = head;
    while(tmp) {
        BallRenderDraw((Ball*)tmp->value, renderer);
        tmp = tmp->next;
    }


    SDL_RenderPresent(renderer);
}

void CreateNewBall(int x, int y) {
    float radiusRatio = (float)(knobX - (sliderX - sliderWidth / 2)) / sliderWidth;
    int ballRadius = minRadius + (int)(radiusRatio * (maxRadius - minRadius));
    Ball* newBall = BallCreate(x, y,ballRadius);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    bool isRunning = true;
    SDL_Event ev;
    int lastTick = SDL_GetTicks();
    double deltaTime;
   
while(SDL_PollEvent(&ev) != 0) {
   
    HandleSliderEvent(&ev); 
}
void render(double deltaTime) {
   
    RenderSlider(renderer); 
    SDL_RenderPresent(renderer);
}
    while(isRunning) {
        lastTick = SDL_GetTicks();
        while(SDL_PollEvent(&ev) != 0) {
            if(ev.type == SDL_QUIT) {
                isRunning = false;
            }
            if(ev.type == SDL_MOUSEBUTTONDOWN) {
                CreateNewBall(ev.button.x, ev.button.y);
            }
        }
        update(deltaTime);
        render(deltaTime);
        deltaTime = (SDL_GetTicks() - lastTick) / 1000.0;
    }

    printf("%s \n", SDL_GetError());
    return 0;
}
