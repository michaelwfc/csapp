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


/*
(gdb) disas
Dump of assembler code for function phase_3:
=> 0x0000000000400f43 <+0>:     sub    $0x18,%rsp                     // move stack pointer down 0x7fffffffe038 - 0x18= 0x7fffffffe020
   0x0000000000400f47 <+4>:     lea    0xc(%rsp),%rcx                 // 0x7fffffffe020 +0xc = 0x7fffffffe02c 
   0x0000000000400f4c <+9>:     lea    0x8(%rsp),%rdx                 // 0x7fffffffe020 +0x8 = 0x7fffffffe028 ->-84
   0x0000000000400f51 <+14>:    mov    $0x4025cf,%esi                 // argu2= 0x4025cf -> "%d %d" or 37
   0x0000000000400f56 <+19>:    mov    $0x0,%eax                      // return value=0
   0x0000000000400f5b <+24>:    call   0x400bf0 <__isoc99_sscanf@plt> // scanf result( entered nums of int/float/char/string)
   0x0000000000400f60 <+29>:    cmp    $0x1,%eax                      // if(entered nums=2 >1)
   0x0000000000400f63 <+32>:    jg     0x400f6a <phase_3+39>          //   { +39}
   0x0000000000400f65 <+34>:    call   0x40143a <explode_bomb>
   0x0000000000400f6a <+39>:    cmpl   $0x7,0x8(%rsp)                 // if(argu1  >7 )   
   0x0000000000400f6f <+44>:    ja     0x400fad <phase_3+106>         //     { + 106 explode_bomb}
   0x0000000000400f71 <+46>:    mov    0x8(%rsp),%eax                 // else{ return value = }
   0x0000000000400f75 <+50>:    jmp    *0x402470(,%rax,8)             // ? 
   0x0000000000400f7c <+57>:    mov    $0xcf,%eax
   0x0000000000400f81 <+62>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f83 <+64>:    mov    $0x2c3,%eax
   0x0000000000400f88 <+69>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f8a <+71>:    mov    $0x100,%eax
   0x0000000000400f8f <+76>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f91 <+78>:    mov    $0x185,%eax
   0x0000000000400f96 <+83>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f98 <+85>:    mov    $0xce,%eax
   0x0000000000400f9d <+90>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f9f <+92>:    mov    $0x2aa,%eax
   0x0000000000400fa4 <+97>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400fa6 <+99>:    mov    $0x147,%eax
   0x0000000000400fab <+104>:   jmp    0x400fbe <phase_3+123>
   0x0000000000400fad <+106>:   call   0x40143a <explode_bomb>
   0x0000000000400fb2 <+111>:   mov    $0x0,%eax
   0x0000000000400fb7 <+116>:   jmp    0x400fbe <phase_3+123>
   0x0000000000400fb9 <+118>:   mov    $0x137,%eax                // %rax=0x137=311 
   0x0000000000400fbe <+123>:   cmp    0xc(%rsp),%eax             // if(311 == argu2 )
   0x0000000000400fc2 <+127>:   je     0x400fc9 <phase_3+134>     //    { +134}
   0x0000000000400fc4 <+129>:   call   0x40143a <explode_bomb>
   0x0000000000400fc9 <+134>:   add    $0x18,%rsp
   0x0000000000400fcd <+138>:   ret    
End of assembler dump.

*/
void phase_3()
{

}

/*
(gdb) disas
Dump of assembler code for function phase_4:
=> 0x000000000040100c <+0>:     sub    $0x18,%rsp                       // 
   0x0000000000401010 <+4>:     lea    0xc(%rsp),%rcx                   // 
   0x0000000000401015 <+9>:     lea    0x8(%rsp),%rdx                    
   0x000000000040101a <+14>:    mov    $0x4025cf,%esi                   // %rsi = 0x4025cf = 4203983 ->37          
   0x000000000040101f <+19>:    mov    $0x0,%eax                        
   0x0000000000401024 <+24>:    call   0x400bf0 <__isoc99_sscanf@plt>   
   0x0000000000401029 <+29>:    cmp    $0x2,%eax                       // if(enter_nums !=2)
   0x000000000040102c <+32>:    jne    0x401035 <phase_4+41>                 { +41}
   0x000000000040102e <+34>:    cmpl   $0xe,0x8(%rsp)                  // elif(argu1 <=15)
   0x0000000000401033 <+39>:    jbe    0x40103a <phase_4+46>                 { +46}
   0x0000000000401035 <+41>:    call   0x40143a <explode_bomb>         
   0x000000000040103a <+46>:    mov    $0xe,%edx                       // %rdx = 14
   0x000000000040103f <+51>:    mov    $0x0,%esi                       
   0x0000000000401044 <+56>:    mov    0x8(%rsp),%edi                  // %rdi = argu1
   0x0000000000401048 <+60>:    call   0x400fce <func4>                
   0x000000000040104d <+65>:    test   %eax,%eax                       // if(%rax!=0)
   0x000000000040104f <+67>:    jne    0x401058 <phase_4+76>                 { +76 explode_bomb}
   0x0000000000401051 <+69>:    cmpl   $0x0,0xc(%rsp)                  // elif(argu2 ==0)
   0x0000000000401056 <+74>:    je     0x40105d <phase_4+81>                  { +81} 
   0x0000000000401058 <+76>:    call   0x40143a <explode_bomb>
   0x000000000040105d <+81>:    add    $0x18,%rsp
   0x0000000000401061 <+85>:    ret    
End of assembler dump.

*/

