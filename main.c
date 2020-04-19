// main.c
// Execution engine / player code.
// Copyright (C)1997 BJ Eirich

#include <stdio.h>
#include <stdlib.h> // exit
#include <time.h> // time
#include <string.h> // memset
#include "util.h"
#include "control.h"
#include "engine.h"
#include "keyboard.h"
#include "render.h"
#include "timer.h"
#include "vga.h"
#include "sound.h"
#include "vclib.h"
#include "menu.h"
#include "menu2.h"
#include "vc.h"

unsigned char *strbuf,*speech;
extern unsigned char menuptr[256],qabort,itmptr[576],charptr[960];
extern char *effectvc,menuactive,*startupvc;

// NEW: MAGIC
extern char *magicvc;
// END NEW

extern unsigned int effectofstbl[1024],startupofstbl[1024],magicofstbl[1024];

int Exist(char *fname)
{ FILE *tempf;

  tempf=fopen(fname,"rb");
  if (tempf)
  {
     fclose(tempf);
     return 1;
  }
  else return 0;
}

void err(char *ermsg)
{
  keyboard_close();
  stopsound();
  // MD_Exit();
  timer_close();
  closevga();
  printf("%s \n",ermsg);
  exit(-1);
}

void MiscSetup()
{
  strbuf=(unsigned char*)valloc(100,"strbuf");
  keyboard_init();
  keyboard_chain(0);
  sound_init();
  timer_init();
//  allocbuffers(); /* -- ric: 30/Apr/98 -- */
                    /* Removed as already called in sound_init() */
  LoadFont();
  InitRenderSystem();
  srand(time(NULL));
  if (Exist("STARTUP.SCR"))
    printf("Warning: startup.scr found, this file is no longer used \n");
}

void PutOwnerText()
{ int i;

  printf("VERGE - System version 04.Jun.98\n");
  printf("Copyright (C)1997 vecna\n");
  printf("\n");
  printf("The VERGE Development Team\n");
  printf("--\n");
  printf("vecna, hahn, zeromus, McGrue, Locke, aen, Ric, NichG, xBig_D\n");

  // delay(500);

  get_palette();
  for (i=63; i>=0; i--)
      { set_intensity(i);
        //delay(10);
      }
}

void InitPStats()
{ FILE *pdat,*cdat;
  char i;
  pdat=fopen("PARTY.DAT","r");
  if (!pdat) err("Fatal error: PARTY.DAT not found");
  fscanf(pdat,"%s",strbuf);
  tchars=atoi((const char*)strbuf);
  for (i=0; i<tchars; i++)
      { fscanf(pdat,"%s",&pstats[i].chrfile);
        fscanf(pdat,"%s",strbuf);
        fscanf(pdat,"%s",strbuf);
        cdat=fopen((const char*)strbuf,"r");
        if (!cdat) err("Could not open character DAT file.");
        fscanf(cdat,"%s",&pstats[i].name);
        fgets((char*)strbuf,99,cdat);
        fgets((char*)strbuf,99,cdat);
        fscanf(cdat,"%s",strbuf);
        pstats[i].exp=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].curhp=atoi((const char*)strbuf);
        pstats[i].maxhp=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].curmp=atoi((const char*)strbuf);
        pstats[i].maxmp=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].str=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].end=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].mag=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].mgr=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].hit=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].dod=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].mbl=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].fer=atoi((const char*)strbuf);
        fscanf(cdat,"%s",strbuf);
        pstats[i].rea=atoi((const char*)strbuf);
        pstats[i].lv=1;
        fscanf(cdat,"%s",strbuf);
        pstats[i].nxt=atoi((const char*)strbuf);
        pstats[i].status=0;
        pstats[i].invcnt=6;
        memset(&pstats[i].inv, 0, 512);
        fclose(cdat);
       }
  fclose(pdat);
}

void StartNewGame(char *startp)
{ int i;

  numchars=0;
  InitPStats();
  addcharacter(1);
  usenxy=0; gp=0; party[0].speed=4;
  memset(flags,0,32000);
  hr=0; min=0; sec=0; tickctr=0;
  quake=0; hooktimer=0; hookretrace=0; foregroundlock=1;
  screengradient=0; layervc=0; VCClear();
  layer0=1; layer1=1; drawparty=1; drawentities=1; cameratracking=1;
  UpdateEquipStats();
  startmap(startp);
}

