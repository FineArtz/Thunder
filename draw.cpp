#include "draw.h"
#include "others.h"

#include <vector>
#include <stdexcept>
#include <iostream>

using namespace Game;

extern Player player;
extern std::vector<Enemy> enemy;
extern std::vector<Bullet> bullet;


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
void drawImageC(Image *img, PointD p,
                const double &widthRate = 1, const double &heightRate = 1,
                const double &angle = 0, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr){
    drawImageC(img, p.x, p.y, widthRate, heightRate, angle, center, flip, clip);
}
void drawImageC(const item &it, const Point *center = NULL,
                const FlipType &flip = FLIP_NONE, const Rect *clip = nullptr){
    drawImageC(it.img, it.pos.x, it.pos.y, it.wRate, it.hRate, it.imgAngle, center, flip, clip);
}

void drawDebugInfo(){
    std::string dInfo = "duration = ";
    dInfo += toString(trunc(duration));
	Image *text = textToImage(dInfo);
    int w, h;
    getImageSize(text, w, h);
    drawImageC(text, PointD(PLAY_WIDTH + w * 0.5 * 0.8 + 5, h *0.5 * 0.8), 0.8, 0.8); //right top
    cleanup(text);
    //use PointD as argument to avoid ambiguous call

	dInfo = "MouseX = ";
    dInfo += toString(mouseX);
    dInfo += ", MouseY = " + toString(mouseY);
    text = textToImage(dInfo);
    getImageSize(text, w, h);
    drawImageC(text, PointD(PLAY_WIDTH + w * 0.5 * 0.8 + 5, h * 1.5 * 0.8), 0.8, 0.8); //right top
    cleanup(text);

    dInfo = "PlayerX = ";
    dInfo += toString(trunc(player.pos.x));
    dInfo += ", PlayerY = " + toString(trunc(player.pos.y));
    text = textToImage(dInfo);
    getImageSize(text, w, h);
    drawImageC(text, PointD(PLAY_WIDTH + w * 0.5 * 0.8 + 5, h * 2.5 * 0.8), 0.8, 0.8); //right top
    cleanup(text);
}

void drawBackground(){
    //ToDo
}
void drawForeground(){
    #ifdef DEBUG_MODE
    drawDebugInfo();
    #endif // DEBUG_MODE
    Rect sepLine = {PLAY_WIDTH, 1, 3, PLAY_HEIGHT};
    setPenColor(255, 255, 255, 255);
    drawRect(sepLine, true);
}
void drawPlayer(){
	setImageAlpha(player.img, 150);
    drawImageC(player);
}
void drawEnemy(){
    for (auto ene : enemy)
        drawImageC(ene, NULL, FLIP_VERTICAL);
}
void drawBullet(){
    for (Bullet bul : bullet){
        if (bul.isPlayer) drawImageC(bul);
        else drawImageC(bul.img, bul.pos, bul.wRate, bul.hRate, bul.imgAngle, NULL, FLIP_VERTICAL);
    }
}

extern double endTime;
bool drawEndAnime(){
    Image *endAnime = loadImage("red_strip24.png");
    int w, h;
    getImageSize(endAnime, w, h);
    w /= 24;
    int nowFrame = (duration - endTime) * 20;
    if (nowFrame < 24){
        Rect partR = {w*nowFrame, 0, w, h};
        drawImage(endAnime, player.pos.x, player.pos.y, player.wRate * 2, player.hRate * 2, 0, NULL, FLIP_NONE, &partR);
        cleanup(endAnime);
        return true;
    }
    cleanup(endAnime);
    return false;
}
