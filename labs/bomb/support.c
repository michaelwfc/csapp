/*
(gdb) disas initialize_bomb
Dump of assembler code for function initialize_bomb:
   0x00000000004013a2 <+0>:     sub    $0x8,%rsp                // move  stack pointer down 8 bytes 
   0x00000000004013a6 <+4>:     mov    $0x4012a0,%esi           // add returen address to stack
   0x00000000004013ab <+9>:     mov    $0x2,%edi
   0x00000000004013b0 <+14>:    call   0x400b90 <signal@plt>    // call function:
   0x00000000004013b5 <+19>:    add    $0x8,%rsp                // move stack pointer up 8 bytes
   0x00000000004013b9 <+23>:    ret 

*/
void initialize_bomb(){

}