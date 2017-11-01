/* This header contains functions about drawing */
#ifndef DRAW_H
#define DRAW_H

#include "SDL2_header.h"
#include "item.h"
#include "others.h"

#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>

void drawImageC(Image *img, int x, int y,
                const double &widthRate = 1, const double &heightRate = 1,
                const double &angle = 0, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr);
void drawImageC(Image *img, PointD p,
                const double &widthRate = 1, const double &heightRate = 1,
                const double &angle = 0, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr);
void drawImageC(const Game::item &it, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr);

void drawDebugInfo();
void drawGameInfo();
void drawBackground();
void drawForeground();
void drawPlayer();
void drawEnemy();
void drawBullet();
bool drawBurstAnime(int, const Game::item&);
void drawEndInfo();

#endif // DRAW_H
