#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include<math.h>

#define WIDTH 1000
#define HEIGH 600
unsigned char DATA[256] = { 0 };
static unsigned char ROM[65535] = { 0 };
unsigned char SP = 7;
unsigned short PC = 0;
int CICLOS = 0;
unsigned short CSEG;

void next();
void Push();
unsigned char Pop();
void MOV();
void overflow(unsigned char x, unsigned char y);
void psw(int x);
void insere_bit(short num, unsigned char c);
unsigned char bit(int x);
void underflow_AC(unsigned char x, unsigned);
void ADD();
void RR();
void RL();
void RRC();
void RLC();
void SWAP();
unsigned char bit(int x);
unsigned char get_bit(unsigned char x, int num);
void ANL();
void ORL();
void XRL();
void RET();
void MUL();
void JZ();
void JNZ();
void JNB();
void JB();
void DJNZ();
void SUBB();
void DIV();
void DA();
void CPL();
void CLR();
void CJNE();
void INC();
void DEC();
void SJMP();
void AJMP();
void LJMP();
void ACALL();
void LCALL();
void ADDC();
void END();

struct instruction
{
	unsigned char opcode;
	unsigned char Flag_IMD : 1;
	unsigned char Flag_NUM_ENDER : 1;
	unsigned char nbytes;
	unsigned char nciclos;
	unsigned short cardoso : 13;
	void(*fn)(void);


};

