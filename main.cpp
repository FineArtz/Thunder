#include "SDL2_header.h"

#include <cstdio>
#include <map>
#include <vector>
#include <deque>
#include <iomanip>
#include <stdexcept>

const std::string Game::TitleName = "Rednuht 0.1";
const int Game::SCREEN_WIDTH	= 640;
const int Game::SCREEN_HEIGHT	= 800;

using namespace Game;

std::map<int, bool> keyboard;

PointD posPlayer, velocityPlayer;
PointD posEnemy[10];
std::deque<PointD> bullet;
double speedPlayer, speedBullet;
double speedAttack;

Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];

double wRate = 1.0, hRate = 1.0;

void loadPictures(){
	imagePlayer = loadImage( "player.png" );
	imageBullet = loadImage( "bullet.png" );
	imageEnemy	= loadImage( "enemy.png"  );
}

void initialize(){
	FPS_DISPLAY = true;

	posPlayer = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
	posEnemy[0] = posPlayer;
	speedPlayer = 5;
	speedBullet = 20;
	speedAttack = 0.1;
	canvasColor = {0, 0, 0, 255};

	loadPictures();
}

void drawImageC(Image *img, int x, int y,
                const double &widthRate = 1, const double &heightRate = 1,
                const double &angle = 0, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr){ //now x and y is real center point
    int w, h;
    getImageSize(img, w, h);
    int leftx = x - w / 2 * widthRate;
    int topy = y - h / 2 * heightRate;
    drawImage(img, leftx, topy, widthRate, heightRate, angle, center, flip, clip);
}

void drawPlayer(){
	int w,h;
	getImageSize(imagePlayer, w, h);
	setImageAlpha(imagePlayer, 150);
	wRate = 0.5;
	hRate = 0.5;
	//drawImageC(imagePlayer, posPlayer.x, posPlayer.y);
    drawImageC(imagePlayer, posPlayer.x, posPlayer.y, wRate, hRate);
}
void drawBackground(){
	Rect rect = {70, 50, 80, 90};
	setPenColor((Color){255, 255, 0, 255});

	//	Pay attention: (Color){255,255,0} means (Color){255,255,0,0}
	//	and means you will draw nothing

	drawRect( rect, true );

}
void drawForeground(){
	Rect rect = {200, 176, 85, 100};
	setPenColor((Color){0, 0, 255, 200});

	drawRect( rect );
}
void drawHint(){
    std::string te = "duration = ";
    te += toString(trunc(duration));
	Image *text = textToImage(te);
	int w,h;
	getImageSize(text, w, h);
	drawImageC(text, SCREEN_WIDTH - h / 2, SCREEN_HEIGHT / 2, 1, 1, 90+180);
}
void drawMouse(){
    std::string posMouse = "X = ";
    posMouse += toString(mouseX);
    posMouse += ", Y = " + toString(mouseY);
    Image *text = textToImage(posMouse);
    int w, h;
    getImageSize(text, w, h);
    drawImageC(text, SCREEN_WIDTH - w / 2, h / 2);
}
/*void drawCenter(){
    //Color curColor = canvasColor;
    setPenColor(255, 0, 255, 255);
    drawPoint(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    //canvasColor = curColor;
}*/
void drawBullet(const PointD &bul){
    drawImageC(imageBullet, bul.x, bul.y, wRate, hRate);
}
void drawBullet(){
    for (auto itb : bullet){
        drawImageC(imageBullet, itb.x, itb.y, wRate, hRate);
        //drawImageC(imageBullet, itb.x, itb.y);
    }
}
/*int lastAnime = 0;
void drawEnemy()
{
	int w,h;
	getImageSize( imageEnemy, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha( imageEnemy, 255 );
	drawImage( imageEnemy, posEnemy[0].x, posEnemy[0].y, 2, 2, 0, NULL, FLIP_NONE, &clip );
}*/
void drawEnemy(){}

void draw()
{
	drawBackground();
	drawMouse();
	drawPlayer();
	drawBullet();
	drawEnemy();
	drawForeground();
	drawHint();
	//drawCenter();
}

//Deal with Events
void dealWithPlayer(){
	bool move = false;
	if(keyboard[KEY_UP]	|| keyboard['w']){ //move forward
		velocityPlayer = velocityPlayer + PointD(0,-1) * speedPlayer;
		move = true;
	}
	if(keyboard[KEY_DOWN] || keyboard['s']){ //move backward
		velocityPlayer = velocityPlayer + PointD(0,+1) * speedPlayer;
		move = true;
	}
	if(keyboard[KEY_LEFT] || keyboard['a']){ //move leftward
		velocityPlayer = velocityPlayer + PointD(-1,0) * speedPlayer;
		move = true;
	}
	if(keyboard[KEY_RIGHT] || keyboard['d']){ //move rightward
		velocityPlayer = velocityPlayer + PointD(+1,0) * speedPlayer;
		move = true;
	}

	double len = velocityPlayer.length();
	if(len > speedPlayer){ //move to fast!
		velocityPlayer = velocityPlayer / len * speedPlayer;
	}
	posPlayer = posPlayer + velocityPlayer;

	if(!move){ //not move, velocity gradually lows
		velocityPlayer = velocityPlayer * 0.8; //speed of lowness
		if(velocityPlayer.length() < 0.1)
			velocityPlayer = PointD(); //stop
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
        itb->y -= speedBullet;
        if (itb->y - hb / 2 <= 0)
            bullet.erase(itb); //out of screen
        if (itb == bullet.end()) break;
        else ++itb;
    }
    //if (keyboard['z']) std::cout << "Z" << std::endl;

    if (keyboard[KEY_ENTER] || keyboard['z']){ //create a new bullet
        if (duration - lastBulletTime >= speedAttack){
            getImageSize(imagePlayer, wp, hp);
            //PointD newBullet(posPlayer.x, posPlayer.y - hp / 2 - hb / 2);
            bullet.emplace_back(posPlayer.x, posPlayer.y - (hp / 2 - hb) * hRate);
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
