
#include <SDL.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;



enum AllSurfaces
{
    SURFACE_BOARD ,
    SURFACE_TOTAL
};
enum ChessPieces
{
	WKing,
	WQueen,
	WBishop,
	WKnight,
	WRook,
	WPawn,
	BKing,
	BQueen,
	BBishop,
	BKnight,
	BRook,
	BPawn,
	PTotal
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();
//ingame flag
int inGame = 0;
//Frees media and shuts down SDL
void close();

SDL_Texture WhiteK = SDL_CreateTexture(gRenderer,SDL_BITMAPORDER_NONE,SDL_TEXTUREACCESS_TARGET,107,107);
//Loads individual image
SDL_Surface* loadSurface( std::string path );
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;
//Default renderer
SDL_Renderer* gRenderer = NULL;
//Current displayed image
SDL_Surface* gStretchedSurface = NULL;
//surfaces
SDL_Surface* gExtraSur[SURFACE_TOTAL];
//chess pieces sprites
SDL_Rect gChessPieces[PTotal];
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "CHESS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
		//Initialize renderer color
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load stretching surface
	gStretchedSurface = loadSurface( "res/main.bmp" );
	if( gStretchedSurface == NULL )
	{
		printf( "Failed to load stretching image!\n" );
		success = false;
	}
	gExtraSur[SURFACE_BOARD] = loadSurface("res/board.bmp" );
	if( gExtraSur[SURFACE_BOARD] == NULL )
	{
		printf( "Failed to load stretching image!\n" );
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	SDL_FreeSurface( gStretchedSurface );
	gStretchedSurface = NULL;
	for( int i = 0; i < SURFACE_TOTAL; ++i )
	{
		SDL_FreeSurface( gExtraSur[ i ] );
		gExtraSur[ i ] = NULL;
	}
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
                    {
                        //Select surfaces based on key press
                        switch( e.key.keysym.sym )
                        {
						case SDLK_g:
							if (inGame == 1)
							{
								break;
							}
							else
							{
								gStretchedSurface = gExtraSur[SURFACE_BOARD];
								inGame = 1;
							}
							break;
                         default:                     
                         break;
                        }
                    }
				}

				//Apply the image stretched
				SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;
				SDL_BlitScaled( gStretchedSurface, NULL, gScreenSurface, &stretchRect );
				
				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
