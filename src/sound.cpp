// sound.c
// RPG <-> MikMod interfacing code
// Copyright (C)1997 BJ Eirich

#include "control.h"
#include "engine.h"
#include "keyboard.h"
#include "main.h"
#include "render.h"
#include "vc.h"
#include "vga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char mp_volume;
int md_device;
int md_mixfreq;
int md_dmabufsize;
int md_mode;
int mp_loop;
int md_numchn;
#define DMODE_STEREO 0
#define DMODE_16BITS 1
#define DMODE_INTERP 2

void
delay(int n)
{
}

// UNIMOD *mf;
// SAMPLE *sfx[30];
char numfx;
char curch;
char jf;
// char waitvrt,speed=0,moneycheat=0;
char speed;
// char moneycheat;
// int mapvcm; // -- aen; 31/May/98 -- no longer used; see LoadVC().
// int vcbufm;
// int mapm; // -- aen; 30/May/98 -- no longer used; see load_map().

char playing = 0;
char playingsong[13];

void
tickhandler()
{
  // MP_HandleTick();
  // MD_SetBPM(mp_bpm);
}

void
ParseSetup()
{
  FILE *s;

  if (!(s = fopen("SETUP.CFG", "r")))
  {
    printf("Could not open SETUP.CFG - Using defaults.\n");
    delay(2000);
    return;
  }

parseloop:
  if ((keyboard_map[SCAN_ALT]) && (keyboard_map[SCAN_CTRL]) &&
      (keyboard_map[SCAN_DEL]))
  {
    err("Exiting: CTRL-ALT-DEL pressed.");
  }

  fscanf(s, "%s", strbuf);
  if (!strcmp(strbuf, "waitvrt"))
  {
    fscanf(s, "%s", strbuf);
    waitvrt = atoi(strbuf);
    goto parseloop;
  }
  //  if (!strcmp(strbuf,"vsp"))    // -- aen; 31/May/98 -- no longer used;
  //     { fscanf(s,"%s",strbuf);   // -- see load_map().
  //       vspm=atoi(strbuf);
  //       goto parseloop; }
  //  if (!strcmp(strbuf,"map"))    // -- aen; 30/May/98 -- no longer used;
  //     { fscanf(s,"%s",strbuf);   // -- see load_map().
  //       mapm=atoi(strbuf);
  //       goto parseloop; }
  //  if (!strcmp(strbuf,"mapvc"))  // -- aen; 31/May/98 -- no longer used;
  //     { fscanf(s,"%s",strbuf);   // -- see LoadVC().
  //       mapvcm=atoi(strbuf);
  //       goto parseloop; }
  if (!strcmp(strbuf, "vcbuf"))
  {
    fscanf(s, "%s", strbuf);
    vcbufm = atoi(strbuf);
    goto parseloop;
  }
  if (!strcmp(strbuf, "speeddemon"))
  {
    speed = 1;
    goto parseloop;
  }
  if (!strcmp(strbuf, "meseta"))
  {
    moneycheat = 1;
    goto parseloop;
  }
  if (!strcmp(strbuf, "vspspeed"))
  {
    vspspeed = 1;
    goto parseloop;
  }

  if (!strcmp(strbuf, "joystick"))
  {
    fscanf(s, "%s", strbuf);
    jf = atoi(strbuf);
    goto parseloop;
  }
  if (!strcmp(strbuf, "kb1"))
  {
    fscanf(s, "%s", &kb1);
    goto parseloop;
  }
  if (!strcmp(strbuf, "kb2"))
  {
    fscanf(s, "%s", &kb2);
    goto parseloop;
  }
  if (!strcmp(strbuf, "kb3"))
  {
    fscanf(s, "%s", &kb3);
    goto parseloop;
  }
  if (!strcmp(strbuf, "kb4"))
  {
    fscanf(s, "%s", &kb4);
    goto parseloop;
  }
  if (!strcmp(strbuf, "jb1"))
  {
    fscanf(s, "%s", &jb1);
    goto parseloop;
  }
  if (!strcmp(strbuf, "jb2"))
  {
    fscanf(s, "%s", &jb2);
    goto parseloop;
  }
  if (!strcmp(strbuf, "jb3"))
  {
    fscanf(s, "%s", &jb3);
    goto parseloop;
  }
  if (!strcmp(strbuf, "jb4"))
  {
    fscanf(s, "%s", &jb4);
    goto parseloop;
  }

  if (!strcmp(strbuf, "sounddevice"))
  {
    fscanf(s, "%s", strbuf);
    md_device = atoi(strbuf);
    goto parseloop;
  }
  if (!strcmp(strbuf, "mixrate"))
  {
    fscanf(s, "%s", strbuf);
    md_mixfreq = atoi(strbuf);
    goto parseloop;
  }
  if (!strcmp(strbuf, "dmabufsize"))
  {
    fscanf(s, "%s", strbuf);
    md_dmabufsize = atoi(strbuf);
    goto parseloop;
  }
  if (!strcmp(strbuf, "forcemono"))
  {
    md_mode &= ~DMODE_STEREO;
    goto parseloop;
  }
  if (!strcmp(strbuf, "force8bit"))
  {
    md_mode &= ~DMODE_16BITS;
    goto parseloop;
  }
  if (!strcmp(strbuf, "interpolate"))
  {
    md_mode |= DMODE_INTERP;
    goto parseloop;
  }
  if (!strcmp(strbuf, "volume"))
  {
    fscanf(s, "%s", strbuf);
    mp_volume = atoi(strbuf);
    goto parseloop;
  }

  fclose(s);
}

