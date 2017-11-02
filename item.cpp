#include "item.h"

namespace Game{
void Enemy::setAbility(){//ToDo
    switch(eType){
        case 0:
            vel.x = 0;
            vel.y = 3;
            HP = 1;
            speedAttack = std::max(2.0 - duration * 0.05, 0.5);
            bulTime = 0.0;
            break;
        case 1:
            vel.x = 0;
            vel.y = 1.5;
            HP = 3;
            speedAttack = 32767;
            bulTime = -32768;
            break;
        default:
            break;
    }
}

bool outOfScreen(const item &it){
    int w, h;
    getImageSize(it.img, w, h);
    w = w / 2 * it.wRate;
    h = h / 2 * it.hRate;
    if (it.pos.x - w <= 0 || it.pos.y - h <= 0
     || it.pos.x + w >= PLAY_WIDTH || it.pos.y + h >= PLAY_HEIGHT)
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
