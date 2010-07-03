#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <time.h>

#include "display.h"
#include "sound.h"
#include "keyboard.h"
#include "highscore.h"

#define WIDTH 15
#define HEIGHT 30

#define BLOCKPIXSIZE 20
#define BLOCKSIZE 5
#define BLOCKNUM 9

#define MASK 0x80000000
#define CODE 0x80000000
#define CODE2 0x40000000

#define RANDI 60
#define QRANDI 100
#define PRERAND 3

Display display = Display((WIDTH+BLOCKSIZE*2)*BLOCKPIXSIZE,(HEIGHT)*BLOCKPIXSIZE,"Pentrix");
Keyboard keyboard = Keyboard();
Sound sound = Sound();



char DEBUGMSG2[128];
char TEXT[128];
int score= 0;
int hscore=0;




int blockx = 0;
int blocky = 0;
int blockt = 0;
int blockr = 0;

int minxcord = BLOCKSIZE-1;
int minycord = BLOCKSIZE-1;
int maxxcord = 0;
int maxycord = 0;

int randBlocksi = BLOCKNUM*PRERAND;
int randBlocks[BLOCKNUM*PRERAND];
int postceedingBlock = rand() % BLOCKNUM;
int rotationBlock = 0;

int cycle = 0;

int map[WIDTH][HEIGHT] = {};

static int blocks[BLOCKNUM][4][BLOCKSIZE][BLOCKSIZE] = {
    {{0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0},
    {0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0}},
    {{0,0,0,0,0, 0,0,0,0,1, 0,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,1,0},
    {0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,0, 1,0,0,0,0, 0,0,0,0,0},
    {0,1,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0}},
    {{0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1, 0,0,0,0,1, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,1,1,0,0},
    {0,0,0,0,0, 1,0,0,0,0, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,1,1,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0}},
    {{0,0,0,0,0, 0,0,1,1,0, 1,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,1,0,0, 0,0,1,0,0, 0,0,1,1,0, 0,0,0,1,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,0,0,0, 0,0,1,1,1, 0,1,1,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,1,0,0,0, 0,1,1,0,0, 0,0,1,0,0, 0,0,1,0,0}},
    {{0,0,0,0,0, 0,0,0,0,0, 1,1,1,0,0, 0,0,1,1,0, 0,0,0,0,0},
    {0,0,1,0,0, 0,0,1,0,0, 0,1,1,0,0, 0,1,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,1,1,0,0, 0,0,1,1,1, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,0,1,0, 0,0,1,1,0, 0,0,1,0,0, 0,0,1,0,0}},
    {{0,0,1,0,0, 0,0,1,0,0, 0,1,1,0,0, 0,1,1,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,1,1,0,0, 0,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,1,1,0, 0,0,1,1,0, 0,0,1,0,0, 0,0,1,0,0},
    {0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,0, 0,0,1,1,0, 0,0,0,0,0}},
    {{0,1,0,0,0, 0,1,0,0,0, 0,1,1,0,0, 0,1,1,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,1,1,1,1, 0,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,1,1,0, 0,0,1,1,0, 0,0,0,1,0, 0,0,0,1,0},
    {0,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 1,1,1,1,0, 0,0,0,0,0}},
    {{0,0,0,0,0, 0,0,1,1,0, 0,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,1,0,0, 0,0,1,1,0, 0,0,1,1,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,0, 0,1,1,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,1,1,0,0, 0,1,1,0,0, 0,0,1,0,0, 0,0,0,0,0}},
    {{0,0,0,0,0, 0,1,1,1,0, 0,0,1,1,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,0,1,0, 0,0,1,1,0, 0,0,1,1,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,0,0,0, 0,1,1,0,0, 0,1,1,1,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,1,1,0,0, 0,1,1,0,0, 0,1,0,0,0, 0,0,0,0,0}}
};

