#include "keyboard.h"
#include <SDL/SDL.h>

Keyboard::Keyboard(){
   Keyboard::Clear();
   Keyboard::Quit=false;
   SDL_EnableKeyRepeat(50,50);
   SDL_PollEvent(NULL);
}


void Keyboard::Poll(){
    SDL_Event event;
	while(SDL_PollEvent(&event)) { 
		if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_r){Keyboard::r = true;}
                if(event.key.keysym.sym==SDLK_p){Keyboard::p = true;}
                if(event.key.keysym.sym==SDLK_s){Keyboard::s = true;}
                if(event.key.keysym.sym==SDLK_LEFT){Keyboard::Left = true;}
                if(event.key.keysym.sym==SDLK_RIGHT){Keyboard::Right = true;}
                if(event.key.keysym.sym==SDLK_UP){Keyboard::Up = true;}
                if(event.key.keysym.sym==SDLK_DOWN){Keyboard::Down = true;}
                if(event.key.keysym.sym==SDLK_SPACE){Keyboard::Space = true;}
        }
        if(event.type == SDL_KEYUP){
                if(event.key.keysym.sym==SDLK_r){Keyboard::r = false;}
                if(event.key.keysym.sym==SDLK_p){Keyboard::p = false;}
                if(event.key.keysym.sym==SDLK_s){Keyboard::s = false;}
                if(event.key.keysym.sym==SDLK_LEFT){Keyboard::Left = false;}
                if(event.key.keysym.sym==SDLK_RIGHT){Keyboard::Right = false;}
                if(event.key.keysym.sym==SDLK_UP){Keyboard::Up = false;}
                if(event.key.keysym.sym==SDLK_DOWN){Keyboard::Down = false;}
                if(event.key.keysym.sym==SDLK_SPACE){Keyboard::Space = false;}
        }

        if(event.type == SDL_QUIT){Keyboard::Quit = true; exit(1);}
        else{Keyboard::Quit = false;}
    }
}
void Keyboard::Clear(){
   Keyboard::r = false;
   Keyboard::p = false;
   Keyboard::s = false;
   Keyboard::Left = false;
   Keyboard::Right = false;
   Keyboard::Up = false;
   Keyboard::Down = false;
   Keyboard::Space = false;
}


