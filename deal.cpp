#include "SDL2_header.h"
#include "item.h"
#include "deal.h"
#include "draw.h"
#include <map>

using namespace Game;

extern std::map<int, bool> keyboard;
extern Player player;
extern std::vector<Enemy> enemy;
extern std::vector<Bullet> bullet;
extern bool gameOver;
extern int soulAttack;

extern Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];

const double PI = 3.1415926;

//Deal with Events
double lastBulletTime = 0.0;
int dealWithPlayer(){
    //Game Over
    if (player.HP <= 0){
        return -1;
    }

    //movement
	bool move = false;
	if(keyboard[KEY_UP]	|| keyboard['w']){ //move forward
		player.vel = player.vel + PointD(0,-1) * player.speed;
		move = true;
	}
	if(keyboard[KEY_DOWN] || keyboard['s']){ //move backward
		player.vel = player.vel + PointD(0,+1) * player.speed;
		move = true;
	}
	if(keyboard[KEY_LEFT] || keyboard['a']){ //move leftward
		player.vel = player.vel + PointD(-1,0) * player.speed;
		move = true;
	}
	if(keyboard[KEY_RIGHT] || keyboard['d']){ //move rightward
		player.vel = player.vel + PointD(+1,0) * player.speed;
		move = true;
	}

	double len = player.vel.length();
	if(len > player.speed){ //move to fast!
		player.vel = player.vel / len * player.speed;
	}
	player.pos = player.pos + player.vel;
	maintainToScreen(player);

	if(!move){ //not move, velocity gradually lows
		player.vel = player.vel * 0.8; //speed of lowness
		if(player.vel.length() < 0.1)
			player.vel = PointD(); //stop
	}

	//shoot
	int wb, wp, hb, hp;
	getImageSize(imageBullet, wb, hb);
	if (keyboard[KEY_ENTER] || keyboard['z']){ //create a new bullet
        if (duration - lastBulletTime >= player.speedAttack){
            getImageSize(imagePlayer, wp, hp);
            //PointD newBullet(posPlayer.x, posPlayer.y - hp / 2 - hb / 2);
            bullet.emplace_back(imageBullet, player.pos.x, player.pos.y - (hp / 2 - hb) * player.hRate,
                                0, -20, player.wRate, player.hRate, 0, true);
           // bullet[bullet.size() - 1].wRate = player.wRate;
            //bullet[bullet.size() - 1].hRate = player.hRate;
            lastBulletTime = duration;
        }
    }

    if ((keyboard[KEY_SPACE] || keyboard['x']) && soulAttack > 0){ //soul attack
        bullet.clear();
        for (auto ene : enemy){
            ene.HP -= 50;
        }
        --soulAttack;
    }
    //is collided
    //ToDo
    return 0;
}

//double lastEnemyTime[10] = {0.0}; //the index represent the eType
double lastEnemyTime = 0.0;
const double gapEnemy = 1.5; //gap of two enemies' arrival
void dealWithEnemy(){
    auto ite = enemy.begin();
    int wb, hb;
    getImageSize(imageBullet, wb, hb);
    while (ite != enemy.end()){
        if (ite->HP <= 0){
             enemy.erase(ite);
             continue;
        }
        ite->pos = ite->pos + ite->vel;
        if (outOfScreen(*ite))
            enemy.erase(ite);
        if (ite == enemy.end()) break;
        if (duration - ite->bulTime >= ite->speedAttack){//Create a new bullet
            double wRate = ite->wRate;
            double hRate = ite->hRate;
            double bx = ite->pos.x, by = ite->pos.y + (ite->imgh / 2 - hb) * ite->hRate;
            double ang = atan((player.pos.x - bx) / (player.pos.y - by));
            double vx = abs(5 * sin(ang)), vy = abs(5 * cos(ang));
            if (bx > player.pos.x) vx = -vx;
            if (by > player.pos.y) vy = -vy;
            ang = abs(ang / PI * 180);
            if (bx >= player.pos.x && by >= player.pos.y)
                ang = 180 - ang;
            else if (bx >= player.pos.x && by < player.pos.y)
                ang = ang;
            else if (bx < player.pos.x && by >= player.pos.y)
                ang = 180 + ang;
            else if (bx < player.pos.x && by < player.pos.y)
                ang = 360 - ang;
            bullet.emplace_back(imageBullet, bx, by, vx, vy, wRate, hRate, ang, false);

            ite->bulTime = duration;
        }
        ++ite;
    }
    if (duration - lastEnemyTime >= gapEnemy){
        int we, he;
        getImageSize(imageEnemy, we, he);
        int r = rand() % (PLAY_WIDTH - we);
        enemy.emplace_back(imageEnemy, r, he / 2 * 0.5, 0);
        Enemy &ene = enemy[enemy.size() - 1];
        ene.wRate = 0.5;
        ene.hRate = 0.5;
        ene.colR = hypot(ene.imgw / 2 * ene.wRate, ene.imgh / 2 * ene.hRate);
        if (outOfScreen(enemy[enemy.size() - 1]))
            maintainToScreen(enemy[enemy.size() - 1]);
        lastEnemyTime = duration;
    }
}

void dealWithBullet(){
    auto itb = bullet.begin();
    while (itb != bullet.end()){
        itb->pos = itb->pos + itb->vel;
        if (outOfScreen(*itb))
            bullet.erase(itb); //out of screen
        if (itb == bullet.end()) break;
        else ++itb;
    }
    //if (keyboard['z']) std::cout << "Z" << std::endl;
}

void dealWithCollision(){
    //bullets with planes
    auto itb = bullet.begin();
    while (itb != bullet.end()){
        //std::cout << itb->isPlayer << std::endl;
        //system("PAUSE");
        if (itb->isPlayer){
            auto ite = enemy.begin();
            while (ite != enemy.end()){
                if (isCollide(*itb, *ite)){
                    --ite->HP;
                    bullet.erase(itb);
                    break;
                }
                ++ite;
            }
            if (itb != bullet.end()) ++itb;
        }
        else{
            if (isCollide(*itb, player)){
                --player.HP;
                player.isCol = true;
                bullet.erase(itb);
                continue;
            }
            else ++itb;
        }
    }

    //player with enemies
    auto ite = enemy.begin();
    while (ite != enemy.end()){
        if (isCollide(*ite, player)){
            ite->HP = 0;
            --player.HP;
            player.isCol = true;
        }
        ++ite;
    }
}

bool dealWithEnd(){
    enemy.clear();
    bullet.clear();
    gameOver = true;
    return !drawEndAnime();
}
