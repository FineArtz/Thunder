#include "SDL2_header.h"

#include <cstdio>
#include <map>



const int Game::SCREEN_WIDTH	= 720;
const int Game::SCREEN_HEIGHT	= 480;
const std::string Game::TitleName = "A Simple Game Demo";

using namespace Game;

std::map<int, bool> keyboard;



PointD posPlayer, velocityPlayer;
PointD posEnemy[10];
int enemyNumber, imageNumber;
double speedPlayer;

Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];

void loadPictures()
{
	imagePlayer = loadImage( "player.png"	);
	imageBullet = loadImage( "bullet.png"	);
	imageEnemy	= loadImage( "player_u.png" );
}

void initialize()
{
	//Display FPS
	FPS_DISPLAY = true;

	//Initialize vairables
	posPlayer = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
	posEnemy[0] = posPlayer;
	enemyNumber = 1;
	speedPlayer = 5;
	canvasColor = {0, 0, 0, 255};

	//Load pictures from files
	loadPictures();
}

void drawPlayer()
{
	int w,h;
	getImageSize( imagePlayer, w, h );
	drawImage( imagePlayer, posPlayer.x-w/2, posPlayer.y-h/2 );
}
void drawBackground()
{
	Rect rect = {70, 50, 80, 90};

	//	Pay attention:
	//		(Color){255,255,0} means (Color){255,255,0,0}
	//		and means you will draw nothing
	setPenColor((Color){255, 255, 0, 255});


	drawRect( rect, true );

}
void drawHint()
{
	Image *text = textToImage( "< This is a simple game demo. >" );
	int w,h;
	getImageSize( text, w, h );
	drawImage( text, SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT-h );
}
void drawBullet()
{

}
void drawEnemy()
{
	int w,h;
	getImageSize( imageEnemy, w, h );
	drawImage( imageEnemy, posEnemy[0].x-w/2, posEnemy[0].y-h/2 );
}

void draw()
{
	drawBackground();
	drawPlayer();
	drawBullet();
	drawEnemy();
	drawHint();
}
void deal()
{
	bool move = false;
	//Calculate velocity
	//Caution! x is horizontal and y is vertical.
	if( keyboard[KEY_UP]	|| keyboard['w'] )
	{
		velocityPlayer = velocityPlayer + PointD(0,-1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_DOWN]	|| keyboard['s'] )
	{
		velocityPlayer = velocityPlayer + PointD(0,+1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_LEFT]	|| keyboard['a'] )
	{
		velocityPlayer = velocityPlayer + PointD(-1,0)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_RIGHT] || keyboard['d'] )
	{
		velocityPlayer = velocityPlayer + PointD(+1,0)*speedPlayer;
		move = true;
	}

	//Limit player's speed
	double len = velocityPlayer.length();
	if( len > speedPlayer )
	{
		velocityPlayer = velocityPlayer/len*speedPlayer;
	}

	//Calculate new position
	posPlayer = posPlayer + velocityPlayer;

	//Stop player
	if(!move)
	{
		velocityPlayer = velocityPlayer * 0.8;
		if( velocityPlayer.length() < 0.1 )
			velocityPlayer = PointD();
	}
}

int work( bool &quit )
{
	//Calculate sth.
	deal();

	//Draw on the screen
	draw();

	if( keyboard[KEY_ESC] )
		quit = true;
	return 0;
}

void mousePress()
{
}

void mouseMove()
{

}

void mouseRelease()
{

}

void keyDown()
{
	keyboard[keyValue] = true;
}

void keyUp()
{
	keyboard[keyValue] = false;
}

void finale()
{
	//Delete all images
	cleanup( imagePlayer, imageBullet, imageEnemy );
	for( int i = 0; i < imageNumber; ++i )
		cleanup( images[i] );
}

