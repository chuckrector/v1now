#ifndef RENDER_H
#define RENDER_H

extern char layer0,layer1,layervc,layervc2,layervcwrite,cameratracking,drawparty,drawentities;
extern unsigned char *vcscreen,hookretrace,*vspmask,*vcscreen2,*vcscreen1;
extern int quakex,quakey,quake,screengradient,tileidx[2048],vspspeed;
extern int foregroundlock,xwin1,ywin1;

extern void InitRenderSystem();
extern void CalcVSPMask();
extern void ExecuteScript(unsigned short int s);
extern void animate(char i);
extern void drawmap();
extern void drawchars(int xw, int yw);
extern void DrawLayer0(int xw, int yw);

#endif