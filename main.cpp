#include "SDL2_header.h"
#include "item.h"
#include "others.h"
#include "draw.h"
#include "deal.h"

#include <cstdio>
#include <map>
#include <vector>
#include <deque>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

const std::string Game::TitleName = "Rednuht 0.1";
const int Game::SCREEN_WIDTH	= 1000;
const int Game::PLAY_WIDTH      = 600;
const int Game::SCREEN_HEIGHT	= 780;
const int Game::PLAY_HEIGHT     = 780;

using namespace Game;

std::map<int, bool> keyboard;

std::vector<Enemy> enemy;
std::vector<Bullet> bullet;
Player player;

bool gameOver = false;

int soulAttack ;

int score = 0, highestScore;
double endTime;

Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];

void loadPictures(){
	imagePlayer = loadImage("player.png");
	imageBullet = loadImage("bullet.png");
	imageEnemy	= loadImage("player_u.png");

	images[0] = loadImage("HP.png");
	images[1] = loadImage("Bomb.png");
	images[2] = loadImage("red_strip24.png");
	images[3] = loadImage("poison_strip24.png");
}
void setPlayer(){
    player.vel = PointD(5, 5);
    player.speedAttack = 0.3;
    player.wRate = 0.5;
    player.hRate = 0.5;
    player.img = imagePlayer;
	getImageSize(player.img, player.imgh, player.imgw);
	player.colR = hypot(player.imgw * player.wRate / 3, player.imgh * player.hRate / 3);
	player.pos = PointD(PLAY_WIDTH / 2, PLAY_HEIGHT - player.imgh / 2 * player.hRate);
}
void initialize(){
    #ifdef DEBUG_MODE
	FPS_DISPLAY = true;
	#endif // DEBUG_MODE

	canvasColor = {0, 0, 0, 255};
	loadPictures();
    setPlayer();
    soulAttack = 3;

    std::ifstream ifs("bin\\Debug\\hsset.dll");
    ifs >> highestScore;
    ifs.close();

    std::srand(std::time(0));
}

void draw()
{
	drawBackground();
	if (!gameOver) drawPlayer();
	drawEnemy();
	drawBullet();
	drawForeground();
}

void dealWithEvent(){
    int alive = dealWithPlayer();
    if (alive == -1){
        player.eraseTime = endTime = duration;
        gameOver = true;
        return;
    }
    dealWithEnemy();
    dealWithBullet();
    dealWithCollision();
}

//Reactions to Mouse
//ToDo
void mousePress(){}
void mouseMove(){}
void mouseRelease(){}

//Reactions to Keyboard
void keyDown(){
	keyboard[keyValue] = true;
}
void keyUp(){
	keyboard[keyValue] = false;
}

//Main
int work(bool &quit){
    if (!gameOver){
        dealWithEvent();
    }
    draw();
    if (gameOver){
        bool isEnd = dealWithEnd();
        if (isEnd){
            if (score + (int)player.eraseTime > highestScore){
                std::ofstream ofs("bin\\Debug\\hsset.dll");
                ofs << score + (int)player.eraseTime << std::endl;
                ofs.close();
            }
            drawEndInfo();
        }
    }

	if(keyboard[KEY_ESC])
		quit = true;
	return 0;
}

//Final
void finale()
{
	cleanup(imagePlayer, imageBullet, imageEnemy);
	for(int i = 0; i < 100; ++i)
		cleanup(images[i]);
}
