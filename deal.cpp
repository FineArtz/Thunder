#include "deal.h"

using namespace Game;

extern std::map<int, bool> keyboard;
extern Player player;
extern std::vector<Enemy> enemy;
extern std::vector<Bullet> bullet;
extern bool gameOver;
extern int soulAttack;
extern int score, highestScore;
extern double endTime;

extern Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];

const double PI = std::acos(-1);

double lastBulletTime = 0.0;
double lastSoulAttackTime = 0.0;

//Deal with Events
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
        if (duration - lastSoulAttackTime >= 1.0){
            bullet.clear();
            for (auto &ene : enemy){
                ene.HP -= 50;
            }
            --soulAttack;
            lastSoulAttackTime = duration;
        }
    }

    if (player.isCol){ //is collided
        player.pos.x = PLAY_WIDTH / 2;
        player.pos.y = PLAY_HEIGHT - player.imgh / 2 * player.hRate;
        bullet.clear();
        player.isCol = false;
    }
    return 0;
}

//double lastEnemyTime[10] = {0.0}; //the index represent the eType
double lastEnemyTime[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
const double gapEnemy[5] = {1.5, 3.5, 2147483647, 2147483614, 2147483647}; //gap of two enemies' arrival

void dealWithEnemy(){
    auto ite = enemy.begin();
    int wb, hb;
    getImageSize(imageBullet, wb, hb);
    while (ite != enemy.end()){
        //std::cout << "DEALWITHENEMY" << std::endl;
        if (ite->HP <= 0){ //be destroyed
            if (ite->eraseTime == 0){
                ite->eraseTime = duration;
                score += 100;
                ++ite;
            }
            else{
                bool b = drawBurstAnime(3, *ite);
                if (!b){
                    enemy.erase(ite);
                    //std::cout << "ERASE ENEMY" << std::endl;
                }
                else ++ite;
            }
            continue;
        }

        ite->pos = ite->pos + ite->vel;
        if (outOfScreen(*ite))
            enemy.erase(ite);

        if (ite == enemy.end()) break;
        if (duration - ite->bulTime >= ite->speedAttack){//Create new bullets
            double wRate = ite->wRate;
            double hRate = ite->hRate;
            switch(ite->eType){
                //each case is added an extra pair of brackets to avoid jumping labels and crossing initializations
                case 0:{ //emit one bullet to the player
                    double bx = ite->pos.x, by = ite->pos.y + (ite->imgh / 2 - hb) * ite->hRate; //position in x and y
                    double ang = atan((player.pos.x - bx) / (player.pos.y - by)); //emitting angle in radian measure
                    double vx = abs(5 * sin(ang)), vy = abs(5 * cos(ang)); //velocity in x and y
                    if (bx > player.pos.x) vx = -vx;
                    if (by > player.pos.y) vy = -vy;
                    ang = abs(ang / PI * 180); //transform into angular measure
                    //further transformation
                    if (bx >= player.pos.x && by >= player.pos.y)
                        ang = 180 - ang;
                    else if (bx >= player.pos.x && by < player.pos.y)
                        ang = ang;
                    else if (bx < player.pos.x && by >= player.pos.y)
                        ang = 180 + ang;
                    else if (bx < player.pos.x && by < player.pos.y)
                        ang = 360 - ang;
                    bullet.emplace_back(imageBullet, bx, by, vx, vy, wRate, hRate, ang, false);
                    ite->bulTime = duration; //update the last emitting time
                    break;
                }
                case 1:{ //emit bullets as a semicircle
                    //int AMOUNT = 18; //the number of bullets, may change with hardness(ToDo)
                    double radio = ite->colR; //initial radio of the semicircle
                    for (int k = 0; k <= 18; ++k){
                        double ang = (double)k / 18.0 * PI; //emitting angle in radian measure
                        double bx = ite->pos.x - radio * std::cos(ang), by = ite->pos.y + radio * std::sin(ang); //position in x and y
                        double vx = -3 * std::cos(ang), vy = 3 * std::sin(ang); //velocity in x and y
                        ang = (k <= 9) ? (90 - k * 10) : (450 - k * 10); //emitting angle in angular measure
                        bullet.emplace_back(imageBullet, bx, by, vx, vy, wRate, hRate, ang, false);
                    }
                    ite->bulTime = duration; //update the last emitting time
                    break;
                }
                default:
                    break;
            }
        }
        ++ite;
    }
    //create a new enemy
    for (int i = 0; i <= 2; ++i){
        if (duration - lastEnemyTime[i] >= gapEnemy[i]){
            int we, he;
            getImageSize(imageEnemy, we, he);
            int r = rand() % (PLAY_WIDTH - we - we / 2) + we / 2;
            switch(i){
                case 0:{
                    enemy.emplace_back(imageEnemy, r, he / 2 * 0.5, 0);
                    Enemy &ene0 = enemy[enemy.size() - 1];
                    ene0.wRate = 0.5;
                    ene0.hRate = 0.5;
                    ene0.colR = hypot(ene0.imgw / 2 * ene0.wRate, ene0.imgh / 2 * ene0.hRate);
                    if (outOfScreen(ene0))
                        maintainToScreen(ene0);
                    lastEnemyTime[0] = duration;
                    break;
                }
                case 1:{
                    if (duration < 40) break;
                    enemy.emplace_back(imageEnemy, r, he / 2 * 0.5, 1);
                    Enemy &ene1 = enemy[enemy.size() - 1];
                    ene1.wRate = 1.0;
                    ene1.hRate = 1.0;
                    ene1.colR = hypot(ene1.imgw / 3 * ene1.wRate, ene1.imgh / 3 * ene1.hRate);
                    if (outOfScreen(ene1))
                        maintainToScreen(ene1);
                    lastEnemyTime[1] = duration;
                    break;
                }
                default:
                    break;
            }
        }
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
                if (ite->eraseTime == 0){
                    if (isCollide(*itb, *ite)){
                        --ite->HP;
                        bullet.erase(itb);
                        break;
                    }
                }
                ++ite;
            }
            if (itb != bullet.end()) ++itb;
        }
        else{
            if (isCollide(*itb, player)){
                #ifndef DEBUG_MODE
                --player.HP;
                if (soulAttack < 3) ++soulAttack;
                player.isCol = true;
                #endif // DEBUG_MODE
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
            #ifndef DEBUG_MODE
            --player.HP;
            if (soulAttack < 3) ++soulAttack;
            player.isCol = true;
            #endif // DEBUG_MODE
        }
        ++ite;
    }
}

bool dealWithEnd(){
    enemy.clear();
    bullet.clear();
    gameOver = true;
    if (!drawBurstAnime(2, player)){ //show score
        static int ctrl = 0; //control delay time
        if (player.eraseTime > 0){
            drawEndInfo();
            if (ctrl % 3 == 0) player.eraseTime -= 1; //do per 5 frames
            if (ctrl % 3 == 0) ++score; //do per 5 frames
            ++ctrl;
            return false;
        }
        else{
            return true;
        }
    }
    return false;
}
