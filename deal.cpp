#include "deal.h"

using namespace Game;

extern std::map<int, bool> keyboard;
extern Player player;
extern std::vector<Enemy> enemy;
extern std::vector<Bullet> bullet;
extern std::vector<Bonus> bonus;
extern bool gameOver;
extern int soulAttack;
extern int score, highestScore;
extern double endTime;

extern Image *imagePlayer, *imageBullet, *imageEnemy, *images[100], *imagesBonus[5];

const double PI = std::acos(-1);

double lastBulletTime = 0.0;
double lastSoulAttackTime = 0.0;
double ISA_Time = 0.0;

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

	//no bonus!
	if (ISA_Time > 10.0 && duration - ISA_Time >= 10.0){
        ISA_Time = 0.0;
        player.speedAttack *= 2;
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
const double gapEnemy[5] = {1.5, 3.3, 2.1, 2147483614, 2147483647}; //gap of two enemies' arrival

void dealWithEnemy(){
    auto ite = enemy.begin();
    int wb, hb;
    getImageSize(imageBullet, wb, hb);
    while (ite != enemy.end()){
        //std::cout << "DEALWITHENEMY" << std::endl;
        if (ite->HP <= 0){ //be destroyed
            if (ite->eraseTime == 0){

                if (ite->eType == 1){ //create bonus
                    int prob = rand() % 100;//probability
                    //4%: bonus HP
                    if (prob == 3) bonus.emplace_back(imagesBonus[4], ite->pos.x, ite->pos.y, 4);
                    //6%: bonus Bomb
                    else if (prob <= 9) bonus.emplace_back(imagesBonus[3], ite->pos.x, ite->pos.y, 3);
                    //10%: bonus speedAttack
                    else if (prob <= 19) bonus.emplace_back(imagesBonus[2], ite->pos.x, ite->pos.y, 2);
                    //20%: bonus 1000 points
                    else if (prob <= 39) bonus.emplace_back(imagesBonus[1], ite->pos.x, ite->pos.y, 1);
                    //40%: bonus 500 points
                    else if (prob <= 79) bonus.emplace_back(imagesBonus[0], ite->pos.x, ite->pos.y, 0);
                }

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
                    if (ite->pos.y < PLAY_HEIGHT * 0.8){ //A condition to decrease hardness
                        double bx = ite->pos.x, by = ite->pos.y + (ite->imgh / 2 - hb) * ite->hRate; //position in x and y
                        double ang = std::atan2(player.pos.y - by, player.pos.x - bx); //emitting angle in radian measure
                        double vx = 5 * std::cos(ang), vy = 5 * std::sin(ang); //velocity in x and y

                        ang = ang / PI * 180; //transform into angular measure
                        ang -= 90; //rotating angle
                        if (ang < 0) ang += 360;
                        bullet.emplace_back(imageBullet, bx, by, vx, vy, wRate, hRate, ang, false);
                        ite->bulTime = duration; //update the last emitting time
                    }
                    break;

                }
                case 1:{ //emit bullets as a semicircle
                    int AMOUNT = 25; //the number of bullets, may change with hardness(ToDo)
                    double radio = ite->colR; //initial radio of the semicircle
                    for (int k = 0; k <= AMOUNT; ++k){
                        double ang = (double)k / AMOUNT * PI; //emitting angle in radian measure
                        double bx = ite->pos.x - radio * std::cos(ang), by = ite->pos.y + radio * std::sin(ang); //position in x and y
                        double vx = -3 * std::cos(ang), vy = 3 * std::sin(ang); //velocity in x and y
                        ang = (k <= AMOUNT / 2) ? (90 - k * 180 / AMOUNT) : (450 - k * 180 / AMOUNT); //emitting angle in angular measure
                        bullet.emplace_back(imageBullet, bx, by, vx, vy, wRate, hRate, ang, false);
                    }
                    ite->bulTime = duration; //update the last emitting time
                    break;
                }
                case 2:{ //gradually change its direction to player
                    Enemy &ene2 = *ite;
                    double ang = std::atan2(ene2.vel.y, ene2.vel.x), angDir = std::atan2((player.pos.y - ene2.pos.y), (player.pos.x - ene2.pos.x));
                    //maintain the angle to [0, 2 * PI)
                    if (ang < 0) ang += 2 * PI;
                    if (angDir < 0) angDir += 2 * PI;
                    //rotate, always choose the direction that the differing angle is less than PI
                    //very annoying judgment, maybe some cases can be merged
                    if (angDir > ang){
                        if (angDir - ang <= PI){
                            ang += PI / 20;
                            if (ang > angDir) ang = angDir;
                        }
                        else{
                            //std::cout << "PREANG = " << ang / PI * 180 << std::endl;
                            ang -= PI / 20;
                            if (ang < 0){
                                ang += 2 * PI;
                                if (ang < angDir) ang = angDir;
                            }
                            //std::cout << "ANGDIR = " << angDir / PI * 180 << std::endl;
                            //std::cout << "NOWANG = " << ang / PI * 180 << std::endl;
                            std::cout << std::endl;
                        }
                    }
                    else{
                        if (ang - angDir <= PI){
                            ang -= PI / 20;
                            if (ang < angDir) ang = angDir;
                        }
                        else{
                            ang += PI / 20;
                            if (ang > 2 * PI){
                                ang -= 2 * PI;
                                if (ang > angDir) ang = angDir;
                            }
                        }
                    }

                    double vx = abs(15 * cos(ang)), vy = abs(15 * sin(ang)); //velocity in x and y
                    if (PI / 2 < ang && ang < PI / 2 * 3) vx = -vx;
                    if (PI < ang && ang < 2 * PI) vy = -vy;
                    ang = abs(ang / PI * 180); //transform into angular measure
                    ang -= 90.0;
                    if (ang < 0) ang += 360; //rotating angle
                    ene2.vel.x = vx;
                    ene2.vel.y = vy; //position in x and y
                    ene2.imgAngle = ang;
                    ite->bulTime = duration;
                    //std::cout << "CHANGE ETYPE2" << std::endl;
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
                    if (duration < 20) break;
                    enemy.emplace_back(imageEnemy, r, he / 2, 1);
                    Enemy &ene1 = enemy[enemy.size() - 1];
                    ene1.wRate = 1.0;
                    ene1.hRate = 1.0;
                    ene1.colR = hypot(ene1.imgw / 3 * ene1.wRate, ene1.imgh / 3 * ene1.hRate);
                    if (outOfScreen(ene1))
                        maintainToScreen(ene1);
                    lastEnemyTime[1] = duration;
                    break;
                }
                case 2:{
                    if (duration < 10) break;
                    enemy.emplace_back(imageEnemy, r, he / 2 * 0.3, 2);
                    Enemy &ene2 = enemy[enemy.size() - 1];
                    double ang = std::atan2(player.pos.y - ene2.pos.y, player.pos.x - ene2.pos.x);
                    ene2.vel.x = 15 * std::cos(ang);
                    ene2.vel.y = 15 * std::sin(ang); //initial velocity
                    ene2.wRate = 0.3;
                    ene2.hRate = 0.3;
                    ene2.colR = hypot(ene2.imgw * ene2.wRate, ene2.imgh * ene2.hRate);
                    if (outOfScreen(ene2))
                        maintainToScreen(ene2);
                    lastEnemyTime[2] = duration;
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

void dealWithBonus(){

    auto itn = bonus.begin();
    while (itn != bonus.end()){
        itn->pos.y += itn->speed;
        itn->speed += itn->deltaSpeed;
        if (itn->speed > itn->maxSpeed) itn->speed = itn->maxSpeed;
        if (outOfScreen(*itn))
            bonus.erase(itn); //out of screen
        if (itn == bonus.end()) break;
        else ++itn;
    }
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
        if (ite->HP > 0){
            if (isCollide(*ite, player)){
                ite->HP = 0;
                #ifndef DEBUG_MODE
                --player.HP;
                if (soulAttack < 3) ++soulAttack;
                player.isCol = true;
                #endif // DEBUG_MODE
            }
        }
        ++ite;
    }

    //player with bonus
    auto itn = bonus.begin();
    while (itn != bonus.end()){
        if (isCollide(*itn, player)){
            switch(itn->bType){
                case 0: //add 500 scores
                    score += 500;
                    break;
                case 1: //add 1000 scores
                    score += 1000;
                    break;
                case 2: //add speedAttack
                    if (ISA_Time < 1e-6) player.speedAttack /= 2;
                    ISA_Time = duration;
                    break;
                case 3: //add a bomb
                    ++soulAttack;
                    break;
                case 4: //add a HP
                    ++player.HP;
                    break;
                default:
                    break;
            }
            bonus.erase(itn);
            continue;
        }
        else ++itn;
    }
}

bool dealWithEnd(){
    enemy.clear();
    bullet.clear();
    bonus.clear();
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