static int colormap[BLOCKNUM]={0xFF0000,0xFFFF00,0xFF00FF,0x0000FF,0x00DDDD,0x00FF00,0xFF7D40,0xAADD00,0x0068FF};
                          
                          
void drawMap();
void drawBlock(int);
int InvasiveCheckPass(int,int,int,int);
void updateBlockBoundaries();
void resetGame();
void newBlock();
void HorizontalStrip();
void render();
void gridLockBlock();
void fallBlock();


void drawMap(){
     for(int y = 0; y < HEIGHT; y++){
             if(y > 0)display.DrawRectangle(0,y*BLOCKPIXSIZE,WIDTH*BLOCKPIXSIZE,WIDTH*BLOCKPIXSIZE,0x222222);
             for(int x = 0; x < WIDTH; x++){
                      display.DrawRectangle(x*BLOCKPIXSIZE,y*BLOCKPIXSIZE,BLOCKPIXSIZE-2,BLOCKPIXSIZE-2,map[x][y]);
                      if((map[x][y] & MASK) == CODE){ //clears working block off the map
                             map[x][y] = 0;
                             
                      }
             } 
     }
}


void drawGhost(){
     int ny = blocky+maxycord;
     while(InvasiveCheckPass(blockx,ny,blockt,blockr)){ny++;}ny--;
     if(!InvasiveCheckPass(blockx,ny,blockt,blockr)){return;}
          
     for(int ox=0;ox<BLOCKSIZE;ox++){
         for(int oy=0;oy<BLOCKSIZE;oy++){
              if(blockx+ox > -1 && blockx+ox < WIDTH && ny+oy > -1 && ny+oy < HEIGHT && blocks[blockt][blockr][oy][ox]){
                  map[blockx+ox][ny+oy] = 0x333333 ^ CODE; 
                  }

         }
     }
}


void drawNextBlock(){
     int nt = 0;
     
     if(randBlocksi >= BLOCKNUM*PRERAND){nt = postceedingBlock;}
     else{nt = randBlocks[randBlocksi];}
     
     
     for(int ox=0;ox<BLOCKSIZE;ox++){
         for(int oy=0;oy<BLOCKSIZE;oy++){
              if(blocks[nt][rotationBlock][oy][ox]){
                  display.DrawRectangle(((WIDTH)+ox)*BLOCKPIXSIZE,((HEIGHT/2-BLOCKSIZE*2)+oy)*BLOCKPIXSIZE,BLOCKPIXSIZE-2,BLOCKPIXSIZE-2,colormap[nt]);
              }

         }
     }
}
void drawBorders(){
     display.DrawRectangle(0,HEIGHT*BLOCKPIXSIZE,WIDTH*BLOCKPIXSIZE,BLOCKSIZE*BLOCKPIXSIZE,0x000000);
}



void drawBlock(int code){
     for(int ox=0;ox<BLOCKSIZE;ox++){
         for(int oy=0;oy<BLOCKSIZE;oy++){
              if(blockx+ox > -1 && blockx+ox < WIDTH && blocky+oy > -1 && blocky+oy < HEIGHT && blocks[blockt][blockr][oy][ox]){
                  map[blockx+ox][blocky+oy] = colormap[blockt] ^ (code);
                  }

         }
     }
}

int InvasiveCheckPass(int mx, int my, int t, int r){
     if(my >= HEIGHT-maxycord){return 0;}
     r = (r > 3) ? 0 : r;
     for(int ox=0;ox<BLOCKSIZE;ox++){
         for(int oy=0;oy<BLOCKSIZE;oy++){
              if(mx+ox > -1 && mx+ox < WIDTH && my+oy > -1 && my+oy < HEIGHT && 
                 blocks[t][r][oy][ox] && ( map[mx+ox][my+oy] &MASK)  != CODE && map[mx+ox][my+oy] != 0)return 0;
         }
     }
     return 1;
}




void updateBlockBoundaries(){
    minxcord = BLOCKSIZE-1;
    minycord = BLOCKSIZE-1;
    maxxcord = 0;
    maxycord = 0;
    for(int x=0;x<BLOCKSIZE;x++){
       for(int y=0;y<BLOCKSIZE;y++){
           if(blocks[blockt][blockr][y][x]){
               if(x > maxxcord)
                    maxxcord = x;
               if(x < minxcord)
                    minxcord = x;
               if(y > maxycord)
                    maxycord = y;
               if(y < minycord)
                    minycord = y;
           }
       }
    }
}

