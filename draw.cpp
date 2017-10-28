#include "SDL2_header.h"
#include "item.h"
#include "others.h"
using namespace Game;

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
    for (auto ene : enemy) drawImageC(ene, 0, 0, FLIP_VERTICAL);
}
inline void drawBullet(){
    for (Bullet bul : bullet){
        if (bul.isPlayer) drawImageC(bul);
        else drawImageC(bul.img, bul.pos, bul.wRate, bul.hRate, 0, 0, FLIP_VERTICAL);
    }
}
