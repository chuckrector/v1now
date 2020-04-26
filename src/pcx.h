#ifndef PCX_H
#define PCX_H

#include <stdio.h>

extern unsigned short int width;
extern unsigned short int depth;
extern FILE *pcxf;
extern unsigned int vidoffset;

extern void loadpcx(char* fname, unsigned char* dest);
extern void LoadPCXHeaderNP(char* fname);
extern void ReadPCXLine(unsigned char* dest);
extern void ScreenShot();

#endif