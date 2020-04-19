#ifndef SOUND_H
#define SOUND_H

extern void sound_init();
extern void playsong(char *sngnme);
extern void stopsong();
extern void stopsound();
extern void playeffect(char efc);
extern char speed,moneycheat;
extern unsigned char mp_volume;
extern int md_device;
extern int md_mixfreq;
extern int md_dmabufsize;
extern int md_mode;
extern int mp_loop;
extern int md_numchn;

#endif
