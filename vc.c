// vc.c
// All VergeC interpretation routines.
// Copyright (C)1997 BJ Eirich

#include <stdio.h>
#include "engine.h"
#include "keyboard.h"
#include "vc.h"

extern char *strbuf;
extern int vcbufm;

// NEW: MAGIC
char *magicvc;
// END NEW

char *mapvc;                           // MAP VC code buffer
char *effectvc,*startupvc;             // Effect VC code buffer / Startup.vcs

char *vcdatabuf;                       // The data buffer for VC use
char *code, *basevc;                   // current code data ptr
unsigned int scriptofstbl[1024];       // Map script offset table
unsigned int effectofstbl[1024];       // Effect script offset table
unsigned int magicofstbl[1024];        // Magic script offset table - ric: 08/May/98 - who forgot this then?
unsigned int startupofstbl[1024];      // Startup script offset table
int numscripts;                        // Number of scripts in current map VC
unsigned int varl[10];                 // Chain/call pass variables
unsigned int tvar[26];                 // Temporary/Throwaway variables
char killvc=0;                         // abort VC loop;

InitVCMem()
  {
  // -- aen; 31/May/98 -- otf mem allocation
  //mapvc=valloc(mapvcm,"mapvc");

  effectvc = valloc(25000,"effectvc");
  startupvc = valloc(25000,"startupvc");
  vcdatabuf = valloc(vcbufm,"vcdatabuf");
  }

LoadVC(FILE *f)
  {
  int b,e,mapvcs;

  fread(&numscripts, 1, 4, f);
  fread(&scriptofstbl, 4, numscripts, f);

  // -- aen; 31/May/98 -- otf (on the fly) mem allocation
  b=ftell(f);             // get beginning offset of map vc code
  fseek(f, 0, SEEK_END);  // seek to end of map vc code
  e=ftell(f);             // get end offset of map vc code
  fseek(f, b, SEEK_SET);  // reset file pos
  mapvcs=e-b+1;           // calc total map vc code size (in bytes)
  vfree(mapvc);           // free mapvc mem (if necessary)
  mapvc = valloc(mapvcs, "LoadVC:mapvc"); // allocate necessary mapvc mem
  fread(mapvc, mapvcs, 1, f); // read in the map vc code from disk
  }

unsigned char GrabC()
{ unsigned char c;

  c=*code;
  code++;

  return c;
}

unsigned short int GrabW()
{ unsigned short int c;
  unsigned short int *ptr;

  ptr=code;
  c=*ptr;
  code+=2;

  return c;
}

unsigned int GrabD()
{ unsigned int c;
  unsigned int *ptr;

  ptr=code;
  c=*ptr;
  code+=4;

  return c;
}

GrabString(char *str)
{ int i;

  i=0;
  while (*code)
  { str[i]=*code;
    code++;
    i++;
  }
  str[i]=0;
  code++;

}

ProcessVar0Assign()
{ int t,w;
  unsigned char b;

  t=GrabC();
  b=GrabC();
  switch (b)
  { case SET: WriteVar0(t,ResolveOperand()); return;
    case INCSET: w=ReadVar0(t); w+=ResolveOperand(); WriteVar0(t,w); return;
    case DECSET: w=ReadVar0(t); w-=ResolveOperand(); WriteVar0(t,w); return;
    case INCREMENT: w=ReadVar0(t); w++; WriteVar0(t,w); return;
    case DECREMENT: w=ReadVar0(t); w--; WriteVar0(t,w); return;
  }
}

ProcessVar1Assign()
{ int t,a,w;
  unsigned char b;

  t=GrabC();
  a=ResolveOperand();
  b=GrabC();
  switch (b)
  { case SET: WriteVar1(t,a,ResolveOperand()); return;
    case INCSET: w=ReadVar1(t,a); w+=ResolveOperand(); WriteVar1(t,a,w); return;
    case DECSET: w=ReadVar1(t,a); w-=ResolveOperand(); WriteVar1(t,a,w); return;
    case INCREMENT: w=ReadVar1(t,a); w++; WriteVar1(t,a,w); return;
    case DECREMENT: w=ReadVar1(t,a); w--; WriteVar1(t,a,w); return;
  }
}

ProcessVar2Assign()
{ int t,a,c,w;
  unsigned char b;

  t=GrabC();
  a=ResolveOperand();
  c=ResolveOperand();
  b=GrabC();
  switch (b)
  { case SET: WriteVar2(t,a,c,ResolveOperand()); return;
    case INCSET: w=ReadVar2(t,a,c); w+=ResolveOperand(); WriteVar2(t,a,c,w); return;
    case DECSET: w=ReadVar2(t,a,c); w-=ResolveOperand(); WriteVar2(t,a,c,w); return;
    case INCREMENT: w=ReadVar2(t,a,c); w++; WriteVar2(t,a,c,w); return;
    case DECREMENT: w=ReadVar2(t,a,c); w--; WriteVar2(t,a,c,w); return;
  }
}

unsigned int ResolveOperand()
{ unsigned int cr;
  unsigned char c;

  cr=ProcessOperand();              // Get base number
  while (1)
  {
     c=GrabC();
     switch (c)
     {
        case ADD: cr += ProcessOperand(); continue;
        case SUB: cr -= ProcessOperand(); continue;
        case DIV: cr = cr / ProcessOperand(); continue;
        case MULT: cr = cr * ProcessOperand(); continue;
        case MOD: cr = cr % ProcessOperand(); continue;
        case OP_END: break;
     }
     break;
  }
  return cr;
}

