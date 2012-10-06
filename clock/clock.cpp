
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <dos.h>
#include <bios.h>
#include <stdlib.h>

#define pi 3.1415927

int ResetMouse () {
  asm {
    xor ax, ax
    int 0x33
  }
  return _AX == 0xFFFF;
}

void ShowMouseCursor() {
  asm {
    mov ax, 1
    int 0x33
  }
}

void HideMouseCursor() {
  asm {
    mov ax, 2
    int 0x33
  }
}


void MoveMouseCursor(int x, int y) {
  asm {
    mov ax, 4
    mov cx, x
    mov dx, y
    int 0x33
  }
}

int main(void)
{
  struct MouseState a;
  int i;
  float s1 = 0, s2 = 0;
  float m1 = 0, m2 = 0;
  float h1 = 0, h2 = 0;
  struct time tim;
  float w, len;
  float xsr, ysr;
  int gdriver = DETECT, gmode, errorcode;
  initgraph(&gdriver, &gmode, "borlandc\\bgi\\");
  errorcode = graphresult();
  if(errorcode != grOk)
  {
    printf("Graphics error: %s\n", grapherrormsg(errorcode));
    printf("Press any key to halt:");
    getch();
    exit(1);
  }
  xsr = getmaxx() / 2;
  ysr = getmaxy() / 2;

  circle(xsr, ysr, 160);
  for(i = 0; i < 60; i++)
  {
    s1=sin(i * pi / 30);
    s2=cos(i * pi / 30);
    if(i % 5==0)
    {
      line(xsr+(160+0)*s2,ysr+(160+0)*s1,xsr+(160-7)*s2,ysr+(160-7)*s1);
    }
    line(xsr+(160+0)*s2,ysr+(160+0)*s1,xsr+(160-4)*s2,ysr+(160-4)*s1);
  }
  s1 = sin(pi - pi * tim.ti_sec / 30);
  s2 = cos(pi - pi * tim.ti_sec / 30);
  m1 = sin(pi - pi * tim.ti_min / 30);
  m2 = cos(pi - pi * tim.ti_min / 30);
  h1 = sin(pi - pi * tim.ti_hour / 30);
  h2 = cos(pi - pi * tim.ti_hour / 30);

  ResetMouse();
  ShowMouseCursor();
  MoveMouseCursor(0, 0);

  while(!kbhit())
  {
    gettime(&tim);
    setcolor(0);
    w = 1;
    len = 150;
    line(s2 * w + xsr, -s1 * w + ysr, s1 * len + xsr, s2 * len + ysr);
    line(xsr - w * s2, ysr + w * s1, len * s1 + xsr, len * s2 + ysr);
    line(xsr + w * s2, ysr - w * s1, xsr -w * s2, ysr + w * s1);
    s1 = sin(pi - pi * tim.ti_sec / 30);
    s2 = cos(pi - pi * tim.ti_sec / 30);
    setcolor(10);
    line(s2 * w + xsr, -s1 * w + ysr, s1 * len + xsr, s2 * len + ysr);
    line(xsr - w * s2, ysr + w * s1, len * s1 + xsr, len * s2 + ysr);
    line(xsr + w * s2, ysr - w * s1, xsr - w * s2, ysr + w * s1);

    setcolor(0);
    w = 4;
    len = 140;
    line(m2 * w + xsr, -m1 * w + ysr, m1 * len + xsr, m2 * len + ysr);
    line(xsr - w * m2, ysr + w * m1, len * m1 + xsr, len * m2 + ysr);
    line(xsr + w * m2, ysr - w * m1, xsr - w * m2, ysr + w * m1);
    m1 = sin(pi - pi * tim.ti_min / 30 - pi * tim.ti_sec / 30 / 60);
    m2 = cos(pi - pi * tim.ti_min / 30 - pi * tim.ti_sec / 30 / 60);
    setcolor(11);
    line(m2*w+xsr, -m1*w+ysr, m1*len+xsr, m2*len+ysr);
    line(xsr-w*m2, ysr+w*m1, len*m1+xsr, len*m2+ysr);
    line(xsr+w*m2, ysr-w*m1, xsr-w*m2, ysr+w*m1);

    setcolor(0);
    w = 4;
    len = 80;
    line(h2*w+xsr, -h1*w+ysr, h1*len+xsr, h2*len+ysr);
    line(xsr-w*h2, ysr+w*h1, len*h1+xsr, len*h2+ysr);
    line(xsr+w*h2, ysr-w*h1, xsr-w*h2, ysr+w*h1);
    h1 = sin(pi-pi*tim.ti_hour/6.0-(pi*tim.ti_min/30)/12);
    h2 = cos(pi-pi*tim.ti_hour/6.0-(pi*tim.ti_min/30)/12);
    setcolor(130);
    line(h2*w+xsr, -h1*w+ysr, h1*len+xsr, h2*len+ysr);
    line(xsr-w*h2, ysr+w*h1, len*h1+xsr, len*h2+ysr);
    line(xsr+w*h2, ysr-w*h1, xsr-w*h2, ysr+w*h1);
    delay(50);
    //printf("0x%X\r", _DS);
  }
  getch();
  HideMouseCursor();
  closegraph();
  return 0;
}
