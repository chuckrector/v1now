#ifndef VGA_H
#define VGA_H

// vga.h
extern void initvga();
extern void closevga();
extern void quick_killgfx();
extern void quick_restoregfx();
extern void vgadump();
extern void setpixel(int x, int y, char c);
extern void hline(int x, int y, int dist, char c);
extern void box(int x, int y, int x2, int y2, char color);
extern void set_palette(unsigned char *pall);
extern void set_intensity(unsigned int n);
extern unsigned char* screen;
extern unsigned char* virscr;
extern unsigned char pal[768];
extern void LoadFont();
extern void get_palette();
extern void gotoxy(int x, int y);
extern void printstring(char* str);
extern void tcopysprite(int x, int y, int width, int height, char* spr);
extern void fin();
extern void fout();
extern void Tcopysprite(int x1, int y1, int width, int height, unsigned char* src);
extern void _Tcopysprite(int x1, int y1, int width, int height, unsigned char* src);
extern void ColorField(int x, int y, int x2, int y2, unsigned char* tbl);
extern void copytile(int x, int y, char* spr);
extern void tcopytile(int x, int y, char* spr, char* matte);
extern void VCprintstring(int xx, int yy, char* str);
extern void VCtcopysprite(int x, int y, int width, int height, char* spr);
extern void dec_to_asciiz(int num, char* buf);
extern void bigprintstring(char* str);
extern void grabregion(int x, int y, int width, int height, char* spr);
extern void copysprite(int x, int y, int width, int height, char* spr);
extern void fontcolor(unsigned char c);
extern void textwindow(char portrait, char* str1, char* str2, char* str3);
extern void ColorScale(char* dest, int st, int fn, int inv);
extern void vline(int x, int y, int y2, char c);

extern unsigned char menuxlatbl[256],greyxlatbl[256],scrnxlatbl[256],*transparencytbl;

#endif