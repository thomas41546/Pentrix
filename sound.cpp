#include "sound.h"
char DEBUGMSG3[128];

Sound::Sound(){


    if (SDL_Init (SDL_INIT_AUDIO) < 0)
    {
        //sprintf (DEBUGMSG3, "Couldn't initialize Sound: %s\n", SDL_GetError ());
        //MessageBox (0, DEBUGMSG3, "Error", MB_ICONHAND); 
        exit (1);
    }
    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 1;
    int audio_buffers = 8192;
     
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
    	//sprintf(DEBUGMSG3,"Unable to initialize audio: %s\n", Mix_GetError());
        //MessageBox (0, DEBUGMSG3, "Error", MB_ICONHAND); 
        exit (1);
    }
}
void Sound::LoadSound(const char *referenceName,const char *fileName)
{
    char DEBUGMSG3[1024];

    Mix_Chunk *sound = Mix_LoadWAV(fileName);
    if(sound == NULL) {
    	//sprintf(DEBUGMSG3,"Unable to load WAV file: %s\n", Mix_GetError());
        //MessageBox (0, DEBUGMSG3, "Error", MB_ICONHAND); 
        exit (1);
    }
    Sound::Chunks[referenceName] = sound;
}


void Sound::PlaySound(const char *referenceName, int volume, int loops)
{
    char DEBUGMSG3[1024];
    Mix_VolumeChunk(Sound::Chunks[referenceName],volume);
    if(Mix_PlayChannel(-1, Sound::Chunks[referenceName], loops) == -1) {
    	//sprintf(DEBUGMSG3,"Unable to play WAV file: %s\n", Mix_GetError());
        //MessageBox (0, DEBUGMSG3, "Error", MB_ICONHAND); 
        exit (1);
    }
}