/*
 mov    %fs:0x28,%rax
The assembly instruction mov %fs:0x28, %rax involves moving the value stored at a specific memory location, which is calculated based on the value of the fs segment register, into the rax register. Here’s a detailed breakdown:

Understanding the Instruction
%fs:: This refers to the FS segment register. The fs register is one of the segment registers used in x86 architecture. Segment registers are used in conjunction with offsets to access different segments of memory. The fs register is often used to access thread-specific data, such as thread-local storage (TLS) in some operating systems.
0x28: This is the offset from the base address stored in the fs segment. The fs segment register contains a base address, and 0x28 is added to this base address to calculate the effective address from which the data will be retrieved.
%rax: This is a 64-bit general-purpose register in x86-64 architecture. The result of the memory fetch operation will be stored in this register.

(gdb) disas
Dump of assembler code for function phase_5:
=> 0x0000000000401062 <+0>:     push   %rbx                       // 
   0x0000000000401063 <+1>:     sub    $0x20,%rsp                 // 0x7fffffffe038 - 0x20=  0x7fffffffe018
   0x0000000000401067 <+5>:     mov    %rdi,%rbx                  // %rbx = &argu1
   0x000000000040106a <+8>:     mov    %fs:0x28,%rax              // rax   0xd8bdac3c8fd72200  -2828915614807547392
   0x0000000000401073 <+17>:    mov    %rax,0x18(%rsp)            // x/8x 0x7fffffffe028 > 0x00    0x22    0xd7    0x8f    0x3c    0xac    0xbd    0xd8         
   0x0000000000401078 <+22>:    xor    %eax,%eax                  // %rax ^ %rax
   0x000000000040107a <+24>:    call   0x40131b <string_length>   
   0x000000000040107f <+29>:    cmp    $0x6,%eax                  // if( string_length==6)
   0x0000000000401082 <+32>:    je     0x4010d2 <phase_5+112>     //    { +112}
   0x0000000000401084 <+34>:    call   0x40143a <explode_bomb>
   0x0000000000401089 <+39>:    jmp    0x4010d2 <phase_5+112>
   0x000000000040108b <+41>:    movzbl (%rbx,%rax,1),%ecx            // 
   0x000000000040108f <+45>:    mov    %cl,(%rsp)
   0x0000000000401092 <+48>:    mov    (%rsp),%rdx
   0x0000000000401096 <+52>:    and    $0xf,%edx
   0x0000000000401099 <+55>:    movzbl 0x4024b0(%rdx),%edx
   0x00000000004010a0 <+62>:    mov    %dl,0x10(%rsp,%rax,1)
   0x00000000004010a4 <+66>:    add    $0x1,%rax
   0x00000000004010a8 <+70>:    cmp    $0x6,%rax                      // if( %rax !=6)
   0x00000000004010ac <+74>:    jne    0x40108b <phase_5+41>          //   { +41}  
   0x00000000004010ae <+76>:    movb   $0x0,0x16(%rsp)
   0x00000000004010b3 <+81>:    mov    $0x40245e,%esi
   0x00000000004010b8 <+86>:    lea    0x10(%rsp),%rdi
   0x00000000004010bd <+91>:    call   0x401338 <strings_not_equal>
   0x00000000004010c2 <+96>:    test   %eax,%eax                      //if( %rax ==0)
   0x00000000004010c4 <+98>:    je     0x4010d9 <phase_5+119>           { +119 }
   0x00000000004010c6 <+100>:   call   0x40143a <explode_bomb>
   0x00000000004010cb <+105>:   nopl   0x0(%rax,%rax,1)
   0x00000000004010d0 <+110>:   jmp    0x4010d9 <phase_5+119>
   0x00000000004010d2 <+112>:   mov    $0x0,%eax                  // 
   0x00000000004010d7 <+117>:   jmp    0x40108b <phase_5+41>
   0x00000000004010d9 <+119>:   mov    0x18(%rsp),%rax
   0x00000000004010de <+124>:   xor    %fs:0x28,%rax
   0x00000000004010e7 <+133>:   je     0x4010ee <phase_5+140>
   0x00000000004010e9 <+135>:   call   0x400b30 <__stack_chk_fail@plt>
   0x00000000004010ee <+140>:   add    $0x20,%rsp
   0x00000000004010f2 <+144>:   pop    %rbx
   0x00000000004010f3 <+145>:   ret    
End of assembler dump.

*/