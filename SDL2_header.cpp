#include "res_path.h"
#include "pointd.h"
#include "SDL2_header.h"


// -------------res_path.h------------
std::string getResourcePath(const std::string &subDir){
#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif
	static std::string baseRes;
	if (baseRes.empty()){
		char *basePath = SDL_GetBasePath();
		if (basePath){
			baseRes = basePath;
			SDL_free(basePath);
		}
		else {
			std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
			return "";
		}
		//We replace the last bin/ with res/ to get the the resource path
		size_t pos = baseRes.rfind("bin");
		baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
	}
	return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}

// -------------pointd.h--------------
double cross( const PointD &a, const PointD &b )
{
	return a.x*b.y - a.y*b.x;
}
double dot( const PointD &a, const PointD &b )
{
	return a.x*b.x + a.y*b.y;
}
// -----------SDL2_header.h------------


namespace Game {


SDL_Renderer	*renderer	= NULL;
SDL_Window		*window		= NULL;

bool FPS_DISPLAY	= false;
double nowFPS;
double duration		= 0;
uint64_t duration_i	= 0;
Color canvasColor = {0, 0, 0, 255};

bool mousePressed	= false;
bool mouseDragged	= false;
uint8_t mouseButton;
int mouseX = 0;
int mouseY = 0;
int pmouseX = -1;
int pmouseY = -1;
bool keyPressed = false;
int keyValue;


const unsigned int FPS_RATE = 60;
const std::string RES_PATH_IMG	= getResourcePath("image");
const std::string RES_PATH_FONT = getResourcePath("fonts");


std::string	fontName = "msyh";
int			fontSize = 25;

uint8_t lastColor[4];

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}
/*
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}
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
				   const double &angle, const SDL_Point *center,
				   const SDL_RendererFlip &flip,
				   const SDL_Rect *clip)
{
	SDL_RenderCopyEx(ren, tex, clip, &dst, angle, center, flip );
}
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
				   const double &widthRate,	const double &heightRate,
				   const double &angle, const SDL_Point *center,
				   const SDL_RendererFlip &flip,
				   const SDL_Rect *clip){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w*widthRate;
		dst.h = clip->h*heightRate;
	}else
	{
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
		dst.w *= widthRate;
		dst.h *= heightRate;
	}
	renderTexture(tex, ren, dst, angle, center, flip, clip);
}
/*
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param fontFile The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color,
		int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText returns, then
	//load that surface into a texture
	SDL_Surface *surf = TTF_RenderUTF8_Solid(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

// ----------- Functions for users ---------------


Image* textToImage( const std::string &msg,
					const int32_t &size, const Color &color,
					const std::string &fontType)
{
	return renderText(msg, RES_PATH_FONT + fontType + ".ttf", color, size, renderer);
}


/*
 * Loads an image
 * @param file The image file to load
 * @return the loaded image, or nullptr if something went wrong.
 */
Image* loadImage( const std::string &file )
{
	return loadTexture( RES_PATH_IMG +file, renderer );
}

void setImageAlpha( Image *img, Uint8 alpha )
{
	SDL_SetTextureAlphaMod( img, alpha );
}

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
			   const double &widthRate,	const double &heightRate,
			   const double &angle, const Point *center,
			   const FlipType &flip,
			   const Rect *clip)
{
	SDL_Renderer *ren = renderer;
	renderTexture( img, ren, x, y, widthRate, heightRate, angle, center, flip, clip );
}

void getImageSize( Image *img, int &width, int &height )
{
	SDL_QueryTexture( img, NULL, NULL, &width, &height );
}


void setPenColor( const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a)
{
	lastColor[0] = r;
	lastColor[1] = g;
	lastColor[2] = b;
	lastColor[3] = a;
	SDL_SetRenderDrawColor( renderer, r, g, b, a );
}
void setPenColor( const Color &color )
{
	setPenColor( color.r, color.g, color.b, color.a );
}

void drawPoint( int x, int y )
{
	SDL_RenderDrawPoint( renderer, x, y );
}
void drawPoint( const Point& p )
{
	drawPoint( p.x, p.y );
}