struct instruction instruction_set[] = {
	{ 0x0, 0, 0, 0, 0, 0, END },
	{ 0x1, 0, 0, 0, 0, 0, NULL},
	{ 0x2, 0, 0, 0, 0, 0, NULL},
	{ 0x3, 0, 0, 0, 0, 0, NULL},
	{ 0x04, 0,0,1,1,0x400,INC },//INC do A
	{ 0x5, 0, 0, 0, 0, 0, NULL },
	{ 0x6, 0, 0, 0, 0, 0, NULL},
	{ 0x7, 0, 0, 0, 0, 0, NULL},
	{ 0x08, 0,0, 1, 1, 1,INC },//inc R0
	{ 0x09, 0, 0,1, 1, 2,INC },//inc R1
	{ 0x0A, 0, 0,1, 1, 4,INC },//inc R2
	{ 0x0B, 0, 0,1, 1, 8,INC },//inc R3
	{ 0x0C, 0, 0,1, 1, 16,INC },//inc R4
	{ 0x0D, 0, 0,1, 1, 32,INC },//inc R5 
	{ 0x0E, 0, 0,1, 1, 64,INC },//inc R6
	{ 0x0F, 0, 0,1, 1, 128,INC },//inc R7
	{ 0x10, 0, 0, 0, 0, 0, NULL },
	{ 0x11, 0, 0, 0, 0, 0, NULL},
	{ 0x12, 0, 0, 0, 0, 0, NULL},
	{ 0x13, 0, 0, 0, 0, 0, NULL},
	{ 0x14, 0,0, 1, 1,0x400, DEC },//DEC A
	{ 0x15, 0, 0, 0, 0, 0, NULL },
	{ 0x16, 0, 0, 0, 0, 0, NULL },
	{ 0x17, 0, 0, 0, 0, 0, NULL },
	{ 0x18, 0, 0,1, 1, 1,DEC },//DEC R0
	{ 0x19, 0, 0,1, 1, 2,DEC },//DEC R1
	{ 0x1A, 0, 0,1, 1, 4, DEC },//DEC R2
	{ 0x1B, 0, 0,1, 1, 8,DEC },//DEC R3
	{ 0x1C, 0, 0,1, 1, 16,DEC },//DEC R4
	{ 0x1D, 0, 0,1, 1, 32,DEC },//DEC R5
	{ 0x1E, 0, 0,1, 1, 64,DEC },//DEC R6
	{ 0x1F, 0, 0,1, 1, 128,DEC },//DEC R7
	{ 0x20, 0, 0, 0, 0, 0, NULL },
	{ 0x21, 0, 0, 0, 0, 0, NULL },
	{ 0x22, 0, 0, 0, 0, 0, NULL },
	{ 0x23, 0, 0, 0, 0, 0, NULL },
	{ 0x24, 0, 0, 0, 0, 0, NULL },
	{ 0x25, 0, 1,2, 1,0x1400, ADD },//ADD A,direct
	{ 0x26, 0, 0, 0, 0, 0, NULL },
	{ 0x27, 0, 0, 0, 0, 0, NULL },
	{ 0x28, 0, 1,1, 1,0x401, ADD },//ADD A,r0
	{ 0x29, 0, 1,1, 1,0x402, ADD },//ADD A,r1
	{ 0x2A, 0, 1,1, 1,0x404, ADD },//ADD A,r2
	{ 0x2B, 0, 1,1, 1,0x408, ADD },//ADD A,r3
	{ 0x2C, 0, 1,1, 1,0x410, ADD },//ADD A,r4
	{ 0x2D, 0, 1,1, 1,0x420, ADD },//ADD A,r5	
	{ 0x2E, 0, 1,1, 1,0x440, ADD },//ADD A,r6
	{ 0x2F, 0, 1,1, 1,0x480,ADD },//ADD A,r7
	{ 0x30, 0, 0, 0, 0, 0, NULL },
	{ 0x31, 0, 0, 0, 0, 0, NULL },
	{ 0x32, 0, 0, 0, 0, 0, NULL },
	{ 0x33, 0, 0, 0, 0, 0, NULL },
	{ 0x34, 0, 0, 0, 0, 0, NULL },
	{ 0x35, 0, 0, 0, 0, 0, NULL },
	{ 0x36, 0, 0, 0, 0, 0, NULL },
	{ 0x37, 0, 0, 0, 0, 0, NULL },
	{ 0x38, 0, 0, 0, 0, 0, NULL },
	{ 0x39, 0, 0, 0, 0, 0, NULL },
	{ 0x3a, 0, 0, 0, 0, 0, NULL },
	{ 0x3b, 0, 0, 0, 0, 0, NULL },
	{ 0x3c, 0, 0, 0, 0, 0, NULL },
	{ 0x3d, 0, 0, 0, 0, 0, NULL },
	{ 0x3e, 0, 0, 0, 0, 0, NULL },
	{ 0x3f, 0, 0, 0, 0, 0, NULL },
	{ 0x40, 0, 0, 0, 0, 0, NULL },
	{ 0x41, 0, 0, 0, 0, 0, NULL },
	{ 0x42, 0, 0, 0, 0, 0, NULL },
	{ 0x43, 0, 0, 0, 0, 0, NULL },
	{ 0x44, 0, 0, 0, 0, 0, NULL },
	{ 0x45, 0, 0, 0, 0, 0, NULL },
	{ 0x46, 0, 0, 0, 0, 0, NULL },
	{ 0x47, 0, 0, 0, 0, 0, NULL },
	{ 0x48, 0, 0, 0, 0, 0, NULL },
	{ 0x49, 0, 0, 0, 0, 0, NULL },
	{ 0x4a, 0, 0, 0, 0, 0, NULL },
	{ 0x4b, 0, 0, 0, 0, 0, NULL },
	{ 0x4c, 0, 0, 0, 0, 0, NULL },
	{ 0x4d, 0, 0, 0, 0, 0, NULL },
	{ 0x4e, 0, 0, 0, 0, 0, NULL },
	{ 0x4f, 0, 0, 0, 0, 0, NULL },
	{ 0x50, 0, 0, 0, 0, 0, NULL },
	{ 0x51, 0, 0, 0, 0, 0, NULL },
	{ 0x52, 0, 0, 0, 0, 0, NULL },
	{ 0x53, 0, 0, 0, 0, 0, NULL },
	{ 0x54, 0, 0, 0, 0, 0, NULL },
	{ 0x55, 0, 0, 0, 0, 0, NULL },
	{ 0x56, 0, 0, 0, 0, 0, NULL },
	{ 0x57, 0, 0, 0, 0, 0, NULL },
	{ 0x58, 0, 0, 0, 0, 0, NULL },
	{ 0x59, 0, 0, 0, 0, 0, NULL },
	{ 0x5a, 0, 0, 0, 0, 0, NULL },
	{ 0x5b, 0, 0, 0, 0, 0, NULL },
	{ 0x5c, 0, 0, 0, 0, 0, NULL },
	{ 0x5d, 0, 0, 0, 0, 0, NULL },
	{ 0x5e, 0, 0, 0, 0, 0, NULL },
	{ 0x5f, 0, 0, 0, 0, 0, NULL },
	{ 0x60, 0, 0, 2, 2, 0, JZ },//JZ offset
	{ 0x61, 0, 0, 0, 0, 0, NULL },
	{ 0x62, 0, 0, 0, 0, 0, NULL },
	{ 0x63, 0, 0, 0, 0, 0, NULL },
	{ 0x64, 0, 0, 0, 0, 0, NULL },
	{ 0x65, 0, 0, 0, 0, 0, NULL },
	{ 0x66, 0, 0, 0, 0, 0, NULL },
	{ 0x67, 0, 0, 0, 0, 0, NULL },
	{ 0x68, 0, 0, 0, 0, 0, NULL },
	{ 0x69, 0, 0, 0, 0, 0, NULL },
	{ 0x6a, 0, 0, 0, 0, 0, NULL },
	{ 0x6b, 0, 0, 0, 0, 0, NULL },
	{ 0x6c, 0, 0, 0, 0, 0, NULL },
	{ 0x6d, 0, 0, 0, 0, 0, NULL },
	{ 0x6e, 0, 0, 0, 0, 0, NULL },
	{ 0x6f, 0, 0, 0, 0, 0, NULL },
	{ 0x70, 0, 0, 2, 2, 0, JNZ },//JNZ offset
	{ 0x71, 0, 0, 0, 0, 0, NULL },
	{ 0x72, 0, 0, 0, 0, 0, NULL },
	{ 0x73, 0, 0, 0, 0, 0, NULL },
	{ 0x74, 1, 1, 2, 1, 0xC00, MOV },// 	MOV 	A, #immed
	{ 0x75, 1, 1, 3, 2, 0x1800, MOV },//MOV 	direct, #immed
	{ 0x76, 1, 1, 2, 1, 0xA00, MOV },//MOV 	@R0, #immed
	{ 0x77, 1, 1, 2, 1, 0x900, MOV },// 	MOV 	@R1, #immed
	{ 0x78, 1, 1, 2, 1, 0x801, MOV },//MOV 	R0, #immed
	{ 0x79, 1, 1, 2, 1, 0x802, MOV },// 	MOV 	R1, #immed
	{ 0x7A, 1, 1, 2, 1, 0x804, MOV },//MOV 	R2, #immed
	{ 0x7B, 1, 1, 2, 1, 0x808, MOV },//MOV 	R3, #immed
	{ 0x7C, 1, 1, 2, 1, 0x810, MOV },//MOV 	R4, #immed
	{ 0x7D, 1, 1, 2, 1, 0x820, MOV },//MOV 	R5, #immed
	{ 0x7E, 1, 1, 2, 1, 0x840, MOV },//MOV 	R6, #immed
	{ 0x7F, 1, 1, 2, 1, 0x880, MOV },//MOV 	R7, #immed
	{ 0x80, 0, 0, 2, 2, 0, SJMP },//SJMP offset
	{ 0x81, 0, 0, 0, 0, 0, NULL },
	{ 0x82, 0, 0, 0, 0, 0, NULL },
	{ 0x83, 0, 0, 0, 0, 0, NULL },
	{ 0x84, 0, 1, 1, 4, 0, DIV },//DIV AB
	{ 0x85, 0, 1, 3, 2, 0x1800, MOV },// 	MOV 	direct, direct
	{ 0x86, 0, 1, 2, 2, 0x1200, MOV },//MOV 	direct, @R0
	{ 0x87, 0, 1, 2, 2, 0x1100, MOV },//MOV 	direct, @R1
	{ 0x88, 0, 1, 2, 2, 0x1001, MOV },//MOV 	direct, R0
	{ 0x89, 0, 1, 2, 2, 0x1002, MOV },// 	MOV 	direct, R1
	{ 0x8A, 0, 1, 2, 2, 0x1004, MOV },//MOV 	direct, R2
	{ 0x8B, 0, 1, 2, 2, 0x1008, MOV },//MOV 	direct, R3
	{ 0x8C, 0, 1, 2, 2, 0x1010, MOV },//MOV 	direct, R4
	{ 0x8D, 0, 1, 2, 2, 0x1020, MOV },//MOV 	direct, R5
	{ 0x8E, 0, 1, 2, 2, 0x1040, MOV },//MOV 	direct, R6
	{ 0x8F, 0, 1, 2, 2, 0x1080, MOV },//MOV 	direct, R7
	{ 0x90, 0, 0, 0, 0, 0, NULL },
	{ 0x91, 0, 0, 0, 0, 0, NULL },
	{ 0x92, 0, 0, 0, 0, 0, NULL },
	{ 0x93, 0, 0, 0, 0, 0, NULL },
	{ 0x94, 0, 0, 0, 0, 0, NULL },
	{ 0x95, 0, 1, 2, 1, 0x1400, SUBB },//SUBB 	A, direct
	{ 0x96, 0, 0, 0, 0, 0, NULL },
	{ 0x97, 0, 0, 0, 0, 0, NULL },
	{ 0x98, 0, 1, 1, 1, 0x401, SUBB },//SUBB 	A, R0
	{ 0x99, 0, 1, 1, 1, 0x402, SUBB },//SUBB 	A, R1
	{ 0x9A, 0, 1, 1, 1, 0x404, SUBB },//SUBB 	A, R2
	{ 0x9B, 0, 1, 1, 1, 0x408, SUBB },//SUBB 	A, R3
	{ 0x9C, 0, 1, 1, 1, 0x410, SUBB },//SUBB 	A, R4
	{ 0x9D, 0, 1, 1, 1, 0x420, SUBB },//SUBB 	A, R5
	{ 0x9E, 0, 1, 1, 1, 0x440, SUBB },//SUBB 	A, R6
	{ 0x9F, 0, 1, 1, 1, 0x480, SUBB },//SUBB 	A, R7
	{ 0xa0, 0, 0, 0, 0, 0, NULL },
	{ 0xa1, 0, 0, 0, 0, 0, NULL },
	{ 0xa2, 0, 0, 0, 0, 0, NULL },
	{ 0xa3, 0, 0, 0, 0, 0, NULL },
	{ 0xA4, 0, 1, 1, 4, 0, MUL },// 	MUL 	AB
	{ 0xa5, 0, 0, 0, 0, 0, NULL },
	{ 0xA6, 0, 1, 2, 2, 0x1200, MOV },//MOV 	@R0, direct
	{ 0xA7, 0, 1, 2, 2, 0x1100, MOV },//MOV 	@R1, direct
	{ 0xA8, 0, 1, 2, 2, 0x1001, MOV },// 	MOV 	R0, direct
	{ 0xA9, 0, 1, 2, 2, 0x1002, MOV },// 	MOV 	R1, direct
	{ 0xAA, 0, 1, 2, 2, 0x1004, MOV },// 	MOV 	R2, direct
	{ 0xAB, 0, 1, 2, 2, 0x1008, MOV },// 	MOV 	R3, direct
	{ 0xAC, 0, 1, 2, 2, 0x1010, MOV },// 	MOV 	R4, direct
	{ 0xAD, 0, 1, 2, 2, 0x1020, MOV },// 	MOV 	R5, direct
	{ 0xAE, 0, 1, 2, 2, 0x1040, MOV },// 	MOV 	R6, direct
	{ 0xAF, 0, 1, 2, 2, 0x1080, MOV },// 	MOV 	R7, direct
	{ 0xb0, 0, 0, 0, 0, 0, NULL },
	{ 0xb1, 0, 0, 0, 0, 0, NULL },
	{ 0xb2, 0, 0, 0, 0, 0, NULL },
	{ 0xb3, 0, 0, 0, 0, 0, NULL },
	{ 0xB4, 1, 1, 3, 2, 0xC00, CJNE },//CJNE 	A, #immed, offset
	{ 0xB5, 0, 1, 3, 2, 0x1400, CJNE },// 	CJNE 	A, direct, offset
	{ 0xB6, 1, 1, 3, 2, 0xA00, CJNE },// 	CJNE 	@R0, #immed, offset
	{ 0xB7, 1, 1, 3, 2, 0x900, CJNE },//CJNE 	@R1, #immed, offset
	{ 0xB8, 1, 1, 3, 2, 0x801, CJNE },//CJNE 	R0, #immed, offset
	{ 0xB9, 1, 1, 3, 2, 0x802, CJNE },//CJNE 	R1, #immed, offset
	{ 0xBA, 1, 1, 3, 2, 0x804, CJNE },//CJNE 	R2, #immed, offset
	{ 0xBB, 1, 1, 3, 2, 0x808, CJNE },//CJNE 	R3, #immed, offset
	{ 0xBC, 1, 1, 3, 2, 0x810, CJNE },//CJNE 	R4, #immed, offset
	{ 0xBD, 1, 1, 3, 2, 0x820, CJNE },//CJNE 	R5, #immed, offset
	{ 0xBE, 1, 1, 3, 2, 0x840, CJNE },//CJNE 	R6, #immed, offset
	{ 0xBF, 1, 1, 3, 2, 0x880, CJNE },//CJNE 	R7, #immed, offset
	{ 0xc0, 0, 0, 0, 0, 0, NULL },
	{ 0xc1, 0, 0, 0, 0, 0, NULL },
	{ 0xc2, 0, 0, 0, 0, 0, NULL },
	{ 0xc3, 0, 0, 0, 0, 0, NULL },
	{ 0xc4, 0, 0, 0, 0, 0, NULL },
	{ 0xc5, 0, 0, 0, 0, 0, NULL },
	{ 0xc6, 0, 0, 0, 0, 0, NULL },
	{ 0xc7, 0, 0, 0, 0, 0, NULL },
	{ 0xc8, 0, 0, 0, 0, 0, NULL },
	{ 0xc9, 0, 0, 0, 0, 0, NULL },
	{ 0xca, 0, 0, 0, 0, 0, NULL },
	{ 0xcb, 0, 0, 0, 0, 0, NULL },
	{ 0xcc, 0, 0, 0, 0, 0, NULL },
	{ 0xcd, 0, 0, 0, 0, 0, NULL },
	{ 0xce, 0, 0, 0, 0, 0, NULL },
	{ 0xcf, 0, 0, 0, 0, 0, NULL },
	{ 0xd0, 0, 0, 0, 0, 0, NULL },
	{ 0xd1, 0, 0, 0, 0, 0, NULL },
	{ 0xd2, 0, 0, 0, 0, 0, NULL },
	{ 0xd3, 0, 0, 0, 0, 0, NULL },
	{ 0xd4, 0, 0, 0, 0, 0, NULL },
	{ 0xD5, 0, 0, 3, 2, 0x1000, DJNZ },// 	DJNZ 	direct, offset
	{ 0xd6, 0, 0, 0, 0, 0, NULL },
	{ 0xd7, 0, 0, 0, 0, 0, NULL },
	{ 0xD8, 0, 0, 2, 2, 1, DJNZ },//DJNZ 	R0, offset
	{ 0xD9, 0, 0, 2, 2, 2, DJNZ },//DJNZ 	R1, offset
	{ 0xDA, 0, 0, 2, 2, 4, DJNZ },//DJNZ 	R2, offset
	{ 0xDB, 0, 0, 2, 2, 0x8, DJNZ },//DJNZ 	R3, offset
	{ 0xDC, 0, 0, 2, 2, 0x10, DJNZ },//DJNZ 	R4, offset
	{ 0xDD, 0, 0, 2, 2, 0x20, DJNZ },//DJNZ 	R5, offset
	{ 0xDE, 0, 0, 2, 2, 0x40, DJNZ },//DJNZ 	R6, offset
	{ 0xDF, 0, 0, 2, 2, 0x80, DJNZ },//DJNZ 	R7, offset	
	{ 0xe0, 0, 0, 0, 0, 0, NULL },
	{ 0xe1, 0, 0, 0, 0, 0, NULL },
	{ 0xe2, 0, 0, 0, 0, 0, NULL },
	{ 0xe3, 0, 0, 0, 0, 0, NULL },
	{ 0xe4, 0, 0, 0, 0, 0, NULL },
	{ 0xE5, 0, 1, 2, 1, 0x1400, MOV },//MOV 	A, direct
	{ 0xE6, 0, 1, 1, 1, 0x600, MOV },//MOV 	A, @R0
	{ 0xE7, 0, 1, 1, 1, 0x500, MOV },//MOV 	A, @R1
	{ 0xE8, 0, 1, 1, 1, 0x401, MOV },//MOV 	A, R0
	{ 0xE9, 0, 1, 1, 1, 0x402, MOV },//MOV 	A, R1
	{ 0xEA, 0, 1, 1, 1, 0x404, MOV },//MOV 	A, R2
	{ 0xEB, 0, 1, 1, 1, 0x408, MOV },//MOV 	A, R3
	{ 0xEC, 0, 1, 1, 1, 0x410, MOV },//MOV 	A, R4
	{ 0xED, 0, 1, 1, 1, 0x420, MOV },//MOV 	A, R5
	{ 0xEE, 0, 1, 1, 1, 0x440, MOV },//MOV 	A, R6
	{ 0xEF, 0, 1, 1, 1, 0x480, MOV },//MOV 	A, R7
	{ 0xf0, 0, 0, 0, 0, 0, NULL },
	{ 0xf1, 0, 0, 0, 0, 0, NULL },
	{ 0xf2, 0, 0, 0, 0, 0, NULL },
	{ 0xf3, 0, 0, 0, 0, 0, NULL },
	{ 0xf4, 0, 0, 0, 0, 0, NULL },
	{ 0xF5, 0, 1, 2, 1, 0x1400, MOV },// 	MOV 	direct, A
	{ 0xF6, 0, 1, 1, 1, 0x600, MOV },//MOV 	@R0, A
	{ 0xF7, 0, 1, 1, 1, 0x500, MOV },//MOV 	@R1, A
	{ 0xF8, 0, 1, 1, 1, 0x401, MOV },// 	MOV 	R0, A
	{ 0xF9, 0, 1, 1, 1, 0x402, MOV },// 	MOV 	R1, A
	{ 0xFA, 0, 1, 1, 1, 0x404, MOV },// 	MOV 	R2, A
	{ 0xFB, 0, 1, 1, 1, 0x408, MOV },// 	MOV 	R3, A
	{ 0xFC, 0, 1, 1, 1, 0x410, MOV },// 	MOV 	R4, A
	{ 0xFD, 0, 1, 1, 1, 0x420, MOV },// 	MOV 	R5, A
	{ 0xFE, 0, 1, 1, 1, 0x440, MOV },// 	MOV 	R6, A
	{ 0xFF, 0, 1, 1, 1, 0x480, MOV }// 	MOV 	R7, A

};

