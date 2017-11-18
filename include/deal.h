/* This header contains functions about dealing with events */
#ifndef DEAL_H
#define DEAL_H

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "draw.h"
#include "others.h"
#include "SDL2_header.h"
#include "item.h"

int dealWithPlayer();

//double lastEnemyTime[10] = {0.0}; //the index represent the eType
//double lastEnemyTime = 0.0;
//const double gapEnemy = 2.0; //gap of two enemies' arrival
void dealWithEnemy();

void dealWithBullet();

void dealWithBonus();

void dealWithCollision();

bool dealWithEnd();

#endif // DEAL_H
