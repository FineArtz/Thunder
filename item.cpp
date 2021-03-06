#include "item.h"

namespace Game{
void Enemy::setAbility(){
    switch(eType){
        case 0:
            vel.x = 0;
            vel.y = 3;
            HP = 1;
            speedAttack = std::max(2.0 - duration * 0.05, 0.4); //increase with time, but has a limit
            bulTime = 0.0;
            break;
        case 1:
            vel.x = 0;
            vel.y = 1.5;
            HP = 3;
            speedAttack = 32767; //only attack once
            bulTime = -32768;
            break;
        case 2:
            vel.x = 0.0;
            vel.y = 0.0; //the initial velocity depends on the relative position between player and this plane
            HP = 1;
            speedAttack = 0.1;
            bulTime = 0.0;
            //std::cout << "CREATE ENEMY 2" << std::endl;
            break;
        default:
            break;
    }
}

void Bonus::setBonus(){
    switch(bType){
        case 0: //add 500 score
            speed = -5.0 / 2.5;
            deltaSpeed = 3.0 / 2.5;
            maxSpeed = 10.0 / 2.5;
            break;
        case 1: //add 1000 score
            speed = -5.0 / 2.5;
            deltaSpeed = 4.5 / 2.5;
            maxSpeed = 15.0 / 2.5;
            break;
        case 2: //increase speedAttack
            speed = -5.0 / 2.5;
            deltaSpeed = 5.0 / 2.5;
            maxSpeed = 15.0 / 2.5;
            break;
        case 3: //add a bomb
            speed = 0.0 / 2.5;
            deltaSpeed = 5.0 / 2.5;
            maxSpeed = 15.0 / 2.5;
            break;
        case 4: //add a health point
            speed = 0.0 / 2.5;
            deltaSpeed = 6.0 / 2.5;
            maxSpeed = 18.0 / 2.5;
            break;
        default:
            break;
    }
    wRate = 0.2;
    hRate = 0.2;
    colR = imgw * wRate * 4;
}

bool outOfScreen(const item &it){
    int w, h;
    getImageSize(it.img, w, h);
    w = w / 2 * it.wRate;
    h = h / 2 * it.hRate;
    if (it.pos.x  <= 0 || it.pos.y <= 0
     || it.pos.x >= PLAY_WIDTH || it.pos.y >= PLAY_HEIGHT)
        return true;
    return false;
}

void maintainToScreen(item &it){
    int w, h;
    getImageSize(it.img, w, h);
    w = w / 2 * it.wRate;
    h = h / 2 * it.hRate;
    if (it.pos.x < w) it.pos.x = w;
    if (it.pos.x > PLAY_WIDTH - w) it.pos.x = PLAY_WIDTH - w;
    if (it.pos.y < h) it.pos.y = h;
    if (it.pos.y > PLAY_HEIGHT - h) it.pos.y = PLAY_HEIGHT - h;
}

bool isCollide(const item &i1, const item &i2){
    double d = hypot(i1.pos.x - i2.pos.x, i1.pos.y - i2.pos.y);
    d /= i1.wRate;
    if (d - i1.colR - i2.colR <= 10e-6) return true;
    return false;
}
}