//instrucoes
void END() {
	PC = 0;
}

void CJNE()
{
	unsigned char endereco, val, ofset;
	ofset = Pop();
	endereco = Pop();
	val = Pop();
	
	if ((DATA[endereco] - val))
		if (ofset > 0x7F)
			PC -= (0xFF - ofset);
		else
			PC += ofset;

}
void JNZ()
{
	unsigned char ofset = Pop();
	if (DATA[0xE0])
		if (ofset > 0x7F)
			PC -= (0xFF - ofset);
		else PC += ofset;
}
void JZ()
{
	unsigned char ofset = Pop();
	if (!DATA[0xE0])
		if (ofset > 0x7F)
			PC -= (0xFF - ofset);
		else
			PC += ofset;
}
void DJNZ()
{
	DATA[Pop()]--;
	unsigned char ofset = Pop();
	if (ofset > 0x7F)
		PC -= (0xFF - ofset);
	else
		PC += ofset;
}
void DIV()
{
	unsigned short aux;
	if (DATA[0xF0])
	{

		aux = (DATA[0xE0] / DATA[0xF0]);
		DATA[0xF0] = DATA[0xE0] - aux*DATA[0xF0];
		DATA[0xE0] = aux;
	}
	else
		psw(bit(2));
}
void MUL()
{
	unsigned short x = (DATA[0xE0] * DATA[0xF0]);
	if (x>0xFF)
	{
		DATA[0xF0] = (x & 0xFF00) >> 8;
		DATA[0xE0] = x & 0xFF;
	}
	else DATA[0xE0] *= DATA[0xF0];
}
void SJMP()
{
	unsigned char ofset = Pop();
	if (ofset > 0x7F)
		PC -= (0xFF - ofset);
	else
		PC += ofset;
	
}
void CLR_()
{
	DATA[0xE0] = 0;
}
void CPL()
{
	DATA[0xE0] = ~DATA[0xE0];
}
void RR()
{
	unsigned char c = get_bit(0xE0, 0);
	DATA[0xE0] >>= 1;
	if (c)
		insere_bit(0xE0, bit(7));
}
void RL()
{
	unsigned char c = get_bit(0xE0, 7);
	DATA[0xE0] <<= 1;
	if (c)
		insere_bit(0xE0, bit(0));
}
void RRC()
{
	unsigned char c1 = get_bit(0xE0, 0);
	unsigned char c2 = get_bit(0xD0, 7);
	DATA[0xE0] >>= 1;
	if (c2) {
		insere_bit(0xE0, bit(7));
	}
	insere_bit(0xD0, c1 << 7);
}
void RLC()
{
	unsigned char c1 = get_bit(0xE0, 7);
	unsigned char c2 = get_bit(0xD0, 7);
	DATA[0xE0] <<= 1;
	if (c2)
		insere_bit(0xE0, bit(0));
	insere_bit(0xD0, c1 << 7);
}
void SWAP()
{
	DATA[0xE0] = (DATA[0xE0] & 0x0F) << 4 | (DATA[0xE0] & 0xF0) >> 4;
}
unsigned char get_bit(unsigned char x, int num)//rever
{
	x = DATA[x] & bit(num);
	return x >>= num;
}
void underflow(unsigned char x, unsigned char y)
{
	if (y - x<0)
		psw(bit(7));
	underflow_AC(y, x);
}
void underflow_AC(unsigned char x, unsigned char y)
{
	if (((x &= 0x0F) - (y &= 0x0F))<0)
		psw(bit(6));
}
unsigned char bit(int x)
{
	return 1 << x;
}
void insere_bit(short num, unsigned char c)
{
	DATA[num] &= (~c);
	DATA[num] |= c;

}
void psw(int x)
{
	insere_bit(0xD0, x);
}
void overflow_AC(unsigned char x, unsigned char y)
{
	x &= 0xF;
	y &= 0xF;
	if (x + y>15)
		psw(bit(6));
}
void overflow(unsigned char x, unsigned char y)
{
	if (x + y>0xFF)
	{
		overflow_AC(x, y);
		psw(bit(7));
	}
}
void ADDC()
{
	DATA[0xE0] += get_bit(0xD0, 7);
	ADD();
}
void ADD()
{
	unsigned char endereco = Pop();
	overflow(DATA[0xE0], DATA[endereco]);
	DATA[0xE0] = DATA[0xE0] + DATA[endereco];
}
void INC()
{
	unsigned char endereco = Pop();
	overflow(DATA[endereco], 1);
	DATA[endereco]++;
}
void SUBB()
{
	unsigned char c = Pop();
	underflow(DATA[c], DATA[0xE0]);
	DATA[0xE0] = DATA[0xE0] - DATA[c];
}
void DEC()
{
	unsigned char endereco = Pop();
	underflow(1,DATA[endereco]);
	DATA[endereco]--;
}
void Push(unsigned char val)
{
	DATA[++SP] = val;
}
unsigned char Pop()
{
	return DATA[SP--];
}

