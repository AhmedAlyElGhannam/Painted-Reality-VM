/* main.c */

#include "PaintedRealityVM.h"

int main(int argc, char *argv[]) 
{
    Program *prog;
    VM *vm;

    vm = virtualmachine();
    printf("vm = %p (size = %d)\n", vm, sizeof(VM));

    prog = dumdumprog(vm);
    printf("prog = %p\n", prog);

    // prints 01 00 05 02 
    //         ^ mov instruction
    //           ^ first arg
    //               ^ second arg
    //                  ^ nop instruction
    printhex($1 prog, (map_opcode_to_instr_size(mov) + map_opcode_to_instr_size(nop)), ' ');

    execute(vm);

    printf("ax = %.04hx\n", $i vm $ax);
    
    return 0;
}