void LoadGame(char *fn)
{ FILE *f;
  char i,b;

  quake=0; hooktimer=0; hookretrace=0;
  screengradient=0; layervc=0; VCClear(); foregroundlock=1;
  layer0=1; layer1=1; drawparty=1; drawentities=1; cameratracking=1;
  numchars=0;
  f=fopen(fn,"rb");
  fread(strbuf, 1, 51, f);
  fread(&gp, 1, 4, f);
  fread(&hr, 1, 1, f);
  fread(&min, 1, 1, f);
  fread(&sec, 1, 1, f);
  fread(&b, 1, 1, f);
  fread(&menuactive, 1, 1, f);
  fread(virscr, 1, 2560, f);
  fread(&mapname, 1, 13, f);
  fread(&party, 1, sizeof party, f);
  fread(&partyidx, 1, 5, f);
  fread(&flags, 1, 32000, f);
  fread(&tchars, 1, 1, f);
  fread(&pstats, 1, sizeof pstats, f);
  fclose(f);
  for (i=0; i<b; i++)
      addcharacter(partyidx[i]);
  nx=party[0].x/16;
  ny=party[0].y/16;
  usenxy=1;
  startmap(mapname);
}

void ProcessEquipDat()
{ FILE *f;
  int a,i,t;
  int i2;

  // This function parses EQUIP.DAT, which sets all the stats for equipment.
  // It's pretty long as it has to have a processing section for each possible
  // stat, plus some other stuff. :P

  f=fopen("EQUIP.DAT","r");
  if (!f) err("Could not open EQUIP.DAT.");
  fscanf(f,"%d",&a);
  for (i=1; i<=a; i++)
  {
    pl1:
      fscanf(f,"%s",strbuf); strupr((char*)strbuf);
      if (!strcmp((char*)strbuf,"//")) { fgets((char*)strbuf,99,f); goto pl1; }
      if (!strcmp((char*)strbuf,"ATK")) { fscanf(f,"%s",strbuf);
                                   equip[i].str=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"DEF")) { fscanf(f,"%s",strbuf);
                                   equip[i].end=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"MAG")) { fscanf(f,"%s",strbuf);
                                   equip[i].mag=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"MGR")) { fscanf(f,"%s",strbuf);
                                   equip[i].mgr=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"HIT")) { fscanf(f,"%s",strbuf);
                                   equip[i].hit=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"DOD")) { fscanf(f,"%s",strbuf);
                                   equip[i].dod=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"MBL")) { fscanf(f,"%s",strbuf);
                                   equip[i].mbl=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"FER")) { fscanf(f,"%s",strbuf);
                                   equip[i].fer=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"REA")) { fscanf(f,"%s",strbuf);
                                   equip[i].rea=atoi((const char*)strbuf); goto pl1; }
      if (!strcmp((char*)strbuf,"ONEQUIP")) { fscanf(f,"%s",strbuf);
                                       equip[i].onequip=atoi((const char*)strbuf)+1; goto pl1; }
      if (!strcmp((char*)strbuf,"ONDEEQUIP")) { fscanf(f,"%s",strbuf);
                                         equip[i].ondeequip=atoi((const char*)strbuf)+1; goto pl1; }
      if (!strcmp((char*)strbuf,"EQABLE"))
      {
        eqloop:
          fscanf(f,"%s",strbuf);
          if (!strcmp((char*)strbuf,"-")) continue;
          equip[i].equipable[atoi((const char*)strbuf)-1]=1;
          goto eqloop;
      }
  }
// FOLLOWING IS NEW, AND NEEDS TRIMMING

  f=fopen("MAGICEQ.DAT","r");
  if (!f) err("Could not open MAGICEQ.DAT.");
  fscanf(f,"%d",&a);
  for (i=1; i<=a; i++)
  {
    mpl1:
      fscanf(f,"%s",strbuf); strupr((char*)strbuf);
      if (!strcmp((char*)strbuf,"//")) { fgets((char*)strbuf,99,f); goto mpl1; }

      if (!strcmp((char*)strbuf,"EQABLE"))
      {
        meqloop:
          fscanf(f,"%s",strbuf);
          if (!strcmp((char*)strbuf,"-")) continue;
          mequip[i].equipable[atoi((const char*)strbuf)-1]=1;
          goto meqloop;
      }
      if (!strcmp((char*)strbuf,"LEVEL"))
      {
        mlevloop:
          fscanf(f,"%s",strbuf);
          if (!strcmp((char*)strbuf,"-")) continue;
          i2 = atoi((const char*)strbuf);
          mequip[i].level[i2-1]=1;
          goto mlevloop;
      }

  }
// END NEW
}

