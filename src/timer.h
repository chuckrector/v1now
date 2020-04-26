#ifndef TIMER_H
#define TIMER_H

extern void timer_init();
extern void timer_close();
extern unsigned int timer_count, timer, hooktimer;
extern unsigned char an, tickctr, sec, min, hr;

#endif // TIMER_H