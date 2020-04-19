#ifndef MENU_H
#define MENU_H

extern unsigned char menuptr[256],charptr[960],itmptr[576];
extern int atkp,defp,magp,mgrp,hitp,dodp,mblp,ferp,reap;

extern void menubox(int x, int y, int x2, int y2);
extern void SystemMenu();
extern void MainMenu();
extern void tmenubox(int x, int y, int x2, int y2);
extern void StatusScreen(char cz);
extern void PutBuySellBox(char p);
extern void PutGPBox();
extern void PutCharBox(char a, char b, char c, char d, char e, char p);
extern void MBuyMenu();
extern void MSellMenu();
extern void BuyMenu();
extern void SellMenu();
extern void SellCharItem(char c);
extern void ConfirmSell(char c, char ptr);
extern void MConfirmSell(char c, char ptr);

#endif MENU_H