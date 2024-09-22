/*
(gdb) disas phase_1
Dump of assembler code for function phase_1:
   0x0000000000400ee0 <+0>:     sub    $0x8,%rsp
   0x0000000000400ee4 <+4>:     mov    $0x402400,%esi                // set input to %esi 
   0x0000000000400ee9 <+9>:     call   0x401338 <strings_not_equal>  // x = strings_not_equal()
   0x0000000000400eee <+14>:    test   %eax,%eax                     // if(x==0) ,%eax store the return value
   0x0000000000400ef0 <+16>:    je     0x400ef7 <phase_1+23>         // recursive run phase_1  
   0x0000000000400ef2 <+18>:    call   0x40143a <explode_bomb>       
   0x0000000000400ef7 <+23>:    add    $0x8,%rsp
   0x0000000000400efb <+27>:    ret    
End of assembler dump.

(gdb) x/s 0x402400
0x402400:       "Border relations with Canada have never been better."

*/

input ="Border relations with Canada have never been better."

void phase_1(input)
{
    return ;
}



/*
(gdb) disas phase_2
Dump of assembler code for function phase_2:
   0x0000000000400efc <+0>:     push   %rbp                        // pushes the current value of %rbp onto the stack  %rbp =2
   0x0000000000400efd <+1>:     push   %rbx                        //                                                  %rbx=0x7fffffffe128 -> -84
   0x0000000000400efe <+2>:     sub    $0x28,%rsp                  // sub stack pointer down to 28 bytes               %rsp=0x7fffffffdff8(->91) -0x28 =  0x7fffffffdfd0 -> 40 
   0x0000000000400f02 <+6>:     mov    %rsp,%rsi                   // %rsi is argu2 = 0x6037d0  using x/s $rsi :  (input_strings)"1 2 3 4 5 6" =  0x7fffffffdfd0 ->40 ?
   0x0000000000400f05 <+9>:     call   0x40145c <read_six_numbers> // call  read_six_numbers()  the return value  %rax=6, %rsp changed because of line<+6> 
   0x0000000000400f0a <+14>:    cmpl   $0x1,(%rsp)                 // if( *p ==1)    %rsp is the first num from input ? %rsp=0x7fffffffdfd0 -> 1 ?
   0x0000000000400f0e <+18>:    je     0x400f30 <phase_2+52>       //    { +52 }
   0x0000000000400f10 <+20>:    call   0x40143a <explode_bomb>     // else {explode_bomb}
   0x0000000000400f15 <+25>:    jmp    0x400f30 <phase_2+52>       //   
   0x0000000000400f17 <+27>:    mov    -0x4(%rbx),%eax             // return value = value at %rbx -4               %rbx -4 = 0x7fffffffdfd8-4= 0x7fffffffdfd4 -> 1, 2 ? 
   0x0000000000400f1a <+30>:    add    %eax,%eax                   // 2 * return value 
   0x0000000000400f1c <+32>:    cmp    %eax,(%rbx)                 // if(%eax == %rbx=2)
   0x0000000000400f1e <+34>:    je     0x400f25 <phase_2+41>            {  +41 }
   0x0000000000400f20 <+36>:    call   0x40143a <explode_bomb>     // else{explode_bomb()}
   0x0000000000400f25 <+41>:    add    $0x4,%rbx                   // %rbx +4  =0x7fffffffdfd8 +4= 0x7fffffffdfd8 
   0x0000000000400f29 <+45>:    cmp    %rbp,%rbx                   // if( value at %rbx != value %rbp)
   0x0000000000400f2c <+48>:    jne    0x400f17 <phase_2+27>       //    { +27}
   0x0000000000400f2e <+50>:    jmp    0x400f3c <phase_2+64>       // else{ +64}
   0x0000000000400f30 <+52>:    lea    0x4(%rsp),%rbx               // %rsp+4 address to %rbx  (callee save registers) %rbx = 0x7fffffffdfd0 + 4 -> 2 ? 
   0x0000000000400f35 <+57>:    lea    0x18(%rsp),%rbp              // %rsp+18        to %rbp                          %rbp = 0x7fffffffdfd0 + 0x18= 0x7fffffffdfe8 -> 49 ?
   0x0000000000400f3a <+62>:    jmp    0x400f17 <phase_2+27>        // 
   0x0000000000400f3c <+64>:    add    $0x28,%rsp                   // %rsp+28
   0x0000000000400f40 <+68>:    pop    %rbx
   0x0000000000400f41 <+69>:    pop    %rbp
   0x0000000000400f42 <+70>:    ret    
End of assembler dump.


(gdb) disas
Dump of assembler code for function read_six_numbers:
=> 0x000000000040145c <+0>:     sub    $0x18,%rsp          // move the stack pointer down 18 bytes
   0x0000000000401460 <+4>:     mov    %rsi,%rdx           // argu3 =  argu2                       140737488347088      
   0x0000000000401463 <+7>:     lea    0x4(%rsi),%rcx      // argu4 = &(argu2 +4)                  140737488347092
   0x0000000000401467 <+11>:    lea    0x14(%rsi),%rax     // return = &(argu2 + 20)               140737488347108
   0x000000000040146b <+15>:    mov    %rax,0x8(%rsp)      // move return to stack pointer +8      
   0x0000000000401470 <+20>:    lea    0x10(%rsi),%rax     // return = &(argu2 + 16)               140737488347104
   0x0000000000401474 <+24>:    mov    %rax,(%rsp)         // move return to stack pointer         
   0x0000000000401478 <+28>:    lea    0xc(%rsi),%r9       // argu6= argu3 + 12                    140737488347100                   
   0x000000000040147c <+32>:    lea    0x8(%rsi),%r8       // argu5 = argu3 + 8                    140737488347096
   0x0000000000401480 <+36>:    mov    $0x4025c3,%esi      // argu3 = 0x4025c3                     4203971              
   0x0000000000401485 <+41>:    mov    $0x0,%eax           // return =0                            
   0x000000000040148a <+46>:    call   0x400bf0 <__isoc99_sscanf@plt>
   0x000000000040148f <+51>:    cmp    $0x5,%eax                                                  if(return >5)
   0x0000000000401492 <+54>:    jg     0x401499 <read_six_numbers+61>                                { +61}
   0x0000000000401494 <+56>:    call   0x40143a <explode_bomb>                                    else{explode_bomb}
   0x0000000000401499 <+61>:    add    $0x18,%rsp                                                                                            
   0x000000000040149d <+65>:    ret    
End of assembler dump.

*/

input = ""
void phase_2(input)
{}


