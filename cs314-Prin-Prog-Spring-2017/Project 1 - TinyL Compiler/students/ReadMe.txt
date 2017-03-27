Rafal Stapinski

The optimizer runs by first going through the instructions in order and
marking any output instructions as critical as well as the first loadi instr.
Then it goes backwards and for each instruction that is marked as critical,
goes backwards to find the immediately critical instructions necessary for it
to run. Memory is freed after all critical instructions are printed out by
traversing the linked list and freeing each piece of memory individually.
