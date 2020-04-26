#ifndef VCLIB_H
#define VCLIB_H

extern unsigned char storeinv[12];
extern char stringbuffer[100];
extern char keepaz;

extern void VCClear();
extern void drawmaploc(int xw, int yw);
extern void WriteVar0(int var, int value);
extern long ReadVar0(int var);
extern void WriteVar1(int var, int arg1, int value);
extern long ReadVar1(int var, int arg1);
extern void WriteVar2(int var, int arg1, int arg2, int value);
extern long ReadVar2(int var, int arg1, int arg2);
extern void ExecLibFunc(unsigned char func);

#endif