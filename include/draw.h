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

//draw an image by its center position
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

void drawBackground();
//draw background images

void drawBonus();
//draw bonus

void drawBullet();
//draw bullets

bool drawBurstAnime(int, const Game::item&);
//draw a bursting animation when a plane dies

void drawDebugInfo();
//draw debug information in the debug mode

void drawEndInfo();
//draw some informations when the game is over

void drawEnemy();
//draw enemy planes

void drawForeground();
//draw foreground images

void drawGameInfo();
//draw some information of the game

void drawPlayer();
//draw the player plane


#endif // DRAW_H
