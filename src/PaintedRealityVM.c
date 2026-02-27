/* PaintedRealityVM.c */
#include "PaintedRealityVM.h"

VM *
virtualmachine(Program* prog, int16 progsz)
{
    VM *p;
    Program *pp;
    int16 size;

    assert((prog) && (progsz));

    size = sizeof(struct s_vm);
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

    memcpy(pp, prog, progsz);

    // should initialize vm's program with pp

    goto out;

p_alloc_err:
    free(p);
vm_alloc_err:
    p = (VM *)0;
out:
    return p;
}