int ProcessOperand()
{ unsigned char op_desc;
  int a,b,c;

  op_desc=GrabC();
  switch (op_desc)
  {
      case OP_IMMEDIATE: return GrabD();
      case OP_VAR0: return ReadVar0(GrabC());
      case OP_VAR1: a=GrabC();
                    b=ResolveOperand();
                    return ReadVar1(a,b);
      case OP_VAR2: a=GrabC();
                    b=ResolveOperand();
                    c=ResolveOperand();
                    return ReadVar2(a,b,c);
      case OP_GROUP: return ResolveOperand();
  }
}

ProcessIf()
{ unsigned int elseofs;
  unsigned char numargs,i;
  unsigned short int arg1, arg2;
  char controlbyte;

  char exec=1,varidx,varparam;

  numargs=GrabC();                     // Get number of arguements in this IF.
  elseofs=GrabD();                     // Get jumplocation if IF is false.

  for (i=0; i<numargs; i++)
      {
            arg1=ResolveOperand();
            controlbyte=GrabC();

            if (controlbyte==ZERO || controlbyte==NONZERO)
            {
                switch (controlbyte)
                { case ZERO: if (arg1) exec=0; break;
                  case NONZERO: if (!arg1) exec=0; break; }
            } else
            {
                arg2=ResolveOperand();
                switch (controlbyte)
                { case EQUALTO: if (arg1 != arg2) exec=0; break;
                  case NOTEQUAL: if (arg1 == arg2) exec=0; break;
                  case GREATERTHAN: if (arg1 <= arg2) exec=0; break;
                  case GREATERTHANOREQUAL: if (arg1 < arg2) exec=0; break;
                  case LESSTHAN: if (arg1 >= arg2) exec=0; break;
                  case LESSTHANOREQUAL: if (arg1 > arg2) exec=0; break;
                }
            }

            if (!exec)
               { code=basevc+elseofs;
                 return; }
      }
}

ProcessFor0()
{
  unsigned int vidx=0,min,max,incv,curval,t;
  unsigned char incs,c;
  unsigned int bptr;

  vidx=GrabC();

  min=ResolveOperand();
  max=ResolveOperand();
  incs=GrabC();
  incv=ResolveOperand();
  bptr=code;

  WriteVar0(vidx,min);
  curval=min;

execloop:
  code=bptr;

  ExecuteBlock();

  t=ReadVar0(vidx);
  if (incs) { t+=incv; curval+=incv; }
       else { t-=incv; curval-=incv; }
  WriteVar0(vidx,t);

  // Now determine if we've passed the min/max

  if (!incs && curval > min) return;
  if (incs && curval <= max) goto execloop;
  if (!incs && curval >= max) goto execloop;
}

ProcessFor1()
{
  unsigned int vidx=0,min,max,incv,curval,t;
  unsigned char incs,c;
  unsigned int bptr,varg;

  vidx=GrabC();
  varg=ResolveOperand();

  min=ResolveOperand();
  max=ResolveOperand();
  incs=GrabC();
  incv=ResolveOperand();
  bptr=code;

  WriteVar1(vidx,varg,min);
  curval=min;

execloop:
  code=bptr;

  ExecuteBlock();

  t=ReadVar1(vidx,varg);
  if (incs) { t+=incv; curval+=incv; }
       else { t-=incv; curval-=incv; }
  WriteVar1(vidx,varg,t);

  // Now determine if we've passed the min/max

  if (!incs && curval > min) return;
  if (incs && curval <= max) goto execloop;
  if (!incs && curval >= max) goto execloop;
}

ProcessSwitch()
{ int realvalue,compvalue;
  unsigned char c, *next;

  realvalue=ResolveOperand();
  c=GrabC();
  while (c!=ENDSCRIPT)
  {
    compvalue=ResolveOperand();
    next=GrabD();
    if (compvalue!=realvalue)
    {
      (int) code=(int) basevc+(int) next;
      c=GrabC();
      continue;
    }
    ExecuteBlock();
    c=GrabC();
  }
}

ExecuteScript(unsigned short int s)
{ unsigned char c;

  basevc=mapvc;
  code=mapvc+scriptofstbl[s];

  ExecuteBlock();
}

ExecuteHookedScript(unsigned short int s)
{ char *codeb,abortvcb,*basevcb;

  codeb=code;
  abortvcb=killvc;
  basevcb=basevc;
  ExecuteScript(s);
  killvc=abortvcb;
  code=codeb;
  basevc=basevcb;
}

// NEW: MAGIC

ExecuteMagicEffect(unsigned short int s)
{ unsigned char c;

  basevc=magicvc;
  code=magicvc+magicofstbl[s]; /* - ric: 08/May/98 - code fixed - */

  ExecuteBlock();
}


// END NEW

ExecuteEffect(unsigned short int s)
{ unsigned char c;

  basevc=effectvc;
  code=effectvc+effectofstbl[s];

  ExecuteBlock();
}

ExecuteBlock()
{ unsigned char c;

  while (1)
  {
        if (killvc) { killvc=0; break; }
        c=GrabC();
        if (c==EXEC) { ExecLibFunc(GrabC()); continue; }
        if (c==VAR0_ASSIGN) { ProcessVar0Assign(); continue; }
        if (c==VAR1_ASSIGN) { ProcessVar1Assign(); continue; }
        if (c==VAR2_ASSIGN) { ProcessVar2Assign(); continue; }
        if (c==GENERAL_IF) { ProcessIf(); continue; }
        if (c==FOR_LOOP0) { ProcessFor0(); continue; }
        if (c==FOR_LOOP1) { ProcessFor1(); continue; }
        if (c==GOTO) { code=basevc+GrabD(); continue; }
        if (c==SWITCH) { ProcessSwitch(); continue; }
        if (c==ENDSCRIPT) break;
  }
}

StartupScript()
{
  basevc=startupvc;
  code=startupvc;

  ExecuteBlock();
}
