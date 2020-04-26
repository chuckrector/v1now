#ifndef RENDER_H
#define RENDER_H

extern char layer0, layer1, layervc, layervc2, layervcwrite, cameratracking,
    drawparty, drawentities;
extern unsigned char *vcscreen;
extern unsigned char hookretrace;
extern unsigned char *vspmask;
extern unsigned char *vcscreen2;
extern unsigned char *vcscreen1;
extern int quakex, quakey, quake, screengradient, tileidx[2048], vspspeed;
extern int foregroundlock, xwin1, ywin1;
extern unsigned char layer1trans;
extern unsigned char layervctrans;
extern unsigned char layervc2trans;

extern void animate(char i);
extern void CalcVSPMask();
extern void drawchars(int xw, int yw);
extern void DrawLayer0(int xw, int yw);
extern void drawmap();
extern void drawmaploc(int xw, int yw);
extern void InitRenderSystem();

#endif