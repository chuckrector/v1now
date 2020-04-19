#ifndef PCX_H
#define PCX_H

#include <stdio.h>

extern short int width,depth;
extern FILE *pcxf;
extern int vidoffset;

extern void ScreenShot();
extern void loadpcx(char* fname, char* dest);
extern void LoadPCXHeaderNP(char* fname);
extern void ReadPCXLine(unsigned char* dest);

#endif