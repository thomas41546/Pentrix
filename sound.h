#ifndef SOUND
#define SOUND
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <map>

class Sound{
public:
  std::map<const char*, Mix_Chunk*> Chunks;
  Sound();
  void LoadSound(const char *,const char *);
  void PlaySound(const char *,int,int);
  
};

#endif
