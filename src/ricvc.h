#ifndef RICVC_H
#define RICVC_H

extern void BindKey();
extern void CallEffect();
extern void CallScript();
extern int ChooseChar();
extern int ChooseChar(int x1, int y1);
extern void equipMenu();
extern void ExecuteStartUpScript(unsigned short s);
extern void
grey(int width, int height, unsigned char *src, unsigned char *dest);
extern void itemMenu();
extern void magicMenu();
extern void statusScreen();
extern void TextMenu();
extern void VCAString(int x1, int y1, const char *strng, int align);
extern void VCATextNum();
extern void VCborder(int x1, int y1, int x2, int y2);
extern void VCBox();
extern void VCCharName();
extern void
VCColorField(int x1, int y1, int x2, int y2, unsigned char *colortbl);
extern void VCCr2();
extern void VChline(int x, int y, int x2, char c);
extern void VCItemDesc();
extern void VCItemImage();
extern void VCItemName();
extern void VCSpc();
extern void VCTextBox();
extern void VCvline(int x, int y, int y2, char c);

#endif