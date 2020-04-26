#ifndef SOUND_H
#define SOUND_H

extern char speed;
extern char moneycheat;
extern unsigned char mp_volume;
extern int md_device;
extern int md_mixfreq;
extern int md_dmabufsize;
extern int md_mode;
extern int mp_loop;
extern int md_numchn;

extern void playeffect(char efc);
extern void playsong(char *sngnme);
extern void sound_init();
extern void stopsong();
extern void stopsound();

#endif
