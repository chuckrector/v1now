// battle.c
// All battle-related routines.
// Copyright (C)1997 BJ Eirich

#include "control.h"
#include "engine.h"
#include "keyboard.h"
#include "menu.h"
#include "render.h"
#include "timer.h"
#include "vga.h"
#include <stdio.h>
#include <stdlib.h> // rand
#include <string.h> // strcpy

FILE *d;

char
whotoattack()
{
  unsigned char t1, t2;

  if (numchars == 1)
    return partyidx[0] - 1;

tryagain:
  t1 = rand();
  t2 = (255 / numchars) + 1;
  t1 = (t1 / t2);
  if (t1 == numchars)
    goto tryagain;
  if (pstats[partyidx[t1]].status == 1)
    goto tryagain;

  return partyidx[t1] - 1;
}

int
random(int min, int max)
{
  int i;

rnd1:
  i = (rand() % (max + 1));
  if ((i >= min) && (i <= max))
    return i;
  else
    goto rnd1;
}

int
getnum()
{
  int i, tmin, tmax;

  fscanf(d, "%d", &tmin);
  fscanf(d, "%d", &tmax);
  i = random(tmin, tmax);
  return i;
}

int
getsnum()
{
  int i, tmin;

  fscanf(d, "%d", &tmin);
  i = tmin;
  return i;
}

void
levelup(int chr)
{
  int i, i2, i3, tmin, tmax, j;
  int alreadyhave;
  unsigned char *img;

  d = fopen("PARTY.DAT", "r");
  fscanf(d, "%s", strbuf);
  for (i = 0; i <= chr; i++)
  {
    fscanf(d, "%s", strbuf);
    fscanf(d, "%s", strbuf);
    fscanf(d, "%s", strbuf);
  }
  fclose(d);

  d = fopen(strbuf, "r");
  fgets(strbuf, 99, d);
  fgets(strbuf, 99, d);

  pstats[chr].lv++;
  for (i = 0; i < pstats[chr].lv; i++)
    fgets(strbuf, 99, d);

  fscanf(d, "%d", &pstats[chr].nxt);
  i = getnum();
  pstats[chr].maxhp += i;
  pstats[chr].curhp += i;
  i = getnum();
  pstats[chr].maxmp += i;
  pstats[chr].curmp += i;
  i = getnum();
  pstats[chr].str += i;
  i = getnum();
  pstats[chr].end += i;
  i = getnum();
  pstats[chr].mag += i;
  i = getnum();
  pstats[chr].mgr += i;
  i = getnum();
  pstats[chr].hit += i;
  i = getnum();
  pstats[chr].dod += i;
  i = getnum();
  pstats[chr].mbl += i;
  i = getnum();
  pstats[chr].fer += i;
  i = getnum();
  pstats[chr].rea += i;

  i = getsnum();

  alreadyhave = 0;
  i3 = 0;

  while (i3 < 24)
  {
    if (i == pstats[chr].maginv[i3])
      alreadyhave = 1;
    i3++;
  }

  if (!alreadyhave)
  {
    img = magicicons + (magic[i].icon * 256);
    while (!b1 && !b2 && !b4 && !b4)
    {
      drawmap();
      gotoxy(130, 86);
      strcpy(strbuf, pstats[chr].name);
      strcat(strbuf, " has learned");
      tmenubox(120, 80, 120 + (strlen(strbuf) * 10), 118);
      printstring(strbuf);
      tcopysprite(200, 96, 16, 16, img);
      vgadump();
      readcontrols();
    }

    j = pstats[chr].magcnt;
    if (j != 24)
    {
      pstats[chr].maginv[j] = i;
      pstats[chr].magcnt++;
    }
    else
      pstats[chr].maginv[j - 1] = i;
  }

  fclose(d);

  UpdateEquipStats();
}

void
battle()
{
  unsigned char t1;

  t1 = whotoattack();

  if (pstats[t1].curhp <= 2)
    pstats[t1].curhp = 0;
  else
  {
    pstats[t1].curhp -= 2;
    pstats[t1].exp += 2;
  }

  gp += 5;
  if (pstats[t1].curhp < 1)
    pstats[t1].status = 1;
  if (pstats[t1].nxt <= pstats[t1].exp)
    levelup(t1);
}