void MOV()
{	
	unsigned char endereco = Pop();
	DATA[endereco] = Pop();
}

//ficheiro
void copy_string(int inferior, int superior, char *op)
{	
	int i = 0;
	char s[10];
	while (superior - inferior)
		s[i++] = op[inferior++];
	s[i] = '\0';
	ROM[PC++] = (unsigned char)(strtol(s, NULL, 16));

}

void le_fich()
{
	FILE *fp;
	char str_total[100];
	char str_cseg[10] = "";
	int i;
	fp = fopen("1.hex", "r");
	fseek(fp, 3 * sizeof(char), 0);
	for (int j = 3; j < 7; j++)
		sprintf(str_cseg, "%s%c", str_cseg, fgetc(fp));
	PC = strtol(str_cseg, NULL, 16);
	CSEG = PC;
	printf("\n\n%x\n\n", PC);
	fseek(fp, 0, 0);
	//abrir ficheiro e etc
	while ((fgets(str_total, sizeof(str_total), fp)) != NULL) {
		i = 9;
		while ((i + 1) < strlen(str_total) - 2){
			copy_string(i, i + 2, str_total);
			i += 2;
		}
	}

}


//grafica
struct butao {
	sfRectangleShape* but;
	sfText* tit;
};

struct janela {
	struct butao b;
	sfRenderWindow* box;
	int en;
	sfEvent evt;
};

void array_tabela(int val[], int size, char *val_str) {
	for (int i = 0; i < size; i++) {
		sprintf(val_str, "%s0x%0.2X\n", val_str, val[i]);
	}
}

void criar_butao(struct butao *butao, sfVector2f pos, char nome[]) {
	sfFont* arial;
	sfVector2f size = { 80, 30 };

	//butao
	sfRectangleShape_setSize(butao->but, size); //tamanho
	sfRectangleShape_setOutlineThickness(butao->but, 1);
	sfRectangleShape_setOutlineColor(butao->but, sfBlack);
	sfRectangleShape_setPosition(butao->but, pos); //posiçao
	//texto
	arial = sfFont_createFromFile("arial.ttf");
	sfText_setString(butao->tit, nome);
	sfText_setFont(butao->tit, arial);
	sfText_setCharacterSize(butao->tit, 20);
	sfText_setColor(butao->tit, sfBlack);
	pos.x += 5;
	pos.y += 2;
	sfText_setPosition(butao->tit, pos);
}