void InitItems()
{ FILE *f;
  unsigned char b,i;
  int j;
  f=fopen("ITEMICON.DAT","rb");
  if (!f) err("Could not open ITEMICON.DAT.");
  fread(&b, 1, 1, f);
  fread(itemicons+256, 256, b, f);
  fclose(f);

  f=fopen("ITEMS.DAT","r");
  if (!f) err("Could not open ITEMS.DAT.");
  fscanf(f,"%s",strbuf);
  b=atoi((const char*)strbuf);
  for (i=1; i<b+1; i++)
      { fscanf(f,"%s",items[i].name);
        fscanf(f,"%s",strbuf);
        items[i].icon=atoi((const char*)strbuf)+1;
        fscanf(f,"%s",items[i].desc);
        fscanf(f,"%s",strbuf);
        items[i].useflag=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        items[i].useeffect=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        items[i].itemtype=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        items[i].equipflag=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        items[i].equipidx=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        items[i].itmprv=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        items[i].price=atoi((const char*)strbuf); }
  fclose(f);

  // *** NEW ***  MAGIC INIT

  f=fopen("MAGICON.DAT","rb");
  if (!f) err("Could not open MAGICON.DAT.");
  fread(&b, 1, 1, f);
  fread(magicicons+256, 256, b, f);
  fclose(f);

  f=fopen("MAGIC.DAT","r");
  if (!f) err("Could not open MAGIC.DAT.");
  fscanf(f,"%s",strbuf);
  b=atoi((const char*)strbuf);
  for (i=1; i<b+1; i++)
      { fscanf(f,"%s",magic[i].name);
        fscanf(f,"%s",strbuf);
        magic[i].icon=atoi((const char*)strbuf)+1;
        fscanf(f,"%s",magic[i].desc);
        fscanf(f,"%s",strbuf);
        magic[i].useflag=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        magic[i].useeffect=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        magic[i].itemtype=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        magic[i].equipflag=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        magic[i].equipidx=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        magic[i].itmprv=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        magic[i].price=atoi((const char*)strbuf);
        fscanf(f,"%s",strbuf);
        magic[i].cost=atoi((const char*)strbuf); }
  fclose(f);

  f=fopen("MAGIC.VCS","rb");
  if (!f) err("Could not open MAGIC.VCS");
  fread(&j, 1, 4, f);
  fread(&magicofstbl, 4, j, f);
  fread(magicvc, 1, 50000, f);
  fclose(f);

  // END NEW

  ProcessEquipDat();

  f=fopen("MISCICON.DAT","rb");
  if (!f) err("Could not open MISCICON.DAT.");
  fread(&b,1,1,f);
  fread(&menuptr, 1, 256, f);
  fread(&itmptr, 1, 576, f);
  fread(&charptr, 1, 960, f);
  fclose(f);

  f=fopen("SPEECH.SPC","rb");
  if (!f) err("Could not open SPEECH.SPC");
  fread(&b,1,1,f);
  fread(speech, b, 1024, f);
  fclose(f);

  f=fopen("EFFECTS.VCS","rb");
  if (!f) err("Could not open EFFECTS.VCS");
  fread(&j, 1, 4, f);
  fread(&effectofstbl, 4, j, f);
  fread(effectvc, 1, 50000, f);
  fclose(f);

  f=fopen("STARTUP.VCS","rb");
  if (!f) err("Could not open STARTUP.VCS");
  fread(&j, 1, 4, f);
  fread(&startupofstbl, 4, j, f);
  fread(startupvc, 1, 50000, f);
  fclose(f);
}

void StartupMenu()
{ char cursel=0;
  int i,s;

drawloop:
  menubox(100,90,252,142);
  gotoxy(130,103);
  printstring("New Game");
  gotoxy(130,113);
  printstring("Load Game");
  gotoxy(130,123);
  printstring("Exit to DOS");

  if (!cursel) tcopysprite(110,102,16,16,(char*)menuptr);
  if (cursel==1) tcopysprite(110,112,16,16,(char*)menuptr);
  if (cursel==2) tcopysprite(110,122,16,16,(char*)menuptr);

  vgadump();
  while ((down) || (up)) readcontrols();

inputloop:
  readcontrols();
  if (down) { cursel++;
              if (cursel==3) cursel=0;
              playeffect(0);
              playeffect(0);
              goto drawloop; }
  if (up) { if (!cursel) cursel=2;
            else cursel--;
            playeffect(0);
            playeffect(0);
            goto drawloop; }

  if (!b1) goto inputloop;
  if (!cursel) StartNewGame("TEST.MAP");
  if (cursel==1) LoadSaveErase(0);
  if (cursel==2) { timer_count=0;
                   s=91;
fadeloop:          i=(timer_count*64)/s; i=64-i;
                   set_intensity(i);
                   if (timer_count<s) goto fadeloop; set_intensity(0);
                   err(""); }
}

int main()
{
  MiscSetup();
  PutOwnerText();
  initvga();
  InitItems();

  while (1)
  {
    qabort=0;
    /* -- ric: 01/Jun/98 --
     * These variables set to allow the vc layer functions to work
     * by preventing the engine from trying to draw a non-existant
     * map
     */
    cameratracking=0;
    layer0=0;
    layer1=0;
    drawparty=0;
    drawentities=0;

    StartupScript();
  }
  return(0);
}
