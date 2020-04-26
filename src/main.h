#ifndef MAIN_H
#define MAIN_H

extern char *strbuf;

extern void err(const char *ermsg);
extern void LoadGame(const char *fn);
extern void StartNewGame(const char *startp);
extern void StartupMenu();

#endif // MAIN_H