int butao_click(const sfRenderWindow* window, sfRectangleShape* butao)
{
	if (sfMouse_getPosition(window).x > sfRectangleShape_getPosition(butao).x &&
		sfMouse_getPosition(window).x < sfRectangleShape_getPosition(butao).x + sfRectangleShape_getSize(butao).x &&
		sfMouse_getPosition(window).y > sfRectangleShape_getPosition(butao).y &&
		sfMouse_getPosition(window).y < sfRectangleShape_getPosition(butao).y + sfRectangleShape_getSize(butao).y)
		return 1;
	else return 0;
}

char *update_memoria(unsigned char valores[]) {
	static char str[100];
	strcpy(str, "");
	for (int i = 0; i < 128; i++) {
		if (i % 16 == 0) sprintf(str, "%s\n%02X ", str, valores[i]);
		else sprintf(str, "%s%02X ", str, valores[i]);
	}
	return str;
}

char *update_tabela(unsigned char valores[], int size) {
	static char str[100];
	strcpy(str, "");
	for (int i = 0; i < size; i++) {
		sprintf(str, "%s 0x%0.2X\n", str, valores[i]);
	}
	return str;
}

char *update_sistema(unsigned char valores[], int size) {
	static char str[100];
	strcpy(str, "");
	for (int i = 0; i < size; i++) {
		if (i >= 7) { //bits
			if (i <= 10)
				sprintf(str, "%s       %d\n", str, (((DATA[208]) >> (i-7)) & 1));
			else 
				sprintf(str, "%s       %d\n", str, (((DATA[208]) >> (i - 6)) & 1));
		}
		else if (i==3)
			sprintf(str, "%s 0x%0.2X\n", str, CICLOS);
		else
			sprintf(str, "%s 0x%0.2X\n", str, valores[pos_sistema(i)]);
	}
	
	return str;
}

void update_PSW(int i, int valor) {
	if (valor == 0) {
		DATA[208] &= ~(1 << i);
	}
	else DATA[208] |= 1 << i;
}

char *update_portos(void) {
	static char str[100];
	strcpy(str, "");

	for (int i = 0; i < 4; i++) {
		for (int j = 7; j >= 0; j--)
			sprintf(str, "%s%d ", str, (((DATA[128 + (16 * i)]) >> (j)) & 1));
		sprintf(str, "%s\n\n", str);
	}

	return str;
}

void update_7seg(sfRectangleShape* display[]) {

	for (int i = 0; i < 8; i++) {
		if (((DATA[144] >> i) & 1) == 0)
			sfRectangleShape_setFillColor(display[i], sfRed);
		else sfRectangleShape_setFillColor(display[i], sfBlack);
	}

}

int pos_sistema(int pos) {
	int i;
	switch (pos) {
	case 0: i = 224; break;
	case 1: i = 240; break;
	case 2: i = 129; break;
	//case 3: i = 256; break; //PC
	case 4: i = 130; break;
	case 5: i = 131; break;
	//case 6: i = 258; break; //CYCLES
		//PSW
	case 7: i = 260; break;
	case 8: i = 261; break;
	case 9: i = 262; break;
	case 10: i = 263; break;
	case 11: i = 265; break;
	case 12: i = 266; break;
	case 13: i = 267; break;
	default:  i = 0; break;
	}

	return i;
}

void timer() {
	unsigned short int timer2 = 0;
	int tempo = 30;
	int flag = 0;
	flag = (DATA[200] >> 7) & 1;
	timer2 -= tempo;

	printf("%d\n", timer2);

	while (!flag) {
		timer2++;
		if (timer2 == 0) flag = 1;
	}
}

char *update_timer(void) {
	static char str[100];
	
	sprintf(str, "Flag:\t%d\n\nTimer:\t%d us", (DATA[200] >> 7) & 1, CICLOS);

	return str;
}

