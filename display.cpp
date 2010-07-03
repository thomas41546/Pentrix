#include "display.h"
#include <SDL/SDL.h>
#include <string.h>
//#include <SDL/SDL_draw.h>


//char DEBUGMSG[129];

Display::Display(int screenWidth,int screenHeight, const char *caption){
   if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
        //sprintf (DEBUGMSG, "Couldn't initialize SDL: %s\n", SDL_GetError ());
        //MessageBox (0, DEBUGMSG, "Error", MB_ICONHAND); 
        exit (1);
    }
    
    if (TTF_Init() < 0)
    {
        //sprintf (DEBUGMSG, "Couldn't initialize TTF: %s\n", SDL_GetError ());
        //MessageBox (0, DEBUGMSG, "Error", MB_ICONHAND); 
        exit (1);
    }
    Display::LoadFont("blackWolf.ttf",16);

    /* Set 640x480 16-bits video mode */
    Display::Screen = SDL_SetVideoMode (screenWidth, screenHeight, 0, SDL_SWSURFACE | SDL_DOUBLEBUF);
    if (Display::Screen == NULL)
    {
        //sprintf (DEBUGMSG, "Couldn't set video mode: %s\n",SDL_GetError ());
        //MessageBox (0, DEBUGMSG, "Error", MB_ICONHAND); 
        exit (2);
    }
   SDL_WM_SetCaption( caption, NULL );
   Display::Width = Display::Screen->w;
   Display::Height = Display::Screen->h;
}
void Display::LoadFont(const char *name, int size){
    Display::Font = TTF_OpenFont(name, size);
    if (Font == NULL)
    {
        //sprintf (DEBUGMSG,"%s\n", TTF_GetError());
        //MessageBox (0, DEBUGMSG, "Error", MB_ICONHAND); 
        exit(1);
    }
}
void Display::DrawString(const char *text, int x, int y,int bg, int fg){
    SDL_Rect dest;
    SDL_Surface *surface;
    SDL_Color foregroundColor, backgroundColor;
    
    /* White text on a black background */
    
    foregroundColor.r = (fg & 0xFF0000) >> 4;
    foregroundColor.g = (fg & 0xFF00) >> 2;
    foregroundColor.b = (fg & 0x0000FF);
    
    backgroundColor.r = (bg & 0xFF0000) >> 4;
    backgroundColor.g = (bg & 0xFF00) >> 2;
    backgroundColor.b = (bg & 0x0000FF);
    
    /* Use SDL_TTF to generate a string image, this returns an SDL_Surface */

    surface = TTF_RenderUTF8_Shaded(Display::Font, text, foregroundColor, backgroundColor);

    if (surface == NULL)
    {
        printf("Couldn't create String %s: %s\n", text, SDL_GetError());

        return;
    }
    
    /* Blit the entire surface to the screen */

    dest.x = x;
    dest.y = y;
    dest.w = surface->w;
    dest.h = surface->h;

    SDL_BlitSurface(surface, NULL, Display::Screen, &dest);
    
    /* Free the generated string image */

    SDL_FreeSurface(surface);
}

void Display::Caption(const char * caption){
     SDL_WM_SetCaption( caption, NULL );
}

void Display::Clear(){
   int color = SDL_MapRGB (Display::Screen->format, 0, 0, 0);
   SDL_FillRect (Display::Screen, NULL, color);
}

void Display::DrawRectangle(int x,int y,int w,int h,int color){
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_FillRect (Display::Screen, &rect, color);
}

void Display::Render(){
   //Draw_Circle(Display::Screen,50, 50, 10,0x00FFFF);
   SDL_Flip (Display::Screen);
}

void Display::NewAnimation(const char * referenceName){ //left off here
      struct Animation * animation = (struct Animation *)calloc(sizeof(struct Animation),1);
      
      animation->it = 0;
      animation->pos = 0;
      animation->images = (SDL_Surface **)calloc(sizeof(SDL_Surface *)*32,1);
      
      Display::Animations[referenceName] = animation;
}
void Display::PushAnimationImage(const char * referenceName,const char * fileName){
       
       SDL_Surface *temp; 
       SDL_Surface *image;
       temp = SDL_LoadBMP(fileName);
        
       if ( temp == NULL) {
            //sprintf (DEBUGMSG, "Unable to load bitmap: %s\n",SDL_GetError ());
            //MessageBox (0, DEBUGMSG, "Error", MB_ICONHAND); 
       }
       
       image = SDL_DisplayFormat(temp); 
       SDL_FreeSurface(temp);
       
       Display::Animations[referenceName]->images[Display::Animations[referenceName]->pos++] = image;
       

}
void Display::IterateAnimation(const char * referenceName){
     Display::Animations[referenceName]->it++;
     if(Display::Animations[referenceName]->it >=  Display::Animations[referenceName]->pos){
          Display::Animations[referenceName]->it = 0;                                 
     }
     
}
void Display::DrawAnimation(int x, int y, const char * referenceName){
     SDL_Surface* bitmap = Display::Animations[referenceName]->images[Display::Animations[referenceName]->it];
     Display::DrawImage (x, y, bitmap);
}


void Display::LoadBitmap(const char *referenceName,const char *fileName){
   char *msg;
   SDL_Surface* temp; 
   SDL_Surface *image;
   temp = SDL_LoadBMP(fileName);
    
   if ( temp == NULL) {
       // sprintf (DEBUGMSG, "Unable to load bitmap: %s\n",SDL_GetError ());
        //MessageBox (0, DEBUGMSG, "Error", MB_ICONHAND); 
   }
   SDL_SetColorKey( temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, 255, 0, 255) ); 
   image = SDL_DisplayFormat(temp); 
   SDL_FreeSurface(temp);
   Display::Bitmaps[referenceName] = image;
   
  
}
void Display::DrawBitmap(int x,int y,const char *referenceName){
     SDL_Surface* bitmap = Display::Bitmaps[referenceName];
     Display::DrawImage(x, y, bitmap);
}

void Display::DrawImage(int x, int y, SDL_Surface* bitmap){
   char *msg;
   if (bitmap == NULL) {
       // sprintf (DEBUGMSG, "Unable to find bitmap");
        //MessageBox (0, DEBUGMSG, "Error", MB_ICONHAND); 
        return;
     }
   SDL_Rect srcrect,dstrect;
   srcrect.x = 0;
   srcrect.y = 0;
   srcrect.w = (bitmap)->w;
   srcrect.h = (bitmap)->h;
   
   dstrect.x = x;
   dstrect.y = y;
   dstrect.w = (Display::Screen)->w;
   dstrect.h = (Display::Screen)->h;
   
   
   SDL_BlitSurface(bitmap, &srcrect, Display::Screen, &dstrect);
   
}
/*
void  DisplayMsg(const char *text) //ENDED HERE
{
	SDL_Surface *FinalMessage = load_image("GFX\\mbox.bmp");
	SDL_Event events;
	bool MSG = true;

	RenderTextToSurface(Line1, 5,5, FinalMessage);

	while (MSG)
	{
		SDL_PollEvent(&events);

		if (events.key.keysym.sym == SDLK_RETURN)		// If enter:
		{
			MSG = false;
		}

		put_surface(0, 380, FinalMessage, screen);

		if(SDL_Flip(screen) == -1)
		{
			SDL_FreeSurface(FinalMessage);
			return 1;    
		}
	}

	SDL_FreeSurface(FinalMessage);
	return 0;	// Normal termination
}*/







