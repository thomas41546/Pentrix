# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

all:
	g++ highscore.cpp display.cpp sound.cpp main.cpp keyboard.cpp -lSDLmain -lSDL -lSDL_mixer -lSDL_ttf -o ./bin/Pentrix