void resetGame(){
     
     //==========================START MSG Prints Highscore
     while(!keyboard.s){
        display.Clear();
        drawMap();
        drawBorders();
        sprintf(TEXT,"Press 's' to start");
        display.DrawString(TEXT,  0, (HEIGHT/2)*BLOCKPIXSIZE,0,0x00FF00);
        display.Render();
        keyboard.Poll(); 
        SDL_Delay(50);
         }
     for(int i=0;i < WIDTH;i++){
         for(int ii=0;ii < HEIGHT;ii++){
                 map[i][ii] = 0;
         }
     }
    score = 0;
    newBlock();
}




int comparei = 0;
int compare(const void *str1,const void *str2){
    if(comparei < RANDI){
                comparei++;
                return 1-(rand()%2);
    }
    else{
        return 1;    
    }
}
int getRandBlock(){     
     if(randBlocksi >= BLOCKNUM*PRERAND){//init
         randBlocksi = 0;
         for(int i=0;i<BLOCKNUM*PRERAND;i++){
             randBlocks[i] = (i % BLOCKNUM);
         }
         for(int i=0;i < QRANDI; i++){
             comparei = 0;
             qsort(&randBlocks, BLOCKNUM*PRERAND, sizeof(int), &compare);
         }
         randBlocks[0] = postceedingBlock;
         postceedingBlock = rand() % BLOCKNUM;
     }
     return randBlocks[randBlocksi++];

}

void lostGame(){
     display.Clear(); drawMap(); display.Render();
     //====================Highscore if(score < hscore)
     if(score < hscore){
             //sprintf (DEBUGMSG2, "You lose nub.\nScore: %i\nHighscore: %i",score,hscore);  
             //MessageBox (0, DEBUGMSG2, "You Lost!", MB_ICONINFORMATION);
     }
     else{
          hscore = score;
          saveHighscore(&score);
         // sprintf (DEBUGMSG2, "You just achieved a highscore\nHighscore: %i",hscore);
          //MessageBox (0, DEBUGMSG2, "Nice!!!", MB_ICONINFORMATION);
          
          
     }
     resetGame();
}


void newBlock(){
    blockx = WIDTH/(BLOCKSIZE);
    blockt = getRandBlock();
    blockr = rotationBlock;
    updateBlockBoundaries();
    blocky = -maxycord;
    rotationBlock = (++rotationBlock > 3) ? 0 : rotationBlock;

    if(!InvasiveCheckPass(blockx,blocky,blockt,blockr)){lostGame();}
    else{sound.PlaySound("blockHit",120,0);}
}
void fallBlock(){
     if(InvasiveCheckPass(blockx,blocky+1,blockt,blockr)){blocky++;} //&& !(blocky >= HEIGHT-1-maxycord)
     else{
          gridLockBlock();
     }
}
void gridLockBlock(){
       drawBlock(0); 
       newBlock();
}


void HorizontalStrip(){
     int scoreAddition = 1;
     for(int y = 0; y < HEIGHT; y++){
         int prev =  1;
         for(int x = 0; x < WIDTH; x++){
              if((map[x][y] & MASK) == CODE || map[x][y] == 0){prev = 0;}
         }
         if(prev){ 
                  scoreAddition++;
                  for(int yy=y;yy>0;yy--){
                  for(int xx=0;xx<WIDTH;xx++){
                           map[xx][yy] = map[xx][yy-1];
                  }
                  }
                  sound.PlaySound("horizontal",128,0);
                  SDL_Delay(50);
                 y = -1; //reloop
         }
     }
     if(scoreAddition > 1)score += (100*scoreAddition + 10*scoreAddition*scoreAddition*scoreAddition);
     if(score > hscore){hscore = score;}
}