int lastvol;

void
sound_init()
// Incidently, sound_init() also initializes the control interface, since
// they both use SETUP.CFG.
{
  char t;

  md_mixfreq = 44100;
  md_dmabufsize = 2048;
  md_mode = DMODE_16BITS | DMODE_STEREO;
  md_device = 0;
  mp_volume = 100;

  // vspm=256000;
  // mapvcm=50000; // -- aen; 31/May/09 -- no longer used; see LoadVC().
  // vcbufm=250000;
  // mapm=150000; // -- aen; 30/May/09 -- no longer used; see load_map().

  kb1 = 28;
  kb2 = 56;
  kb3 = 1;
  kb4 = 57;
  jb1 = 1;
  jb2 = 2;
  jb3 = 3;
  jb4 = 4;
  waitvrt = 0;
  jf = 0;
  vspspeed = 0;

  ParseSetup();
  allocbuffers();
  initcontrols(jf);
  playingsong[0] = 0;

  // ML_RegisterLoader(&load_s3m);
  // ML_RegisterLoader(&load_uni);
  // ML_RegisterLoader(&load_xm);
  // ML_RegisterLoader(&load_mod);
  // MD_RegisterDriver(&drv_nos);
  // MD_RegisterDriver(&drv_sb);
  // MD_RegisterDriver(&drv_gus);
  // MD_RegisterPlayer(tickhandler);

  // if(!MD_Init()) {
  //   printf("Driver error: %s.\n",myerr);
  //   exit(1); }
  lastvol = mp_volume;
}

void
sound_loadsfx(char *fname)
{
  FILE *f;
  char i;

  if (md_device == 3)
  {
    return;
  }
  numfx = 0;
  if (!(f = fopen(fname, "r")))
  {
    err("Could not open sound effect index file.");
  }
  fscanf(f, "%s", strbuf);
  numfx = atoi(strbuf);

  for (i = 0; i < numfx; i++)
  {
    fscanf(f, "%s", strbuf);
    // if (!(sfx[i]=MW_LoadWavFN(strbuf)))
    err("WAV load error.");
  }
  fclose(f);
}

void
sound_freesfx()
{
  char i;

  // for (i=0; i<numfx; i++)
  //     MW_FreeWav(sfx[i]);
}

void stopsound();
void playeffect(char efc);
void
playsong(char *sngnme)
{
  if (md_device == 3)
  {
    return;
  }
  if (!strcmp(sngnme, playingsong))
  {
    return;
  }
  memcpy(&playingsong, sngnme, 13);

  if (playing)
  {
    stopsound();
  }
  // if (!(mf=ML_LoadFN(sngnme))) err(myerr);
  // MP_Init(mf);
  // md_numchn=mf->numchn+2;
  mp_loop = 1;
  mp_volume = lastvol;
  sound_loadsfx("MAIN.SFX");
  // MD_PlayStart();
  playing = 1;
  playeffect(numfx - 1);
  playeffect(numfx - 1);
}

void
stopsound()
{
  if (md_device == 3)
  {
    return;
  }

  lastvol = mp_volume;
  mp_volume = 0;
  // if (MP_Ready()) return;
  if (!playing)
  {
    return;
  }
  // MD_PlayStop();
  sound_freesfx();
  // ML_Free(mf);
  playing = 0;
}

void
playeffect(char efc)
{
  char chanl;

  if (md_device == 3)
  {
    return;
  }
  if (!playing)
  {
    return;
  }

  chanl = md_numchn - curch;
  if (curch == 1)
  {
    curch = 2;
  }
  else
  {
    curch = 1;
  }

  // MD_VoiceSetVolume(chanl,64);
  // MD_VoiceSetPanning(chanl,128);
  // MD_VoiceSetFrequency(chanl,sfx[efc]->loopend);
  // MD_VoicePlay(chanl,sfx[efc]->handle,0,sfx[efc]->length,0,0,sfx[efc]->flags);
}

/*playsound(char *fname,int rate)
{ char chanl;

  chanl=md_numchn-curch;
  if (curch==1) curch=2; else curch=1;

  sfx1=MW_LoadWavFN(fname);
  MD_VoiceSetVolume(chanl,64);
  MD_VoiceSetPanning(chanl,128);
  MD_VoiceSetFrequency(chanl,rate);
  MD_VoicePlay(chanl,sfx1->handle,0,sfx1->length,0,0,sfx1->flags);
} */
