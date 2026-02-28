/* main.c */

#include "PaintedRealityVM.h"

int main(int argc, char *argv[]) 
{
    Program *prog;
    VM *vm;
    int8 size;

    size = (map_opcode_to_instr_size(mov) + map_opcode_to_instr_size(nop));
    prog = dumdumprog();
    printf("prog = %p\n", prog);

    vm = virtualmachine(prog, size);
    printf("vm = %p\n", vm);

    return 0;
}
