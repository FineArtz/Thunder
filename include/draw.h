/* This header contains functions about drawing */
#ifndef DRAW_H
#define DRAW_H

#include "SDL2_header.h"
#include "item.h"


void drawImageC(Image *img, int x, int y,
                const double &widthRate, const double &heightRate,
                const double &angle, const Point *center,
                const FlipType &flip, const Rect *clip);

void drawImageC(Image *img, PointD p,
                const double &widthRate, const double &heightRate,
                const double &angle, const Point *center,
                const FlipType &flip, const Rect *clip);
void drawImageC(const Game::item &it, const Point *center,
                const FlipType &flip, const Rect *clip);

void drawDebugInfo();
void drawBackground();
void drawForeground();
void drawPlayer();
void drawEnemy();
void drawBullet();
bool drawEndAnime();

#endif // DRAW_H
