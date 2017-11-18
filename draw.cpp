#include "draw.h"

using namespace Game;

extern Player player;
extern std::vector<Enemy> enemy;
extern std::vector<Bullet> bullet;
extern std::vector<Bonus> bonus;

extern Image *images[100];

extern int soulAttack;
extern int score, highestScore;
extern double endTime;
extern bool gameOver;

//draw image at its center point
void drawImageC(Image *img, int x, int y,
                const double &widthRate, const double &heightRate,
                const double &angle, const Point *center,
                const FlipType &flip, const Rect *clip){
    int w, h;
    getImageSize(img, w, h);
    int leftx = x - w / 2 * widthRate;
    int topy = y - h / 2 * heightRate;
    drawImage(img, leftx, topy, widthRate, heightRate, angle, center, flip, clip);
}
void drawImageC(Image *img, PointD p,
                const double &widthRate, const double &heightRate,
                const double &angle, const Point *center,
                const FlipType &flip, const Rect *clip){
    drawImageC(img, p.x, p.y, widthRate, heightRate, angle, center, flip, clip);
}
void drawImageC(const item &it, const Point *center,
                const FlipType &flip, const Rect *clip){
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
    if (!gameOver)
        gInfo += toString((int)duration);
    else
        gInfo += toString((int)endTime);
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
    for (Enemy ene : enemy)
        if (ene.eraseTime == 0)
            drawImageC(ene, NULL, FLIP_VERTICAL);
}
void drawBullet(){
    for (Bullet bul : bullet){
        if (bul.isPlayer) drawImageC(bul);
        else drawImageC(bul.img, bul.pos, bul.wRate, bul.hRate, bul.imgAngle, NULL, FLIP_VERTICAL);
    }
}
void drawBonus(){
    for (Bonus bon : bonus)
        drawImageC(bon);
}

bool drawBurstAnime(int x, const item &i){
    int w, h;
    getImageSize(images[x], w, h);
    w /= 24;
    int nowFrame = (duration - i.eraseTime) * 20;
    if (nowFrame < 24){
        Rect partR = {w * nowFrame, 0, w, h};
        drawImage(images[x], i.pos.x - w * i.wRate * 2, i.pos.y - h * i.hRate * 2,
                  i.wRate * 4, i.hRate * 4, 0, NULL, FLIP_NONE, &partR);
        return true;
    }
    return false;
}

void drawEndInfo(){
    std::string eInfo = "Time: ";
    eInfo += toString((int)player.eraseTime);
    Image *text = textToImage(eInfo);
    int w, h;
    getImageSize(text, w, h);
    drawImageC(text, PointD(PLAY_WIDTH / 2, PLAY_HEIGHT / 2 - 4 * h), 2.0, 2.0);
    cleanup(text);

    eInfo = "Score: ";
    eInfo += toString(score);
    if (score < highestScore){
        text = textToImage(eInfo);
    }
    else{
        Color RED = {255, 5, 5, 255};
        text = textToImage(eInfo, fontSize, RED);
    }
    getImageSize(text, w, h);
    drawImageC(text, PointD(PLAY_WIDTH / 2, PLAY_HEIGHT / 2), 2.0, 2.0);
    cleanup(text);

    eInfo = "Highest Score: ";
    eInfo += toString(highestScore);
    text = textToImage(eInfo);
    getImageSize(text, w, h);
    drawImageC(text, PointD(PLAY_WIDTH / 2, PLAY_HEIGHT / 2 + 4 * h), 2.0, 2.0);
    cleanup(text);
}
