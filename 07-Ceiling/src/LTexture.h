#ifndef LTextureH

#define LTextureH

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>


//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		//Loads image into pixel buffer
		bool loadPixelsFromFile( std::string path );

		//Creates image from preloaded pixels
		bool loadFromPixels();

		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		//Pixel accessors
		Uint32* getPixels32();
		Uint32 getPitch32();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Surface pixels
		SDL_Surface* mSurfacePixels;
		
		//Image dimensions
		int mWidth;
		int mHeight;
};

#endif