int graphics()
{
	sfVideoMode mode = { WIDTH, HEIGH, 32 };
	//janelas
	sfRenderWindow* window;
	struct janela timer;
	struct janela portos;
	struct janela disp_seg;
	//butoes
	struct butao ant;
	struct butao prox;
	//formas
	sfRectangleShape* display[8];
	sfRectangleShape* registos;
	sfRectangleShape* sistema;
	sfRectangleShape* memoria;
	//texto
	sfText* reg_tit;
	sfText* reg_num;
	sfText* reg_val;
	sfText* sys_tit;
	sfText* sys_num;
	sfText* sys_val;
	sfText* mem_val;
	sfText* mem_linha;
	sfText* mem_col;
	sfText* portos_tit;
	sfText* portos_val;
	sfText* timer_tit;

	//eventos
	sfEvent event;
	//
	sfVector2i pos = { 0,0 };
	sfVector2f rect_pos;
	sfVector2f size = { WIDTH, HEIGH };
	sfFont* arial;
	sfFont* monospace;
	clock_t start = clock(), stop;
	//cores
	sfColor Cinza_val_cor = sfColor_fromRGB(85, 85, 85);
	sfColor Cinza_fundo_cor = sfColor_fromRGB(240, 240, 240);

	//flags etc
	int valor_editar;
	int valor_original;
	int flag_text;
	char key_in[100] = "";

	//font
	arial = sfFont_createFromFile("arial.ttf");
	monospace = sfFont_createFromFile("monospace.ttf");

	//imagens
	sfSprite* sprite_fundo;
	sfTexture* textura_fundo;
	sfSprite* sprite_8051;
	sfTexture* textura_8051;

	/* Create the main window */
	window = sfRenderWindow_create(mode, "Simulador 8051", sfClose, NULL);
	sfRenderWindow_setPosition(window, pos);
	
	//timer_box
	mode.width = WIDTH / 5;
	mode.height = WIDTH / 5;
	timer.box = sfRenderWindow_create(mode, "Timer", sfTitlebar, NULL);
	pos.x = sfRenderWindow_getPosition(window).x + WIDTH + 5;
	pos.y = sfRenderWindow_getPosition(window).y;
	sfRenderWindow_setPosition(timer.box, pos);
	timer.en = 0;
	sfRenderWindow_setVisible(timer.box, timer.en);

	//portos_box
	portos.box = sfRenderWindow_create(mode, "Portos", sfTitlebar, NULL);
	pos.y += 40;
	sfRenderWindow_setPosition(portos.box, pos);
	portos.en = 0;
	sfRenderWindow_setVisible(portos.box, portos.en);

	//7-seg Display
	disp_seg.box = sfRenderWindow_create(mode, "7-Seg", sfTitlebar, NULL);
	pos.y += 40;
	sfRenderWindow_setPosition(disp_seg.box, pos);
	disp_seg.en = 0;
	sfRenderWindow_setVisible(disp_seg.box, disp_seg.en);
	
	//fundo
	textura_fundo = sfTexture_createFromFile("fundo.png", NULL);
	sprite_fundo = sfSprite_create();
	rect_pos.x = WIDTH - 350;
	rect_pos.y = HEIGH - 300;
	sfSprite_setPosition(sprite_fundo, rect_pos);
	size.x = 0.4;
	size.y = 0.4;
	sfSprite_setScale(sprite_fundo, size);
	sfSprite_setTexture(sprite_fundo, textura_fundo, 1);
	sfSprite_setColor(sprite_fundo, sfColor_fromRGBA(255, 255, 255, 128));

	//8051
	textura_8051 = sfTexture_createFromFile("8051.png", NULL);
	sprite_8051 = sfSprite_create();
	rect_pos.x = WIDTH / 3;
	rect_pos.y = HEIGH / 3;
	sfSprite_setPosition(sprite_8051, rect_pos);
	size.x = 0.9;
	size.y = 0.9;
	sfSprite_setScale(sprite_8051, size);
	sfSprite_setTexture(sprite_8051, textura_8051, 1);
	//sfSprite_setColor(sprite_fundo, sfColor_fromRGBA(255, 255, 255, 128));

	//butoes
	//time
	rect_pos.x = WIDTH - 100;
	rect_pos.y = 30;
	timer.b.but = sfRectangleShape_create();
	timer.b.tit = sfText_create();
	criar_butao(&timer.b, rect_pos, "Timer");

	//portos
	rect_pos.y += 30;
	portos.b.but = sfRectangleShape_create();
	portos.b.tit = sfText_create();
	criar_butao(&portos.b, rect_pos, "Portos");
	
	//7-seg
	rect_pos.y += 30;
	disp_seg.b.but = sfRectangleShape_create();
	disp_seg.b.tit = sfText_create();
	criar_butao(&disp_seg.b, rect_pos, "Display");

	//ant
	rect_pos.x = 400;
	rect_pos.y = HEIGH - 35;
	ant.but = sfRectangleShape_create();
	ant.tit = sfText_create();
	criar_butao(&ant, rect_pos, "Anterior");

	//prox
	rect_pos.x += sfRectangleShape_getSize(ant.but).x + 5;
	prox.but = sfRectangleShape_create();
	prox.tit = sfText_create();
	criar_butao(&prox, rect_pos, "Próximo");


	//draw segmentos
	display[0] = sfRectangleShape_create();
	rect_pos.y = WIDTH / 5 - 45;
	sfRectangleShape_setPosition(display[0], rect_pos); //posiçao
	sfRectangleShape_setFillColor(display[0], sfBlack);
	for (int i = 0; i < 8; i++) {
		display[i] = sfRectangleShape_copy(display[0]);
		//size
		if (i == 0 | i == 3 | i == 7) {
			size.x = 50;
			size.y = 15;
			rect_pos.x = 70;
			if (i == 3) rect_pos.y = sfRectangleShape_getPosition(display[0]).y - 65;
			if (i == 7) rect_pos.y = sfRectangleShape_getPosition(display[3]).y - 65;
		}
		else if (i == 1 | i == 4 | i == 5 | i == 6) {
			size.x = 15;
			size.y = 50;
			if (i == 1 | i == 6) rect_pos.x = sfRectangleShape_getPosition(display[0]).x - 20;
			if (i == 4 | i == 5) rect_pos.x = sfRectangleShape_getPosition(display[0]).x + sfRectangleShape_getSize(display[0]).x + 5;
			if (i == 1 | i == 4) rect_pos.y = sfRectangleShape_getPosition(display[0]).y - size.y;
			if (i == 5 | i == 6) rect_pos.y = sfRectangleShape_getPosition(display[1]).y - 65;
		}
		else {
			size.x = 15;
			size.y = 15;
			rect_pos.x = sfRectangleShape_getPosition(display[0]).x + sfRectangleShape_getSize(display[0]).x + 30;
			rect_pos.y = sfRectangleShape_getPosition(display[0]).y;
		}

		sfRectangleShape_setSize(display[i], size); //tamanho
		sfRectangleShape_setPosition(display[i], rect_pos); //posicao
	}
	update_7seg(display);

	//registos
	registos = sfRectangleShape_copy(timer.b.but);//retangulo
	size.x = 150;
	size.y = 180;
	sfRectangleShape_setSize(registos, size); //tamanho
	rect_pos.x = 2;
	rect_pos.y = 2;
	sfRectangleShape_setPosition(registos, rect_pos); //posiçao
	reg_tit = sfText_copy(timer.b.tit); //texto
	sfText_setString(reg_tit, "Registos:");
	sfText_setCharacterSize(reg_tit, 24);
	rect_pos.x = sfRectangleShape_getPosition(registos).x + 2;
	rect_pos.y = sfRectangleShape_getPosition(registos).y + 2;
	sfText_setPosition(reg_tit, rect_pos);
	reg_num = sfText_copy(timer.b.tit); //texto
	sfText_setString(reg_num, "R0\nR1\nR2\nR3\nR4\nR5\nR6\nR7");
	sfText_setCharacterSize(reg_num, 16);
	rect_pos.x = sfText_getPosition(reg_tit).x + 2;
	rect_pos.y = sfText_getPosition(reg_tit).y + 30;
	sfText_setPosition(reg_num, rect_pos);
	reg_val = sfText_copy(reg_num); //texto
	sfText_setString(reg_val, update_tabela(DATA, 8));
	sfText_setCharacterSize(reg_val, 16);
	rect_pos.x = sfText_getPosition(reg_tit).x + 100;
	rect_pos.y = sfText_getPosition(reg_tit).y + 30;
	sfText_setPosition(reg_val, rect_pos);
	sfText_setColor(reg_val, Cinza_val_cor);

	//sistema
	sistema = sfRectangleShape_copy(registos);//retangulo
	size.y = 290;
	sfRectangleShape_setSize(sistema, size); //tamanho
	rect_pos.x = 2;
	rect_pos.y = HEIGH / 2;
	sfRectangleShape_setPosition(sistema, rect_pos); //posiçao
	sys_tit = sfText_copy(reg_tit); //texto
	sfText_setString(sys_tit, "Sistema:");
	rect_pos.x = sfRectangleShape_getPosition(sistema).x + 2;
	rect_pos.y = sfRectangleShape_getPosition(sistema).y + 2;
	sfText_setPosition(sys_tit, rect_pos);
	sys_num = sfText_copy(reg_num); //texto
	sfText_setString(sys_num, "A\nB\nSP\nPC\nDPL\nDPH\nCYCLES\nP\nF1\nOV\nRS\nF0\nAC\nCY");
	rect_pos.x = sfText_getPosition(sys_tit).x + 2;
	rect_pos.y = sfText_getPosition(sys_tit).y + 30;
	sfText_setPosition(sys_num, rect_pos);
	sys_val = sfText_copy(sys_num); //texto
	sfText_setString(sys_val, update_sistema(DATA, 14));
	sfText_setCharacterSize(sys_val, 16);
	rect_pos.x = sfText_getPosition(sys_tit).x + 100;
	rect_pos.y = sfText_getPosition(sys_tit).y + 30;
	sfText_setPosition(sys_val, rect_pos);
	sfText_setColor(sys_val, Cinza_val_cor);

	//memorias
	char mem_str[1000] = "";
	memoria = sfRectangleShape_copy(registos);//retangulo
	rect_pos.x = 200;
	rect_pos.y = 2;
	sfText_setPosition(memoria, rect_pos);//pos geral
	size.x = 515;
	size.y = 185;
	sfRectangleShape_setSize(memoria, size); //tamanho
	mem_val = sfText_copy(reg_val); //texto
	sfText_setString(mem_val, update_memoria(DATA));
	sfText_setFont(mem_val, monospace);
	rect_pos.x = sfRectangleShape_getPosition(memoria).x + 40;
	rect_pos.y = sfRectangleShape_getPosition(memoria).y + 10;
	sfText_setPosition(mem_val, rect_pos);//pos
	mem_linha = sfText_copy(mem_val); //texto linha
	sfText_setString(mem_linha, " 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
	rect_pos.y = sfRectangleShape_getPosition(memoria).y;
	sfText_setPosition(mem_linha, rect_pos);//pos
	sfText_setColor(mem_linha, sfBlue);
	mem_col = sfText_copy(mem_linha); //texto col
	sfText_setString(mem_col, "\n0\n1\n2\n3\n4\n5\n6\n7");
	rect_pos.x = sfRectangleShape_getPosition(memoria).x + 5;
	rect_pos.y = sfText_getPosition(mem_val).y;
	sfText_setPosition(mem_col, rect_pos);//pos

	//portos
	portos_tit = sfText_copy(sys_num);
	sfText_setString(portos_tit, "P0\n\nP1\n\nP2\n\nP3");
	rect_pos.x = 20;
	rect_pos.y = 40;
	sfText_setPosition(portos_tit, rect_pos);//pos

	portos_val = sfText_copy(sys_val);
	rect_pos.x += 50;
	sfText_setPosition(portos_val, rect_pos);//pos
	sfText_setString(portos_val, update_portos());

	//timer
	timer_tit = sfText_copy(portos_tit);
	sfText_setString(timer_tit, update_timer());

	/* Start */
	while (sfRenderWindow_isOpen(window))
	{
		/* Process events */
		while (sfRenderWindow_pollEvent(window, &event))
		{
			/* Close window : exit */
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(window);
			}
			//mouse press
			if (event.type == sfEvtMouseButtonPressed) {
				if (sfMouse_isButtonPressed(sfMouseLeft)) {
					printf("%d %d\n", sfMouse_getPosition(window).x, sfMouse_getPosition(window).y);
					if (butao_click(window, timer.b.but)) {
						if (timer.en == 0)	timer.en = 1;
						else timer.en = 0;
						sfRenderWindow_setVisible(timer.box, timer.en);
					}
					if (butao_click(window, portos.b.but)) {
						if (portos.en == 0)	portos.en = 1;
						else portos.en = 0;
						sfRenderWindow_setVisible(portos.box, portos.en);
					}
					if (butao_click(window, disp_seg.b.but)) {
						if (disp_seg.en == 0)	disp_seg.en = 1;
						else disp_seg.en = 0;
						sfRenderWindow_setVisible(disp_seg.box, disp_seg.en);
					}
					if (butao_click(window, ant.but)) {
						//ant
						printf("ant\n");
					}
					if (butao_click(window, prox.but)) {
						//prox
						next();
						sfText_setString(reg_val, update_tabela(DATA, 8));
						sfText_setString(sys_val, update_sistema(DATA, 14));
						sfText_setString(mem_val, update_memoria(DATA));
						sfText_setString(portos_val, update_portos());
						update_7seg(display);
					}
					if (butao_click(window, memoria)) {
						stop = clock();
						if (stop - start <= 400) { //double click
							start = clock();
							for (int i = 0; i < 16; i++) {
								for (int j = 0; j < 8; j++) {
									if (sfMouse_getPosition(window).x > sfText_getPosition(mem_val).x + (30 * i) &&
										sfMouse_getPosition(window).x < sfText_getPosition(mem_val).x + (30 * (i + 1)) &&
										sfMouse_getPosition(window).y > sfText_getPosition(mem_val).y + 19 + (19 * j) &&
										sfMouse_getPosition(window).y < sfText_getPosition(mem_val).y + 19 + (19 * (j + 1))) {

										flag_text = 1;
										valor_editar = (j * 16) + i;
										valor_original = DATA[valor_editar];
										strcpy(key_in, "");
									}
								}
							}
						}
						else start = clock();		
					}
					if (butao_click(window, registos)) {
						stop = clock();
						if (stop - start <= 400) { //double click
							start = clock();
							for (int i = 0; i < 8; i++) {
								if (sfMouse_getPosition(window).x > sfText_getPosition(reg_val).x &&
									sfMouse_getPosition(window).x < sfText_getPosition(reg_val).x + 43 &&
									sfMouse_getPosition(window).y > sfText_getPosition(reg_val).y + (18 * i) &&
									sfMouse_getPosition(window).y < sfText_getPosition(reg_val).y + (18 * (i + 1))) {
									printf("%d\n", i);
									flag_text = 1;
									valor_editar = i;
									valor_original = DATA[i];
									strcpy(key_in, "");
								}
							}
						}
						else start = clock();
					}
					if (butao_click(window, sistema)) {
						stop = clock();
						if (stop - start <= 400) { //double click
							start = clock();
							for (int i = 0; i < 15; i++) {
								if (sfMouse_getPosition(window).x > sfText_getPosition(sys_val).x &&
									sfMouse_getPosition(window).x < sfText_getPosition(sys_val).x + 43 &&
									sfMouse_getPosition(window).y > sfText_getPosition(sys_val).y + (18 * i) &&
									sfMouse_getPosition(window).y < sfText_getPosition(sys_val).y + (18 * (i + 1))) {

									if (i != 3 && i != 6) {
										flag_text = 3;
										valor_editar = pos_sistema(i);
										valor_original = DATA[valor_editar];
										strcpy(key_in, "");
									}
								}
							}
						}
						else start = clock();
					}
				}
			}
			//text input
			if (event.type == sfEvtTextEntered) {
				switch (flag_text) {
				case 1: //memorias
					if (event.text.unicode < 128) {
						if (event.text.unicode == 13) flag_text = 0;
						else if (event.text.unicode == 27) {
							flag_text = 0;
							DATA[valor_editar] = valor_original;
						}
						else {
							sprintf(key_in, "%s%c", key_in, event.text.unicode);
							if (strtol(key_in, NULL, 16) <= 127)
								DATA[valor_editar] = strtol(key_in, NULL, 16);
							else DATA[valor_editar] = valor_original;
						}
					}
					sfText_setString(mem_val, update_memoria(DATA));
					sfText_setString(reg_val, update_tabela(DATA, 8));
					break;
				case 3:
					if (event.text.unicode < 128) {
						if (event.text.unicode == 13) flag_text = 0;
						else if (event.text.unicode == 27) {
							flag_text = 0;
							if (valor_editar < 256) DATA[valor_editar] = valor_original;
							else if (valor_editar >= 260) DATA[208] = valor_original;

						}
						else {
							sprintf(key_in, "%s%c", key_in, event.text.unicode);
							if (valor_editar < 256) {
								if (strtol(key_in, NULL, 16) <= 127) DATA[valor_editar] = strtol(key_in, NULL, 16);
								else DATA[valor_editar] = valor_original;
							}
							else if (valor_editar >= 260) {
								if (strtol(key_in, NULL, 16) <= 1) update_PSW(valor_editar - 260, strtol(key_in, NULL, 16));
								else DATA[208] = valor_original;
							}
						}
					}
					sfText_setString(sys_val, update_sistema(DATA, 14));
					sfText_setString(mem_val, update_memoria(DATA));
					break;
				}
				//if (event.text.unicode == 13) printf("\nenter\n");
				//if (event.text.unicode < 128) sprintf(key_in, "%s%c", key_in, event.text.unicode);
				//printf("%d\n", event.text.unicode);
			}
			if (event.type == sfEvtKeyPressed) {
				if (event.key.code == sfKeyF11) {
					//proximo
					next();
					sfText_setString(reg_val, update_tabela(DATA, 8));
					sfText_setString(sys_val, update_sistema(DATA, 14));
					sfText_setString(mem_val, update_memoria(DATA));
					sfText_setString(portos_val, update_portos());
					update_7seg(display);
				}
			}
		}

		while (sfRenderWindow_pollEvent(portos.box, &portos.evt)) {
			if (portos.evt.type == sfEvtMouseButtonPressed && sfMouse_isButtonPressed(sfMouseLeft)) {
				printf("Portos: %d %d\n", sfMouse_getPosition(portos.box).x, sfMouse_getPosition(portos.box).y);

				stop = clock();
				if (stop - start <= 400) { //double click
					printf("click");
					start = clock();
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 8; j++) {
							if (sfMouse_getPosition(portos.box).x > sfText_getPosition(portos_val).x + (13 * j) &&
								sfMouse_getPosition(portos.box).x < sfText_getPosition(portos_val).x + (13 * (j + 1)) &&
								sfMouse_getPosition(portos.box).y > sfText_getPosition(portos_val).y + (36 * i) &&
								sfMouse_getPosition(portos.box).y < sfText_getPosition(portos_val).y + (36 * (i + 1))) {
								printf("%d %d\n", i, j);

								DATA[128 + (16 * i)] ^= 1 << (7-j);
								sfText_setString(portos_val, update_portos());
								update_7seg(display);
								//for (int i = 0; i < 256; i++)	printf("%d\t", DATA[i]);
							}
						}
					}
				}
				else start = clock();
			}
		}

		while (sfRenderWindow_pollEvent(disp_seg.box, &disp_seg.evt)) {
			if (disp_seg.evt.type == sfEvtMouseButtonPressed && sfMouse_isButtonPressed(sfMouseLeft)) {
				printf("Display: %d %d\n", sfMouse_getPosition(disp_seg.box).x, sfMouse_getPosition(disp_seg.box).y);

				stop = clock();
				if (stop - start <= 400) { //double click
					printf("click");
					start = clock();
					for (int i = 0; i < 8; i++) {
						if (butao_click(disp_seg.box, display[i])) {

							DATA[144] ^= 1 << i;
							update_7seg(display);
							sfText_setString(portos_val, update_portos());

							//for (int i = 0; i < 256; i++)	printf("%d\t", DATA[i]);
						}
					}
				}
				else start = clock();
			}
		}

		/* Clear the screen */
		sfRenderWindow_clear(window, Cinza_fundo_cor);
		sfRenderWindow_clear(disp_seg.box, Cinza_fundo_cor);
		sfRenderWindow_clear(portos.box, Cinza_fundo_cor);
		sfRenderWindow_clear(timer.box, Cinza_fundo_cor);

		/* Draw */
		sfRenderWindow_drawSprite(window, sprite_fundo, NULL);
		sfRenderWindow_drawRectangleShape(window, timer.b.but, NULL);
		sfRenderWindow_drawText(window, timer.b.tit, NULL);
		sfRenderWindow_drawRectangleShape(window, portos.b.but, NULL);
		sfRenderWindow_drawText(window, portos.b.tit, NULL);
		sfRenderWindow_drawRectangleShape(window, disp_seg.b.but, NULL);
		sfRenderWindow_drawText(window, disp_seg.b.tit, NULL);
		//micro
		sfRenderWindow_drawSprite(window, sprite_8051, NULL);
		//reg
		sfRenderWindow_drawRectangleShape(window, registos, NULL);
		sfRenderWindow_drawText(window, reg_tit, NULL);
		sfRenderWindow_drawText(window, reg_num, NULL);
		sfRenderWindow_drawText(window, reg_val, NULL);
		//sys
		sfRenderWindow_drawRectangleShape(window, sistema, NULL);
		sfRenderWindow_drawText(window, sys_tit, NULL);
		sfRenderWindow_drawText(window, sys_num, NULL);
		sfRenderWindow_drawText(window, sys_val, NULL);
		//mem
		sfRenderWindow_drawRectangleShape(window, memoria, NULL);
		sfRenderWindow_drawText(window, mem_val, NULL);
		sfRenderWindow_drawText(window, mem_linha, NULL);
		sfRenderWindow_drawText(window, mem_col, NULL);
		//prox_ant
		sfRenderWindow_drawRectangleShape(window, prox.but, NULL);
		sfRenderWindow_drawText(window, prox.tit, NULL);
		//sfRenderWindow_drawRectangleShape(window, ant.but, NULL);
		//sfRenderWindow_drawText(window, ant.tit, NULL);
		// verifica 7seg - if color
		for (int i = 0; i < 8; i++)
			sfRenderWindow_drawRectangleShape(disp_seg.box, display[i], NULL);
		//portos
		sfRenderWindow_drawText(portos.box, portos_tit, NULL);
		sfRenderWindow_drawText(portos.box, portos_val, NULL);
		//timer
		sfRenderWindow_drawText(timer.box, timer_tit, NULL);

		/* Update the window */
		sfRenderWindow_display(window);
		sfRenderWindow_display(disp_seg.box);
		sfRenderWindow_display(portos.box);
		sfRenderWindow_display(timer.box);
	}

	/* Cleanup resources */
	sfRenderWindow_destroy(window);
	sfRenderWindow_destroy(timer.box);
	sfRenderWindow_destroy(portos.box);
	sfRenderWindow_destroy(disp_seg.box);

	return 0;
}

