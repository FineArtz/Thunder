#include "draw.h"
#include "others.h"


using namespace Game;

extern Player player;
extern std::vector<Enemy> enemy;
extern std::vector<Bullet> bullet;

extern Image *images[100];

extern int soulAttack;
extern int score, highestScore;

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

void drawGameInfo(){
    double upperBound = SCREEN_HEIGHT / 4;
    std::string gInfo = "HP";
    Color RED = {255, 5, 5, 255};
    Image *text = textToImage(gInfo, fontSize, RED);
    int w, h;
    getImageSize(text, w, h);
    double rate1 = 1.5, rate2 = 0.2;
    drawImageC(text, PointD(PLAY_WIDTH + w / 2 * rate1 + 5, upperBound + h / 2 * rate1), rate1, rate1);
    cleanup(text);
    for (int i = 0; i < player.HP; ++i){
        drawImageC(images[0], PointD(PLAY_WIDTH + w * rate1 + 5 + 50 * rate2 + i * 130 * rate2, upperBound + h / 2 * rate1), rate2, rate2);
    }

    upperBound = upperBound + h * rate1 + 20;
    gInfo = "Bomb";
    Color ORANGE = {255, 255, 5, 255};
    text = textToImage(gInfo, fontSize, ORANGE);
    getImageSize(text, w, h);
    drawImageC(text, PointD(PLAY_WIDTH + w / 2 * rate1 + 5, upperBound + h / 2 * rate1), rate1, rate1);
    cleanup(text);
    for (int i = 0; i < soulAttack; ++i){
        drawImageC(images[1], PointD(PLAY_WIDTH + w * rate1 + 5 + 50 * rate2 + i * 130 * rate2, upperBound + h / 2 * rate1), rate2, rate2);
    }

    upperBound = upperBound + h * rate1 + 20;
    gInfo = "SCORE ";
    gInfo += toString(score);
    Color GREEN = {5, 255, 5, 255};
    text = textToImage(gInfo, fontSize, GREEN);
    getImageSize(text, w, h);
    drawImageC(text, PointD(PLAY_WIDTH + w / 2 * rate1 + 5, upperBound + h / 2 * rate1), rate1, rate1);
    cleanup(text);

    upperBound = upperBound + h * rate1 + 20;
    gInfo = "HIGHEST SCORE ";
    gInfo += toString(highestScore);
    Color BLUE = {5, 5, 255, 255};
    text = textToImage(gInfo, fontSize, BLUE);
    getImageSize(text, w, h);
    rate1 = 1.2;
    drawImageC(text, PointD(PLAY_WIDTH + w / 2 * rate1 + 5, upperBound + h / 2 * rate1), rate1, rate1);
    cleanup(text);
    rate1 = 1.8;

    upperBound = upperBound + h * rate1 + 20;
    gInfo = "TIME ";
    gInfo += toString((int)duration);
    Color PURPLE = {255, 5, 255, 255};
    text = textToImage(gInfo, fontSize, PURPLE);
    getImageSize(text, w, h);
    rate1 = 1.2;
    drawImageC(text, PointD(PLAY_WIDTH + w / 2 * rate1 + 5, upperBound + h / 2 * rate1), rate1, rate1);
    cleanup(text);
}

void drawBackground(){
    //ToDo
}
void drawForeground(){
    #ifdef DEBUG_MODE
    drawDebugInfo();
    #endif // DEBUG_MODE
    Rect sepLine = {PLAY_WIDTH, 1, 3, PLAY_HEIGHT}; //the separating line between playing part and info part
    setPenColor(255, 255, 255, 255); //white
    drawRect(sepLine, true);

    drawGameInfo();
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

bool drawBurstAnime(int x, double startTime){
    int w, h;
    getImageSize(images[x], w, h);
    w /= 24;
    int nowFrame = (duration - startTime) * 20;
    if (nowFrame < 24){
        Rect partR = {w * nowFrame, 0, w, h};
        drawImage(images[x], player.pos.x - w * player.wRate * 2, player.pos.y - h * player.hRate * 2,
                  player.wRate * 4, player.hRate * 4, 0, NULL, FLIP_NONE, &partR);
        return true;
    }
    return false;
}
