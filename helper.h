float EuclideanDistance(float x1, float y1, float x2, float y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

void RenderFillCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int startX = centerX - radius;
    int startY = centerY - radius;
    int endX = centerX + radius;
    int endY = centerY + radius;

    for(int x=startX; x<=endX; x++) {
        for(int y=startY; y<=endY; y++) {
            int dist = EuclideanDistance(centerX, centerY, x, y);
            if(dist <= radius) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}
