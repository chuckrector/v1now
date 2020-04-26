#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <string.h> // strlen

#include "control.h"
#include "engine.h"
#include "keyboard.h"
#include "main.h"
#include "menu2.h"
#include "render.h"
#include "sound.h"
#include "timer.h"
#include "vga.h"

#define bgcolor 154
#define grey1 14
#define grey2 26
#define white 31

extern unsigned char menuptr[256];
extern unsigned char charptr[960];
extern unsigned char itmptr[576];
extern int atkp;
extern int defp;
extern int magp;
extern int mgrp;
extern int hitp;
extern int dodp;
extern int mblp;
extern int ferp;
extern int reap;
extern char menuactive;

extern void BuyMenu();
extern void ConfirmSell(char c, char ptr);
extern void MainMenu();
extern void MBuyMenu();
extern void MConfirmSell(char c, char ptr);
extern void menubox(int x, int y, int x2, int y2);
extern void MSellMenu();
extern void PutBuySellBox(char p);
extern void PutCharBox(char a, char b, char c, char d, char e, char p);
extern void PutGPBox();
extern void SellCharItem(char c);
extern void SellMenu();
extern void StatusScreen(char cz);
extern void SystemMenu();
extern void tmenubox(int x, int y, int x2, int y2);
extern void BuyItem(char ptr);
extern void PurchaseItem(char c, char i);
extern void BuyOrBuyEquip(char c, char ptr);
extern void SellCharMagic(char c);
extern void SellCharItem(char c);
extern void BuyMagic(char ptr);
extern void PurchaseMagic(char c, char i);

#endif // MENU_H