void drawLine( int x1, int y1, int x2, int y2 )
{
	SDL_RenderDrawLine( renderer, x1, y1, x2, y2 );
}
void drawLine( const Point &p1 ,const Point &p2 )
{
	drawLine( p1.x, p1.y, p2.x, p2.y );
}

void drawLines(const SDL_Point* points, int count)
{
	SDL_RenderDrawLines( renderer, points, count );
}

void drawRect( const Rect& rect, const bool& fill )
{
	if( !fill )
		SDL_RenderDrawRect( renderer, &rect );
	else
		SDL_RenderFillRect( renderer, &rect );
}



void setCanvas( int x, int y, int width, int height )
{
	SDL_Rect rect = {x,y,x+width,y+height};
	SDL_RenderSetViewport( renderer, &rect );
}

// ---------------------------------------------------

void drawText( const std::string &msg, const int &x, const int &y,
			   const int32_t &size, const Color &color )
{
	Image *image = textToImage(msg, size, color);

	//Get the texture w/h so we can center it in the screen
	int iW, iH;
	getImageSize(image, iW, iH);
	drawImage(image, x, y);
	cleanup(image);
}

}


int main(int argc, char* args[]) {

	using namespace Game;


	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	//Also need to init SDL_ttf
	if (TTF_Init() != 0){
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}


	//Setup our window and renderer
	window = SDL_CreateWindow(TitleName.c_str(), SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	initialize();

	double deltaTime, delta, oneStepTime = 1./FPS_RATE;
	unsigned int t0, t1;
	int returnValue;
	SDL_Event event;
	bool quit = false;


	t0 = SDL_GetTicks();
	SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
	while( !quit )
	{

		while ( SDL_PollEvent( &event ) )
		{
			switch (event.type)
			{
				case SDL_QUIT :
					quit = true;
					break;
				case SDL_KEYDOWN :
					keyValue = event.key.keysym.sym;
					keyPressed = true;
					keyDown();
					break;
				case SDL_KEYUP:
					keyValue = event.key.keysym.sym;
					keyPressed = false;
					keyUp();
					break;
				case SDL_MOUSEBUTTONDOWN :
					mouseButton = event.button.button;
					pmouseX = mouseX;
					pmouseY = mouseY;
					mouseX = event.button.x;
					mouseY = event.button.y;
					mousePressed = true;
					mouseDragged = false;
					mousePress();
					break;
				case SDL_MOUSEMOTION :
					pmouseX = mouseX;
					pmouseY = mouseY;
					mouseX = event.motion.x;
					mouseY = event.motion.y;
					if (event.motion.state & SDL_BUTTON_LMASK || event.motion.state & SDL_BUTTON_RMASK)
					{
						mouseDragged = true;
					}
					mouseMove();
					break;
				case SDL_MOUSEBUTTONUP :
					pmouseX = mouseX;
					pmouseY = mouseY;
					mouseX = event.button.x;
					mouseY = event.button.y;
					mousePressed = false;
					mouseDragged = false;
					mouseRelease();
					break;
				default:
					break;
			}
		}

		returnValue = work( quit );

		t1 = SDL_GetTicks();
		delta = t1 - t0;
		t0 = t1;
		deltaTime = delta / 1000.0;

		if (delta < oneStepTime*1000) {
			SDL_Delay(oneStepTime*1000 - delta);
			deltaTime = oneStepTime;
		}
		duration = duration + deltaTime;
		duration_i++;


		//Display Fps
		nowFPS = 1.0 / deltaTime;
		if( FPS_DISPLAY )
		{
			char info[20];
			sprintf(info, "FPS: %2d", (int)(nowFPS + 0.5));
			drawText( info, 0, 0 );
		}

		//Draw the renderer
		setPenColor(canvasColor);
		SDL_RenderPresent(renderer);
		setPenColor(lastColor[0],lastColor[1],lastColor[2],lastColor[3]);
		SDL_RenderClear(renderer);
	}


	finale();

	cleanup( window, renderer );

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return returnValue;
}

