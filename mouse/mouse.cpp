#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <dos.h>
#include <bios.h>
#include <stdlib.h>

#define MOUSE_MOVE_MASK 0x01
#define MOUSE_LBUTTON_PRESS 0x02
#define MOUSE_LBUTTON_RELEASE 0x04
#define MOUSE_RBUTTON_PRESS 0x08
#define MOUSE_RBUTTON_RELEASE 0x10
#define MOUSE_MBUTTON_PRESS 0x20
#define MOUSE_MBUTTON_RELEASE 0x40
#define MOUSE_ALL_EVENTS 0x7F

struct MouseState {
	int x, y;
	int Buttons;
};

struct CursorShape {
	unsigned AndMask [16];
	unsigned XorMask [16];
	int HotX, HotY;
};

typedef void(*MouseHandler)(int, int, int, int);
int ResetMouse();
void ShowMouseCursor();
void HideMouseCursor();
void ReadMouseState(MouseState&);
void MoveMouseCursor(int, int );
void SetMouseHorzRange(int, int );
void SetMouseVertRange(int, int);
void SetMouseShape(CursorShape&);
void SetHideRange(int, int, int, int);
void SetMouseHandler(MouseHandler, int = MOUSE_ALL_EVENTS);
void RemoveMouseHandler();

static MouseHandler CurHandler = NULL;
int DoneFlag = 0;

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

void ReadMouseState(MouseState& s) {
	asm {
		mov ax, 3
		int 0x33
	}
	#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
	asm {
		push es
		push di
		les di, dword ptr s
		mov es:[di], cx
		mov es:[di+2], dx
		mov es:[di+4], bx
		pop di
		pop es
	}
	#else
	asm {
		push di
		mov di, word ptr s
		mov [di], cx
		mov [di+2], dx
		mov [di+4], bx
		pop di
	}
	#endif
}

void MoveMouseCursor(int x, int y) {
	asm {
		mov ax, 4
		mov cx, x
		mov dx, y
		int 0x33
	}
}

void SetHorzMouseRange(int xmin, int xmax) {
	asm {
		mov ax, 7
		mov cx, xmin
		mov dx, xmax
		int 0x33
	}
}

void SetVertMouseRange(int ymin, int ymax) {
	asm {
	mov ax, 8
	mov cx, ymin
	mov dx, ymax
	int 0x33
	}
}

void SetMouseShape(CursorShape& c) {
	#if defined(__COMPACT ) || defined(__LARGE__) || defined(__HUGE__)
	asm {
		push es
		push di
		les di, dword ptr c
		mov bx, es:[di+16]
		mov cx, es:[di+18]
		mov dx, di
		mov ax, 9
		int 0x33
		pop di
		pop es
	}
	#else
	asm {
		push  di
		mov di, word ptr c
		mov bx, [di+16]
		mov cx, [di+18]
		mov dx, di
		mov ax, 9
		int 0x33
		pop di
	}
	#endif
}

void SetHideRange(int x1, int y1, int x2, int y2) {
	asm {
		push si
		push di
		mov ax, 0x10
		mov cx, x1
		mov dx, y1
		mov si, x2
		mov di, y2
		int 0x33
		pop di
		pop si
	}
}

static void far MouseStub() {
	asm {
		push ds // preserve ds
		push ax // preserve ax
		mov ax, seg CurHandler
		mov ds, ax
		pop ax // restore ax
		push dx // y
		push cx // x
		push bx // button state
		push ax // event mask
		call CurHandler
		add sp, 8 // clear stack
		pop ds
	}
}

void SetMouseHandler(MouseHandler h, int mask) {
	void far *p = MouseStub;
	CurHandler = h;
	asm {
		push es
		mov ax, 0x0C
		mov cx, mask
		les dx, p
		int 0x33
		pop es
	}
}

void RemoveMouseHandler () {
	CurHandler = NULL;
	asm {
		mov ax, 0x0C
		mov cx, 0
		int 0x33
	}
}

CursorShape c = {
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 1, 1,
1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1,
1, 1, 1, 1};

void SetVideoMode(int mode) {
asm {
	mov ax, mode
	int 0x10
	}
}

void WaitPress(int mask, int buttons, int x, int y) {
	if(mask == 0x02) {
		DoneFlag = 1;
	}
	printf("mask - 0x%x\tbutton - 0x%x\tx = %d\ty = %d\r", mask, buttons, x, y);
}

void main(void) {
	struct MouseState a;
	SetVideoMode(0x12);
	ResetMouse();
	ShowMouseCursor();
	MoveMouseCursor(0, 0);
	SetMouseShape(c);
	SetMouseHandler(WaitPress);
	while(!DoneFlag) {
	}
	RemoveMouseHandler();
	HideMouseCursor();
	SetVideoMode(3);
}