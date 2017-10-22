#include "SDL2_header.h"

#include <cstdio>
#include <map>
#include <vector>
#include <deque>
#include <iomanip>
#include <stdexcept>

#define DEBUG_MODE

const std::string Game::TitleName = "Rednuht 0.1";
const int Game::SCREEN_WIDTH	= 640;
const int Game::SCREEN_HEIGHT	= 780;

using namespace Game;

std::map<int, bool> keyboard;

std::vector<Enemy> enemy;
std::deque<Bullet> bullet;
Player player;

//double wRate = 1.0, hRate = 1.0;

Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];

void loadPictures(){
	imagePlayer = loadImage( "player.png" );
	imageBullet = loadImage( "bullet.png" );
	imageEnemy	= loadImage( "enemy.png"  );
}

void initialize(){
    #ifdef DEBUG_MODE
	FPS_DISPLAY = true;
	#endif // DEBUG_MODE

	player.pos = PointD(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    player.vel = PointD(5, 5);
    player.speedAttack = 0.1;
    player.wRate = 0.5;
    player.hRate = 0.5;

	canvasColor = {0, 0, 0, 255};

	loadPictures();

	player.img = imagePlayer;
}

//draw image at its center point
void drawImageC(Image *img, int x, int y,
                const double &widthRate = 1, const double &heightRate = 1,
                const double &angle = 0, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr){
    int w, h;
    getImageSize(img, w, h);
    int leftx = x - w / 2 * widthRate;
    int topy = y - h / 2 * heightRate;
    drawImage(img, leftx, topy, widthRate, heightRate, angle, center, flip, clip);
}
inline void drawImageC(Image *img, PointD p,
                const double &widthRate = 1, const double &heightRate = 1,
                const double &angle = 0, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr){
    drawImageC(img, p.x, p.y, widthRate, heightRate, angle, center, flip, clip);
}
inline void drawImageC(const item &it,
                const double &angle = 0, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr){
    drawImageC(it.img, it.pos.x, it.pos.y, it.wRate, it.hRate, angle, center, flip, clip);
}

void drawDebugInfo(){
    std::string dInfo = "duration = ";
    dInfo += toString(trunc(duration));
	Image *text = textToImage(dInfo);
    int w, h;
    getImageSize(text, w, h);
    drawImageC(text, PointD(SCREEN_WIDTH - w * 0.5 * 0.8, h *0.5 * 0.8), 0.8, 0.8); //right top
    cleanup(text);
    //use PointD as argument to avoid ambiguous call

	dInfo = "MouseX = ";
    dInfo += toString(mouseX);
    dInfo += ", MouseY = " + toString(mouseY);
    text = textToImage(dInfo);
    getImageSize(text, w, h);
    drawImageC(text, PointD(SCREEN_WIDTH - w * 0.5 * 0.8, h * 1.5 * 0.8), 0.8, 0.8); //right top
    cleanup(text);

    dInfo = "PlayerX = ";
    dInfo += toString(trunc(player.pos.x));
    dInfo += ", PlayerY = " + toString(trunc(player.pos.y));
    text = textToImage(dInfo);
    getImageSize(text, w, h);
    drawImageC(text, PointD(SCREEN_WIDTH - w * 0.5 * 0.8, h * 2.5 * 0.8), 0.8, 0.8); //right top
    cleanup(text);
}
void drawBackground(){
	Rect rect = {70, 50, 80, 90};
	setPenColor((Color){255, 255, 0, 255});

	//	Pay attention: (Color){255,255,0} means (Color){255,255,0,0}
	//	and means you will draw nothing

	drawRect(rect, true);
}
void drawForeground(){
    #ifdef DEBUG_MODE
    drawDebugInfo();
    #endif // DEBUG_MODE
}
void drawPlayer(){
	setImageAlpha(imagePlayer, 150);
    drawImageC(player);
}
inline void drawEnemy(){
    for (auto ene : enemy) drawImageC(ene);
}
inline void drawBullet(){
    for (Bullet bul : bullet){
        if (bul.isPlayer) drawImageC(bul);
        else drawImageC(bul.img, bul.pos, bul.wRate, bul.hRate, 0, 0, FLIP_VERTICAL);
    }
}

void draw()
{
	drawBackground();
	drawPlayer();
	drawEnemy();
	drawBullet();
	drawForeground();
}

//Deal with Events
void dealWithPlayer(){
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
}
void dealWithEnemy(){

}//ToDO

double lastBulletTime = 0.0;
void dealWithBullet(){
    int wp, hp, wb, hb; //p=player, b=bullet
    getImageSize(imageBullet, wb, hb);
    auto itb = bullet.begin();
    while (itb != bullet.end()){
        itb->pos = itb->pos - itb->vel;
        if (outOfScreen(*itb))
            bullet.erase(itb); //out of screen
        if (itb == bullet.end()) break;
        else ++itb;
    }
    //if (keyboard['z']) std::cout << "Z" << std::endl;

    if (keyboard[KEY_ENTER] || keyboard['z']){ //create a new bullet
        if (duration - lastBulletTime >= player.speedAttack){
            getImageSize(imagePlayer, wp, hp);
            //PointD newBullet(posPlayer.x, posPlayer.y - hp / 2 - hb / 2);
            bullet.emplace_back(imageBullet, player.pos.x, player.pos.y - (hp / 2 - hb) * player.hRate);
            bullet[bullet.size() - 1].wRate = player.wRate;
            bullet[bullet.size() - 1].hRate = player.hRate;
            lastBulletTime = duration;
        }
    }
}

//Reactions to Mouse
//ToDo
void mousePress(){}
void mouseMove(){}
void mouseRelease(){}

//Reactions to Keyboard
inline void keyDown(){
	keyboard[keyValue] = true;
}

inline void keyUp(){
	keyboard[keyValue] = false;
}

//Main
int work(bool &quit){
	dealWithPlayer();
	dealWithBullet();
	dealWithEnemy();

	draw();

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
