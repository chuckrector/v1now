#ifndef MAIN_H
#define MAIN_H

extern char *strbuf;

extern void err(char *ermsg);
extern void LoadGame(char *fn);
extern void StartNewGame(char *startp);
extern void StartupMenu();

#endif // MAIN_H