void render(){
        display.Clear();
        drawGhost();
        drawBlock(CODE);
        drawMap();
        drawBorders();
        drawNextBlock();
        sprintf(TEXT,"Score: %i",score);
        display.DrawString(TEXT,  ((WIDTH))*BLOCKPIXSIZE, (0)*BLOCKPIXSIZE,0,0x00FF00);
        sprintf(TEXT,"Highscore: %i",hscore);
        display.DrawString(TEXT,  ((WIDTH))*BLOCKPIXSIZE, (1)*BLOCKPIXSIZE,0,0x00FF00);
        sprintf(TEXT,"Next:");
        display.DrawString(TEXT,  ((WIDTH))*BLOCKPIXSIZE, (4)*BLOCKPIXSIZE,0,0x00FF00);
        display.Render();
}
int main (int argc, char *argv[])
{    
    srand ( time(NULL) );
    
    sound.LoadSound("blockHit","hit.wav");
    sound.LoadSound("horizontal","horizon2.wav");
    sound.LoadSound("rotate","rotate.wav");
    
    loadHighscore(&hscore);
    
    resetGame();
    
    while (keyboard.Quit == false)
    {   
          if(keyboard.r){

                       keyboard.Clear();
                       resetGame();
        }
        
        if(keyboard.p){

                       SDL_Delay(100);
                       keyboard.Poll();
                       keyboard.Clear();
                       while(!keyboard.p){
                            keyboard.Poll(); 
                            display.Clear();
                            drawBorders();
                            sprintf(TEXT,"Game is paused.");
                            display.DrawString(TEXT,  0, (HEIGHT/2)*BLOCKPIXSIZE,0,0x00FF00);
                            display.Render();
                            SDL_Delay(50);
                       }
        }
        int speed = 0;
        speed = score/2400;
        if(speed > 40){speed = 40;}
        
        if(!(cycle% (60-speed)) || keyboard.Down){fallBlock(); HorizontalStrip(); if(keyboard.Down)keyboard.Clear(); }
        
        keyboard.Poll();
        if(keyboard.Left && InvasiveCheckPass(blockx-1,blocky,blockt,blockr)){ --blockx; keyboard.Clear();}
        if(keyboard.Right && InvasiveCheckPass(blockx+1,blocky,blockt,blockr)){++blockx; keyboard.Clear();}
        
        if(keyboard.Up && InvasiveCheckPass(blockx,blocky,blockt,blockr+1)){ 
             blockr = (++blockr > 3) ? 0 : blockr; updateBlockBoundaries();
             sound.PlaySound("rotate",80,0);
             keyboard.Clear();
             
        }
        
        if(keyboard.Space){
            while(InvasiveCheckPass(blockx,blocky+1,blockt,blockr)){fallBlock(); HorizontalStrip();}
            gridLockBlock();
            keyboard.Clear();
            SDL_Delay(80);
        }
    
        
        blockx = (blockx< -minxcord) ? -minxcord : ((blockx > (WIDTH-1)-maxxcord) ? (WIDTH-1)-maxxcord : blockx);
        
        
        render(); 
        
        char TXT[128];
        if(speed > 30){sprintf(TXT,"EXTREME PENTRIX: %i",hscore);}
        else{sprintf(TXT,"Pentrix Highscore: %i",hscore);}
        display.Caption(TXT);
        
        
        SDL_Delay (4); cycle = (++cycle > 239) ? 1 : cycle;
    }

    return 0;
}


/*
original tetris 4x4 matrix
{
                          {{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
                           {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0}, 
                           {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
                           {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0}},
                          
                          {{0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0},
                           {0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0},
                           {1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
                           {0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0}},
                          
                          {{0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0},
                           {1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0},
                           {0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0},
                           {0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0}},

                          
                          {{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
                           {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
                           {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
                           {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
                          
                          {{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},
                           {1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0},
                           {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},
                           {1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0}},
                          
                          {{0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
                          {0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0},
                          {0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0},
                          {0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0}},
                          
                          {{1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
                          {0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0},
                          {1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
                          {0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0}}
                          };
*/
