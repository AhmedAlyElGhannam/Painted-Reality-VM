/* PaintedRealityVM.c */
#include "PaintedRealityVM.h"

VM *virtualmachine(void)
{
    VM *p;
    int16 size;

    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if (!p)
    {
        errno = ErrMem;
        p = (VM *)NULL;
    }

    // initialize all fields in p to 0
    zero($1 p, size);

    return p;
}

int8 map_opcode_to_instr_size(Opcode op)
{
    int8 n, ret;
    IM *p;

    ret = 0;
    for (n = IMs, p = instrmap; n; n--, p++)
    {
        if (p->o == op)
        {
            ret = p->s;
            break;
        }
    }

    return ret;
}

Program *
dumdumprog(VM *vm)
{
    Program *prog;
    Instruction *i1, *i2, *i3;
    Args a1; // i2 doesn't have args
    int16 s1, s2, s3, argsz1;

    s1 = map_opcode_to_instr_size(mov);
    s2 = map_opcode_to_instr_size(nop);
    s3 = map_opcode_to_instr_size(hlt);

    a1 = 0x0000;

    i1 = (Instruction *)malloc($i s1);
    if (!i1)
    {
        errno = ErrMem;
        return (Program *)0;
    }

    i2 = (Instruction *)malloc($i s2);
    if (!i2)
    {
        errno = ErrMem;
        free(i1);
        return (Program *)0;
    }

    i3 = (Instruction *)malloc($i s3);
    if (!i3)
    {
        errno = ErrMem;
        free(i2);
        free(i1);
        return (Program *)0;
    }

    assert(i1 && i2 && i3);

    zero($1 i1, s1);
    zero($1 i2, s2);
    zero($1 i3, s3);

    i1->o = mov;
    argsz1 = (s1 - 1);
    if (s1)
    {
        a1 = 0x0005;
    }

    prog = vm->m;
    mem_cpy($1 prog, $1 i1, 1);
    prog++;
    if (argsz1)
    {
        mem_cpy($1 prog, $1(&a1), argsz1);
        prog += argsz1;
    }

    i2->o = nop;
    mem_cpy($1 prog, $1 i2, 1);
    prog++;

    i3->o = hlt;
    mem_cpy($1 prog, $1 i3, 1);

    free(i1);
    free(i2);
    free(i3);

    // set break line
    vm->b = $2(s1 + s2 + s3 + argsz1);

    // set pc register to first instruction
    vm $pc = (Reg)vm->m;

    // set stack ptr to last possible mem address
    vm $sp = (Reg)(-1);

    // return ptr to top of memory aka start of program
    return ((Program *)&(vm->m));
}

void error(VM *vm, Errorcode e)
{
    int8 stat;

    stat = -1;

    switch (e)
    {
        case ErrSegv:
            fprintf(stderr, "%s\n", "VM Segmentation Fault.");
        break;

        case SysHlt:
            fprintf(stderr, "%s\n", "System Halted.");
            stat = 0;
            printf("ax = %.04hx", $i vm $ax);
        break;

    default:
        break;
    }

    if (vm)
        free(vm);

    exit($i stat);
}

void __mov(VM *vm, Opcode opcode, Args a1, Args a2)
{
    vm $ax = (Reg)a1;
    return;
}

void exec_intr(VM *vm, Instruction *i)
{
    Args a1, a2;
    int16 size;

    assert(vm && i);

    size = map_opcode_to_instr_size(i->o);
    a1 = 0x0000;
    a2 = 0x0000;

    switch (size)
    {
        case 1:
        break;

        case 2:
            a1 = i->a[0];
        break;

        case 3:
            a1 = i->a[0];
            a2 = i->a[1];
        break;

        default:
            segfault(vm);
        break;
    }

    switch (i->o)
    {
        case mov:
            __mov(vm, i->o, a1, a2);
        break;

        case nop:
            // do nothing
        break;

        case hlt:
            error(vm, SysHlt);
        break;

        default:
            segfault(vm);
        break;
    }

    return;
}

void execute(VM *vm)
{
    int32 brkaddr;
    Program *pp;
    Instruction ip;
    int16 size = 0;

    // check if vm exists + vm mem has an instruction at first byte
    assert(vm && (*(vm->m)));
    brkaddr = (((int32)(vm->m)) + (vm->b));
    pp = (Program *)(&(vm->m));

    /* mov ax 0x05; nop; hlt; */
    // 0x01 0x00 0x05; 0x02; 0x03;

    do
    {
        vm $pc += size;
        pp += size;
        ip.o = (*pp);

        // if (vm $pc > vm->b)
        if (((int32)pp) > (brkaddr))
        {
            segfault(vm);
        }

        size = map_opcode_to_instr_size((Opcode)(*pp));
        ip.a[0] = *(pp + 1);
        ip.a[1] = *(pp + 3);
        exec_intr(vm, &ip); // execute single instruction        
    } while ((*pp) != ((Opcode)hlt));

    return;
}
