/* PaintedRealityVM.h */

#ifndef PaintedRealityVM_H_
#define PaintedRealityVM_H_

#define _GNU_SOURCE

#include <spobutils/spobutils.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define ErrMem (0x01)
#define NoArgs { 0x00, 0x00 }

/*
    PaintedReality VM
        16-bit CPU
            AX
            BX
            CX
            DX
            SP
            IP (PC)
        65 KB memory -> 65536 possible memory addresses
        (Serial COM Port)
        (Floppy Disk Drive)
*/

typedef unsigned short int Reg;

struct s_registers
{
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;
    Reg sp;
    Reg ip;
};
typedef struct s_registers Registers;

struct s_cpu
{
    Registers r;
};
typedef struct s_cpu CPU;

typedef int8 Memory[((unsigned short int)(-1))]; // sequence/array or bytes

enum e_opcode
{
    mov = 0x01,
    nop = 0x02,
};
typedef enum e_opcode Opcode;

struct s_instrmap
{
    Opcode o;
    int8 s; // size
};
typedef struct s_instrmap IM;

typedef int8 Args;

struct s_instruction
{
    Opcode o;
    Args a[]; // 0-2 bytes args
};
typedef struct s_instruction Instruction;

typedef int8 Program;

struct s_vm
{
    CPU c;
    Memory m;
    int16 b; // break line
};
typedef struct s_vm VM;

typedef Memory *Stack;

static IM instrmap[] = {
    {mov, 0x03},
    {nop, 0x01}
};

#define IMs (sizeof(instrmap) / sizeof(struct s_instrmap))

Program *dumdumprog(VM*);
int8 map_opcode_to_instr_size(Opcode);
VM *virtualmachine(void);

/*                          r w x
    Section .text         | 1 0 1 | (grows down)
    #########             |       |
    #####                 |       |
    _______________________________
    _______________________________ break line between sections
    *******               |       |
    ********   ^          |       |
    *********  |          |       |
    Section .data         | 1 1 0 | (grows up)


*/

#endif
