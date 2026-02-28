/* PaintedRealityVM.c */
#include "PaintedRealityVM.h"

VM *
virtualmachine(Program *prog, int16 progsz)
{
    VM *p;
    Program *pp;
    int16 size;

    assert((prog) && (progsz));

    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if (!p)
    {
        errno = ErrMem;
        goto vm_alloc_err;
    }

    // initialize all fields in p to 0
    zero($1 p, size);

    pp = (Program *)malloc($i progsz);
    if (!pp)
    {
        errno = ErrMem;
        goto p_alloc_err;
    }

    mem_cpy(pp, prog, progsz);

    // should initialize vm's program with pp

    goto out;

p_alloc_err:
    free(p);
vm_alloc_err:
    p = (VM *)0;
out:
    return p;
}

int8 
map_opcode_to_instr_size(Opcode op)
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
dumdumprog(void)
{
    int16 size;
    Instruction i1, i2;
    static Program prog[2];

    size = map_opcode_to_instr_size(mov);
    i1 = (Instruction)malloc($i size);
    if (!i1)
    {
        errno = ErrMem;
        return (Program *)0;
    }
    
    size = map_opcode_to_instr_size(nop);
    i2 = (Instruction)malloc($i size);
    if (!i2)
    {
        errno = ErrMem;
        free(i1);
        return (Program *)0;
    }
    
    prog[0] = i1;
    prog[1] = i2;
    
    return prog;
}