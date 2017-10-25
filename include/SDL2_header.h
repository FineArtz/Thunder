/*
 * Coded by Jingxiao Chen.
 *
 * Part of this code came from https://github.com/Twinklebear/TwinklebearDev-Lessons
 *
 */

#ifndef SDL2_HEADER_H
#define SDL2_HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <string>

#include "cleanup.h"
#include "res_path.h"
#include "pointd.h"

namespace Game {


#define Image		SDL_Texture
#define Color		SDL_Color
#define Rect		SDL_Rect
#define Point		SDL_Point
#define FlipType	SDL_RendererFlip

#define FLIP_NONE		SDL_FLIP_NONE			/**< Do not flip */
#define FLIP_HORIZONTAL SDL_FLIP_HORIZONTAL		/**< flip horizontally */
#define FLIP_VERTICAL	SDL_FLIP_VERTICAL		/**< flip vertically */

#define KEY_SPACE	SDLK_SPACE
#define KEY_ENTER	SDLK_RETURN
#define KEY_RETURN	SDLK_RETURN
#define KEY_TAB		SDLK_TAB
#define KEY_ESC		SDLK_ESCAPE
#define KEY_UP		SDLK_UP
#define KEY_DOWN	SDLK_DOWN
#define KEY_LEFT	SDLK_LEFT
#define KEY_RIGHT	SDLK_RIGHT
#define KEY_LCTRL	SDLK_LCTRL
#define KEY_LSHIFT	SDLK_LSHIFT
#define KEY_LALT	SDLK_LALT
#define KEY_RCTRL	SDLK_RCTRL
#define KEY_RALT	SDLK_RSHIFT
#define KEY_RSHIFT	SDLK_RALT


#define MOUSE_LEFT   SDL_BUTTON_LEFT
#define MOUSE_MIDDLE SDL_BUTTON_MIDDLE
#define MOUSE_RIGHT  SDL_BUTTON_RIGHT

extern SDL_Renderer	*renderer;
extern SDL_Window	*window;

extern bool FPS_DISPLAY;
extern double nowFPS;
extern double duration;
extern uint64_t duration_i;
extern Color canvasColor;
extern bool mousePressed;
extern double duration;
extern uint8_t mouseButton;
extern int mouseX;
extern int mouseY;
extern bool keyPressed;
extern int keyValue;

extern const unsigned int FPS_RATE;
extern const std::string RES_PATH_IMG;
extern const std::string RES_PATH_FONT;


//Screen attributes
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const std::string TitleName;

extern std::string	fontName;
extern int			fontSize;

extern uint8_t lastColor[4];

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg);
/*
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
/*
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param dst The destination rectangle to render the texture too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
				   const double &angle = 0, const SDL_Point *center = NULL,
				   const SDL_RendererFlip &flip = SDL_FLIP_NONE,
				   const SDL_Rect *clip = nullptr);
/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
				   const double &widthRate = 1,	const double &heightRate = 1,
				   const double &angle = 0, const SDL_Point *center = NULL,
				   const SDL_RendererFlip &flip = SDL_FLIP_NONE,
				   const SDL_Rect *clip = nullptr);
/*
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param fontFile The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color,
		int fontSize, SDL_Renderer *renderer);

// ----------- Functions for users ---------------


Image* textToImage( const std::string &msg,
					const int32_t &size = fontSize, const Color &color = {255, 255, 255, 255},
					const std::string &fontType = fontName);


/*
 * Loads an image
 * @param file The image file to load
 * @return the loaded image, or nullptr if something went wrong.
 */
Image* loadImage( const std::string &file );

void setImageAlpha( Image *img, Uint8 alpha );

/*
 * Draw image on the screen.
 * @param img The image we want to display
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * \param angle		An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
 * \param center	A pointer to a point indicating the point around which dstrect will be rotated
 *					(if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
 * \param flip		An SDL_RendererFlip value stating which flipping actions should be performed on the texture
 * @param clip		The subsection of the picture to display.
 * @return NULL
 */
void drawImage(Image *img, int x, int y,
			   const double &widthRate = 1,	const double &heightRate = 1,
			   const double &angle = 0, const Point *center = NULL,
			   const FlipType &flip = FLIP_NONE,
			   const Rect *clip = nullptr);

void getImageSize( Image *img, int &width, int &height );



void setPenColor( const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a);
void setPenColor( const Color &color );

void drawPoint( int x, int y );
void drawPoint( const Point& p );

void drawLine( int x1, int y1, int x2, int y2 );
void drawLine( const Point &p1 ,const Point &p2 );

void drawLines(const SDL_Point* points, int count);

void drawRect( const Rect& rect, const bool& fill = false );



void setCanvas( int x, int y, int width=SCREEN_WIDTH, int height=SCREEN_HEIGHT );

// ---------------------------------------------------

void drawText( const std::string &msg, const int &x, const int &y,
			   const int32_t &size = fontSize, const Color &color = { 255, 255, 255, 255 } );

}


// Functions for programming
extern void initialize();
extern int	work( bool & );
extern void mousePress();
extern void mouseMove();
extern void mouseRelease();
extern void keyDown();
extern void keyUp();
extern void finale();

int main(int argc, char* args[]);


#endif

