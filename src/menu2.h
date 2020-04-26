#ifndef MENU2_H
#define MENU2_H

struct menu {
  unsigned short int posx,posy;
  char linktype;
};

extern unsigned char gsimg[512];
extern unsigned char iuflag;

extern void EquipMenu(char c);
extern void greyscale(int width, int height, unsigned char* src, unsigned char* dest);
extern void ItemMenu(char c);
extern void LoadSaveErase(char mode);
extern void MagicMenu(char c);
extern void RemoveItem(char c, char i);

#endif