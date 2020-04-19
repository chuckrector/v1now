#ifndef MENU2_H
#define MENU2_H

extern void LoadSaveErase(char mode);
extern void ItemMenu(char c);
extern void EquipMenu(char c);
extern void MagicMenu(char c);
extern void greyscale(int width, int height, unsigned char* src, unsigned char* dest);
extern void RemoveItem(char c, char i);

#endif