#ifndef ITEM_H
#define ITEM_H

//#include "SDL2_header.h"

namespace Game{
extern void drawImageC();

class item{
public:
    //constructor
    item(Image *i = nullptr,
         const double &x = SCREEN_WIDTH / 2, const double &y = SCREEN_HEIGHT / 2)
         : pos(x, y), img(i) {
        if (img != nullptr){
            getImageSize(img, imgw, imgh);
        }
        else{
            imgw = 0;
            imgh = 0;
        }
    };
    virtual ~item(){ img = nullptr; }

    PointD pos;
    Image *img;
    double wRate = 1.0, hRate = 1.0;
    int imgw, imgh;
};

class Bullet : public item{
public:
    //constructor
    Bullet() = default;
    Bullet(Image *i = nullptr,
           const double &xx = SCREEN_WIDTH / 2, const double &yy = SCREEN_HEIGHT / 2,
           const double &vx = 0, const double &vy = -20,
           bool isP = true)
           : item(i, xx, yy), vel(vx, vy), isPlayer(isP) {}
    ~Bullet(){ img = nullptr; }

    PointD vel;
    bool isPlayer;
};

class Plane : public item{
public:
    //constructor
    Plane(Image *i = nullptr,
          const double &xx = SCREEN_WIDTH / 2, const double &yy = SCREEN_HEIGHT / 2,
          const double &vx = 0, const double &vy = 0,
          const double &hp = 1, bool isP = true)
          : item(i, xx, yy), vel(vx, vy), HP(hp), isPlayer(isP) {}
    Plane(const double &hp, const double &xx, const double &yy, const double &vx, const double &vy)
          : Plane(nullptr, hp, xx, yy, vx, vy, true) {};
    ~Plane(){ img = nullptr; }

    PointD vel;
    double HP;
    bool isPlayer;
};

class Player : public Plane{
public:
    //constructor
    Player(Image *i = nullptr,
           const double &xx = SCREEN_WIDTH / 2, const double &yy = SCREEN_HEIGHT / 2,
           const double &vx = 0, const double &vy = 0,
           const double &hp = 5) : Plane(i, xx, yy, vx, vy, hp, true), attack(1), speed(5), speedAttack(1) {};
    ~Player(){ img = nullptr; }

    double attack = 1.0;
    double speed = 5.0;
    double speedAttack = 1.0;
};

class Enemy : public Plane{
public:
    //constructor
    Enemy(Image *i = nullptr,
          const double &xx = SCREEN_WIDTH / 2, const double &yy = SCREEN_HEIGHT / 2,
          const int &et = 0) : Plane(i, xx, yy, 0, 10, 1, false), eType(et) {};
    ~Enemy(){ img = nullptr; };

    void setAbility();

    int eType = 0; //type of enemy
    double attack = 1.0;
    double speedAttack = 100.0;
    double bulTime = 0.0;
};
void Enemy::setAbility(){//ToDo
    switch(eType){
        case 0:
            vel.x = 0;
            vel.y = 10;
            HP = 1;
            attack = 0.0;
            speedAttack = 100.0;
            bulTime = 0.0;
            break;
        default:
            break;
    }
}

inline bool outOfScreen(const item &it){
    int w, h;
    getImageSize(it.img, w, h);
    w = w / 2 * it.wRate;
    h = h / 2 * it.hRate;
    if (it.pos.x - w <= 0 || it.pos.y - h <= 0
     || it.pos.x + w >= SCREEN_WIDTH || it.pos.y + h >= SCREEN_HEIGHT)
        return true;
    return false;
}
void maintainToScreen(item &it){
    int w, h;
    getImageSize(it.img, w, h);
    w = w / 2 * it.wRate;
    h = h / 2 * it.hRate;
    if (it.pos.x < w) it.pos.x = w;
    if (it.pos.x > SCREEN_WIDTH - w) it.pos.x = SCREEN_WIDTH - w;
    if (it.pos.y < h) it.pos.y = h;
    if (it.pos.y > SCREEN_HEIGHT - h) it.pos.y = SCREEN_HEIGHT - h;
}
}

#endif // ITEM_H
