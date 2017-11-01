#ifndef DEAL_H
#define DEAL_H

#include <map>
#include <iostream>

//double lastBulletTime = 0.0;
int dealWithPlayer();

//double lastEnemyTime[10] = {0.0}; //the index represent the eType
//double lastEnemyTime = 0.0;
//const double gapEnemy = 2.0; //gap of two enemies' arrival
void dealWithEnemy();

void dealWithBullet();

void dealWithCollision();

bool dealWithEnd();

#endif // DEAL_H
