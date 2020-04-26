#ifndef VGA_H
#define VGA_H

#include "control.h"
#include "engine.h" // for valloc()
#include "render.h"
#include "timer.h"
#include <stdio.h>
#include <strings.h>

extern unsigned char pal[768];
extern unsigned char pal2[768];

extern unsigned char menuxlatbl[256], greyxlatbl[256], scrnxlatbl[256],
    *transparencytbl;
extern unsigned char oc;
extern char waitvrt;
extern char fade;
extern char cancelfade;

extern unsigned char pal[768];
extern unsigned char *screen;
extern unsigned char *virscr;

extern void bigprintstring(const char *str);
extern void box(int x, int y, int x2, int y2, unsigned char color);
extern void closevga();
extern void ColorField(int x, int y, int x2, int y2, unsigned char *tbl);
extern void ColorScale(unsigned char *dest, int st, int fn, int inv);
extern void copysprite(int x, int y, int width, int height, unsigned char *spr);
extern void copytile(int x, int y, unsigned char *spr);
extern void dec_to_asciiz(int num, char *buf);
extern void fin();
extern void fontcolor(unsigned char c);
extern void fout();
extern void get_palette();
extern void gotoxy(int x, int y);
extern void grabregion(int x, int y, int width, int height, unsigned char *spr);
extern void hline(int x, int y, int dist, char c);
extern void initvga();
extern void LoadFont();
extern void PreCalc_TransparencyFields();
extern void printstring(const char *str);
extern void quick_killgfx();
extern void quick_restoregfx();
extern void set_intensity(unsigned int n);
extern void set_palette(unsigned char *pall);
extern void setpixel(int x, int y, char c);
extern void
tcopysprite(int x, int y, int width, int height, unsigned char *spr);
extern void
Tcopysprite(int x1, int y1, int width, int height, unsigned char *src);
extern void
_Tcopysprite(int x1, int y1, int width, int height, unsigned char *src);
extern void tcopytile(int x, int y, unsigned char *spr, unsigned char *matte);
extern void
textwindow(char portrait, const char *str1, const char *str2, const char *str3);
extern void VCprintstring(int xx, int yy, const char *str);
extern void
VCtcopysprite(int x, int y, int width, int height, unsigned char *spr);
extern void vgadump();
extern void vline(int x, int y, int y2, char c);

#endif