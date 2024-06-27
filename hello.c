//#resource "nesbanked.cfg"
//#define CFGFILE nesbanked.cfg
//#link "chr_generic.s"

#define NES_MAPPER 4
#define	NES_PRG_BANKS 4
#define	NES_CHR_BANKS 2
#define	NES_MIRRORING 0

#include "neslib.h"
#include <string.h>
#include <peekpoke.h>

#define MMC_MODE 0x00

#define MMC3_SET_REG(r,n)\
	POKE(0x8000, MMC_MODE|(r));\
	POKE(0x8001, (n));

#define MMC3_CHR_0000(n) MMC3_SET_REG(0,n)
#define MMC3_CHR_0800(n) MMC3_SET_REG(1,n)
#define MMC3_CHR_1000(n) MMC3_SET_REG(2,n)
#define MMC3_CHR_1400(n) MMC3_SET_REG(3,n)
#define MMC3_CHR_1800(n) MMC3_SET_REG(4,n)
#define MMC3_CHR_1C00(n) MMC3_SET_REG(5,n)
#define MMC3_PRG_8000(n) MMC3_SET_REG(6,n)
#define MMC3_PRG_A000(n) MMC3_SET_REG(7,n)

#define MMC3_MIRROR(n) POKE(0xa000, (n))

const unsigned char palette_bg[]={
0x04, 0x03, 0x10, 0x30,
0x02, 0x03, 0x10, 0x30,
0x02, 0x03, 0x10, 0x30,
0x02, 0x03, 0x10, 0x30
};

const unsigned char debugString[] = "This is the debug string@"; //Called directly letter by letter
const unsigned char stringMain[] = "Hello main code segment@";
const unsigned char string3[] = "Hello Bank 3@";
const unsigned char string4[] = "Hello Bank 4@";
const unsigned char string5[] = "Hello Bank 5@";

#pragma code-name(push, "CODE0") //Fixed Bank 0x8000
void draw_text(word addr, char *text) {
  byte tx = 0;
  vram_adr(addr);
  for (tx = 0; text[tx]!='@'; ++tx)
  {
    vram_put(text[tx]);
  }
}

#pragma code-name(push, "CODE1") //Don't put anything here 0x8000
#pragma code-name(push, "CODE2") //Don't put anything here 0x8000


#pragma code-name(push, "CODE3") //Swappable Bank 0xA000
void FunctionCODE3()
{
  byte x;
  draw_text(NTADR_A(2, 15), "draw_text Bank 3@");
  vram_adr(NTADR_A(2,16));
  for (x = 0; string3[x]!='@';++x)
    vram_put(string3[x]);
}

#pragma code-name(push, "CODE4") //Swappable Bank 0xA000
void FunctionCODE4()
{
  byte x;
  draw_text(NTADR_A(2, 17), "draw_text Bank 4@");
  vram_adr(NTADR_A(2,18));
  for (x = 0; string4[x]!='@';++x)
    vram_put(string4[x]);
}

#pragma code-name(push, "CODE5") //Swappable Bank 0xA000
void FunctionCODE5()
{
  byte x;
  draw_text(NTADR_A(2, 19), "draw_text Bank 5@");
  vram_adr(NTADR_A(2,20));
  for (x = 0; string5[x]!='@';++x)
    vram_put(string5[x]);
}

#pragma code-name(push, "CODE")
byte x;

void main (void) 
{
  int *sp = (int*)0x0024;
  sp[0] = 0x0500;
  
  //MMC3_PRG_8000(0); //Fixed Bank?
  //MMC3_PRG_A000(3); //Swapable Bank
  
  pal_bg(palette_bg);
  vram_adr(NTADR_A(2,1));
  {
  for (x = 0; debugString[x]!='@';++x)
    vram_put(debugString[x]);
  
  draw_text(NTADR_A(2, 3), (char*)stringMain);
  vram_adr(NTADR_A(2,4));
  
  for (x = 0; stringMain[x]!='@';++x)
    vram_put(stringMain[x]);
  
  MMC3_PRG_A000(3);
  FunctionCODE3();
  
  MMC3_PRG_A000(4);
  FunctionCODE4();
  
  MMC3_PRG_A000(5);
  FunctionCODE5();
  
  ppu_on_all();

  while (1)
  {
    ++x;
    if (x > 8)
      x = 0;
    delay(15);
    pal_col(0,x);
    
  }
  }
}