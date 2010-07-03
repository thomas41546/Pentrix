#include "highscore.h"

void readfile(char **,unsigned long int *, const char *);
void writefile(char *,unsigned long int , const char *);

void readfile(char **buffer,unsigned long int *filelength, const char *filename){
     FILE *fh;
     fh =  fopen(filename, "rb");
     if(fh == NULL){return;}//error

     fseek(fh, 0, SEEK_END);
     *filelength = ftell(fh);
     fseek(fh, 0, SEEK_SET);
     
     *buffer = (char *) malloc(*filelength+1); //endline
     fread(*buffer, *filelength,1, fh); 
     *(*buffer+*filelength) = 0; //end
     fclose(fh); 
}


void writefile(char *buffer,unsigned long int buflen, const char *filename){

     FILE *fh;
     fh =  fopen(filename, "wb");
     if(fh == NULL){return;}//error
     fseek(fh, 0, SEEK_SET);
     fwrite(buffer, buflen, 1, fh); 
     fclose(fh); 
}
void writefile(unsigned char *buffer,unsigned long int buflen, const char *filename){

     FILE *fh;
     fh =  fopen(filename, "wb");
     if(fh == NULL){return;}//error
     fseek(fh, 0, SEEK_SET);
     fwrite(buffer, buflen, 1, fh); 
     fclose(fh); 
}


void loadHighscore(int * hscore){
    char *buff;
    unsigned long int blen;
    readfile(&buff,&blen, "hscore.dat");
    *hscore = (int)(*buff);
    memcpy(hscore, buff, blen);
}
void saveHighscore(int * hscore){
     char scorex[sizeof(int)];
     memcpy(scorex, hscore, sizeof(int));
     writefile(scorex,sizeof(int),"hscore.dat");
}






