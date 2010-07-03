#ifndef DISPLAY
#define DISPLAY
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <map>

struct Animation{
       int it;
       int pos;
       SDL_Surface** images;
};

class Display{
	public:
	  int Width;
	  int Height;
	  TTF_Font* Font;
	  SDL_Surface* Screen;
	  std::map<const char*, SDL_Surface*> Bitmaps;
	  std::map<const char*, struct Animation *> Animations;
	  
	  Display(int,int,const char *);
	  
	  void LoadBitmap(const char *referenceName,const char *fileName);
	  void LoadFont(const char *, int);
	  void DrawString(const char *, int , int,int,int);

	  
	  void Caption(const char *);
	  void LoadBitmapA(const char *,const char *);
	  
	  void NewAnimation(const char *);
	  void PushAnimationImage(const char *,const char *);
	  void IterateAnimation(const char *);
	  void DrawAnimation(int, int,const char *);
	  
	  void Clear();
	  void Render();
	  void DrawRectangle(int,int,int,int,int);
	  
	  void DrawBitmap(int,int,const char *);
	  void DrawText(const char, int, int);
	  void DrawImage(int , int , SDL_Surface*);
  
  
  
};
#endif
