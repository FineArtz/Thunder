#ifndef ITEM_H
#define ITEM_H

#include <cmath>
//#include "SDL2_header.h"
/* Class Hierarchy
 * Base Class: item -> Bullet
 *                  -> Plane
 *                  ->          -> Player
 *                  ->          -> Enemy
 *
 * Members
 * Class item   :: Image *img           //a pointer to store the corresponding image
 *              :: PointD pos           //position
 *              :: double wRate, hRate  //magnification in width and height
 *              :: int imgw, imgh       //width and height of the image
 *              :: double colR          //collision radius
 *              :: double imgAngle      //angle of rotation
 *              :: item(...)            //constructor
 *              :: ~item()              //virtual destructor
 *
 * Class Bullet :: public inherited from Class item
 *              :: PointD vel           //velocity
 *              :: bool isPlayer        //whether the bullet is emitted by player or an enemy
 *              :: Bullet(...)          //constructor
 *              :: ~Bullet()            //destructor
 *
 * Class Plane  :: public inherited from Class item
 *              :: PointD vel           //velocity
 *              :: double HP            //health point
 *              :: bool isPlayer        //whether the plane is controlled by player
 *              :: Plane(...)           //constructor
 *              :: ~Plane()             //destructor
 *
 * Class Player :: public inherited from Class Plane
 *              :: double attack        //attack power
 *              :: double speed         //speed of movement
 *              :: double speedAttack   //speed of emitting bullets
 *              :: bool isCol           //whether player is collided
 *              :: Player(...)          //constructor
 *              :: ~Player(...)         //destructor
 *
 * Class Enemy  :: public inherited from Class Plane
 *              :: int eType            //type of enemy
 *              :: speedAttack          //speed of emitting bullets
 *              :: bulTime              //last emitting time
 */
namespace Game{
extern void drawImageC();

class item{
public:
    //constructor
    item(Image *i = nullptr,
         const double &x = SCREEN_WIDTH / 2, const double &y = SCREEN_HEIGHT / 2,
         const double &wr = 1.0, const double &hr = 1.0, const double &ang = 0.0)
         : img(i), pos(x, y), wRate(wr), hRate(hr), imgAngle(ang) {
            if (img != nullptr){
                getImageSize(img, imgw, imgh);
            }
            else{
                imgw = 0;
                imgh = 0;
            }
    };
    virtual ~item(){ img = nullptr; }

    Image *img;
    PointD pos;
    double wRate = 1.0, hRate = 1.0;
    int imgw, imgh;
    double colR = 0.0;
    double imgAngle = 0.0;
};

class Bullet : public item{
public:
    //constructor
    Bullet() = default;
    Bullet(Image *i = nullptr,
           const double &xx = SCREEN_WIDTH / 2, const double &yy = SCREEN_HEIGHT / 2,
           const double &vx = 0, const double &vy = -20, const double &wr = 1.0, const double &hr = 1.0,
           const double &ang = 0.0, bool isP = true)
           : item(i, xx, yy, wr, hr, ang), vel(vx, vy), isPlayer(isP) {}
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
    bool isCol = false;
};

class Enemy : public Plane{
public:
    //constructor
    Enemy(Image *i = nullptr,
          const double &xx = SCREEN_WIDTH / 2, const double &yy = SCREEN_HEIGHT / 2,
          const int &et = 0) : Plane(i, xx, yy, 0, 0, 1, false), eType(et) { setAbility(); };
    ~Enemy(){ img = nullptr; };

    void setAbility();

    int eType = 0; //type of enemy
    double speedAttack = 100.0;
    double bulTime = 0.0;
};
void Enemy::setAbility(){//ToDo
    switch(eType){
        case 0:
            vel.x = 0;
            vel.y = 3;
            HP = 1;
            speedAttack = 2.0;
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
inline bool isCollide(const item &i1, const item &i2){
    double d = hypot(i1.pos.x - i2.pos.x, i1.pos.y - i2.pos.y);
    d /= i1.wRate;
    if (d - i1.colR - i2.colR <= 10e-6) return true;
    return false;
}
}

#endif // ITEM_H