void next() {
	int bin[13] = { 0 };
	int n;
	printf("%x\n", instruction_set[ROM[PC]].opcode);
	n = instruction_set[ROM[PC]].cardoso;
	for (int i = 12; i >= 0; i--) {
		bin[i] = n % 2;
		n = n / 2;
	}

	if (instruction_set[ROM[PC]].cardoso != 0) {
		n = instruction_set[ROM[PC]].opcode;
		if (n == 0x75 | (n >= 0x85 && n <= 0x9F) | (n >= 0xE6 && n <= 0xEF) | n == 0xF5) {
			for (int i = 12; i >= 0; i--) {
				if (bin[i])
					switch (i) {
					case 0: Push(ROM[PC + 1]); break;
					case 1: Push(ROM[PC + 2]); break;
					case 2: Push(DATA[0xE0]); break;
					case 3: Push(DATA[0]); break;
					case 4: Push(DATA[1]); break;
					default: Push(12 - i);
					};
			}
		}
		else if (n == 0x4 | n == 0x14 | n == 0x25 | n == 0x74 | n == 0x95 | n == 0xB4 | n == 0xB5 | n == 0xE5) {
			for (int i = 0; i < 13; i++) {
				if (bin[i])
					switch (i) {
					case 0: Push(DATA[ROM[PC + 1]]); break;
					case 1: Push(ROM[PC + 1]); printf("valor"); break;
					case 2: Push(0xE0); printf("A");  break;
					case 3: Push(DATA[0]); break;
					case 4: Push(DATA[1]); break;
						//default: Push(12 - i);
					};
			}
		}
		else {
			for (int i = 0; i < 13; i++) {
				if (bin[i])
					switch (i) {
					case 0: Push(DATA[ROM[PC + 1]]); break;
					case 1: Push(ROM[PC + 1]); printf("valor"); break;
					case 2: Push(DATA[0xE0]); printf("A");  break;
					case 3: Push(DATA[0]); break;
					case 4: Push(DATA[1]); break;
					default: Push(12 - i);
					};
			}
		}
	}

	if (n == 0x60 | n == 0x80 | n == 0x70 | (n >= 0xB4 && n <= 0xBF) | (n >= 0xD5 && n <= 0xDF)) {
		Push(ROM[PC + instruction_set[ROM[PC]].nbytes - 1]);
	}

	instruction_set[ROM[PC]].fn();

	//Pop();
	printf("\nA-%x  R0-%x\n", DATA[0xE0], DATA[0]);
	PC += instruction_set[ROM[PC]].nbytes;
}

int main() {
	le_fich();
	PC = CSEG;
	next();
	graphics();

	return 0;
}