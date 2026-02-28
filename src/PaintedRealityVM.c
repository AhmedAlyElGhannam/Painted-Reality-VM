/* PaintedRealityVM.c */
#include "PaintedRealityVM.h"

VM *virtualmachine(void)
{
    VM *p;
    // Program *pp;
    int16 size;

    // assert((prog) && (progsz));

    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if (!p)
    {
        errno = ErrMem;
        goto vm_alloc_err;
    }

    // initialize all fields in p to 0
    zero($1 p, size);

    // pp = (Program *)malloc($i progsz);
    // if (!pp)
    // {
    //     errno = ErrMem;
    //     goto p_alloc_err;
    // }

    // mem_cpy(pp, prog, progsz);

    goto out;

p_alloc_err:
    free(p);
vm_alloc_err:
    p = (VM *)0;
out:
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
    Instruction *i1, *i2;
    Args *a1; // i2 doesn't have args
    int16 s1, s2, argsz1;

    s1 = map_opcode_to_instr_size(mov);
    s2 = map_opcode_to_instr_size(nop);

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

    assert(i1 && i2);

    zero($1 i1, s1);
    zero($1 i2, s2);

    i1->o = mov;
    argsz1 = (s1 - 1);
    if (s1)
    {
        a1 = (Args *)malloc($i argsz1);
        if (!a1)
        {
            errno = ErrMem;
            free(i1);
            free(i2);
            return (Program *)0;
        }

        assert(a1);
        zero($1 a1, argsz1);

        *a1 = 0x00;
        (*(a1 + 1)) = 0x05;
    }

    prog = vm->m;
    mem_cpy($1 prog, $1 i1, 1);
    prog++;
    if (a1 && argsz1)
    {
        mem_cpy($1 prog, $1 a1, argsz1);
        prog += argsz1;
        free(a1);
    }

    i2->o = nop;
    mem_cpy($1 prog, $1 i2, 1);
    
    free(i1);
    free(i2);

    // set break line
    vm->b = $2(s1 + s2 + argsz1);

    // return ptr to top of memory aka start of program
    return ((Program *)&(vm->m));
}
