
sum.exe:     file format pei-x86-64


Disassembly of section .text:

0000000140001000 <__mingw_invalidParameterHandler>:
   140001000:	c3                   	ret
   140001001:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
   140001008:	00 00 00 00 
   14000100c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000140001010 <pre_c_init>:
   140001010:	48 83 ec 28          	sub    $0x28,%rsp
   140001014:	48 8b 05 f5 33 00 00 	mov    0x33f5(%rip),%rax        # 140004410 <.refptr.__mingw_initltsdrot_force>
   14000101b:	31 c9                	xor    %ecx,%ecx
   14000101d:	c7 00 01 00 00 00    	movl   $0x1,(%rax)
   140001023:	48 8b 05 f6 33 00 00 	mov    0x33f6(%rip),%rax        # 140004420 <.refptr.__mingw_initltsdyn_force>
   14000102a:	c7 00 01 00 00 00    	movl   $0x1,(%rax)
   140001030:	48 8b 05 f9 33 00 00 	mov    0x33f9(%rip),%rax        # 140004430 <.refptr.__mingw_initltssuo_force>
   140001037:	c7 00 01 00 00 00    	movl   $0x1,(%rax)
   14000103d:	48 8b 05 5c 33 00 00 	mov    0x335c(%rip),%rax        # 1400043a0 <.refptr.__ImageBase>
   140001044:	66 81 38 4d 5a       	cmpw   $0x5a4d,(%rax)
   140001049:	75 0f                	jne    14000105a <pre_c_init+0x4a>
   14000104b:	48 63 50 3c          	movslq 0x3c(%rax),%rdx
   14000104f:	48 01 d0             	add    %rdx,%rax
   140001052:	81 38 50 45 00 00    	cmpl   $0x4550,(%rax)
   140001058:	74 66                	je     1400010c0 <pre_c_init+0xb0>
   14000105a:	48 8b 05 9f 33 00 00 	mov    0x339f(%rip),%rax        # 140004400 <.refptr.__mingw_app_type>
   140001061:	89 0d a5 5f 00 00    	mov    %ecx,0x5fa5(%rip)        # 14000700c <managedapp>
   140001067:	8b 00                	mov    (%rax),%eax
   140001069:	85 c0                	test   %eax,%eax
   14000106b:	74 43                	je     1400010b0 <pre_c_init+0xa0>
   14000106d:	b9 02 00 00 00       	mov    $0x2,%ecx
   140001072:	e8 99 18 00 00       	call   140002910 <__set_app_type>
   140001077:	e8 14 18 00 00       	call   140002890 <__p__fmode>
   14000107c:	48 8b 15 5d 34 00 00 	mov    0x345d(%rip),%rdx        # 1400044e0 <.refptr._fmode>
   140001083:	8b 12                	mov    (%rdx),%edx
   140001085:	89 10                	mov    %edx,(%rax)
   140001087:	e8 fc 17 00 00       	call   140002888 <__p__commode>
   14000108c:	48 8b 15 2d 34 00 00 	mov    0x342d(%rip),%rdx        # 1400044c0 <.refptr._commode>
   140001093:	8b 12                	mov    (%rdx),%edx
   140001095:	89 10                	mov    %edx,(%rax)
   140001097:	e8 f4 04 00 00       	call   140001590 <_setargv>
   14000109c:	48 8b 05 dd 32 00 00 	mov    0x32dd(%rip),%rax        # 140004380 <.refptr._MINGW_INSTALL_DEBUG_MATHERR>
   1400010a3:	83 38 01             	cmpl   $0x1,(%rax)
   1400010a6:	74 50                	je     1400010f8 <pre_c_init+0xe8>
   1400010a8:	31 c0                	xor    %eax,%eax
   1400010aa:	48 83 c4 28          	add    $0x28,%rsp
   1400010ae:	c3                   	ret
   1400010af:	90                   	nop
   1400010b0:	b9 01 00 00 00       	mov    $0x1,%ecx
   1400010b5:	e8 56 18 00 00       	call   140002910 <__set_app_type>
   1400010ba:	eb bb                	jmp    140001077 <pre_c_init+0x67>
   1400010bc:	0f 1f 40 00          	nopl   0x0(%rax)
   1400010c0:	0f b7 50 18          	movzwl 0x18(%rax),%edx
   1400010c4:	66 81 fa 0b 01       	cmp    $0x10b,%dx
   1400010c9:	74 45                	je     140001110 <pre_c_init+0x100>
   1400010cb:	66 81 fa 0b 02       	cmp    $0x20b,%dx
   1400010d0:	75 88                	jne    14000105a <pre_c_init+0x4a>
   1400010d2:	83 b8 84 00 00 00 0e 	cmpl   $0xe,0x84(%rax)
   1400010d9:	0f 86 7b ff ff ff    	jbe    14000105a <pre_c_init+0x4a>
   1400010df:	8b 90 f8 00 00 00    	mov    0xf8(%rax),%edx
   1400010e5:	31 c9                	xor    %ecx,%ecx
   1400010e7:	85 d2                	test   %edx,%edx
   1400010e9:	0f 95 c1             	setne  %cl
   1400010ec:	e9 69 ff ff ff       	jmp    14000105a <pre_c_init+0x4a>
   1400010f1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   1400010f8:	48 8b 0d 01 34 00 00 	mov    0x3401(%rip),%rcx        # 140004500 <.refptr._matherr>
   1400010ff:	e8 fc 0b 00 00       	call   140001d00 <__mingw_setusermatherr>
   140001104:	31 c0                	xor    %eax,%eax
   140001106:	48 83 c4 28          	add    $0x28,%rsp
   14000110a:	c3                   	ret
   14000110b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   140001110:	83 78 74 0e          	cmpl   $0xe,0x74(%rax)
   140001114:	0f 86 40 ff ff ff    	jbe    14000105a <pre_c_init+0x4a>
   14000111a:	44 8b 80 e8 00 00 00 	mov    0xe8(%rax),%r8d
   140001121:	31 c9                	xor    %ecx,%ecx
   140001123:	45 85 c0             	test   %r8d,%r8d
   140001126:	0f 95 c1             	setne  %cl
   140001129:	e9 2c ff ff ff       	jmp    14000105a <pre_c_init+0x4a>
   14000112e:	66 90                	xchg   %ax,%ax

0000000140001130 <pre_cpp_init>:
   140001130:	48 83 ec 38          	sub    $0x38,%rsp
   140001134:	48 8b 05 d5 33 00 00 	mov    0x33d5(%rip),%rax        # 140004510 <.refptr._newmode>
   14000113b:	4c 8d 05 d6 5e 00 00 	lea    0x5ed6(%rip),%r8        # 140007018 <envp>
   140001142:	48 8d 15 d7 5e 00 00 	lea    0x5ed7(%rip),%rdx        # 140007020 <argv>
   140001149:	48 8d 0d d8 5e 00 00 	lea    0x5ed8(%rip),%rcx        # 140007028 <argc>
   140001150:	8b 00                	mov    (%rax),%eax
   140001152:	89 05 ac 5e 00 00    	mov    %eax,0x5eac(%rip)        # 140007004 <startinfo>
   140001158:	48 8b 05 71 33 00 00 	mov    0x3371(%rip),%rax        # 1400044d0 <.refptr._dowildcard>
   14000115f:	44 8b 08             	mov    (%rax),%r9d
   140001162:	48 8d 05 9b 5e 00 00 	lea    0x5e9b(%rip),%rax        # 140007004 <startinfo>
   140001169:	48 89 44 24 20       	mov    %rax,0x20(%rsp)
   14000116e:	e8 cd 14 00 00       	call   140002640 <__getmainargs>
   140001173:	90                   	nop
   140001174:	48 83 c4 38          	add    $0x38,%rsp
   140001178:	c3                   	ret
   140001179:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000140001180 <__tmainCRTStartup>:
   140001180:	41 54                	push   %r12
   140001182:	55                   	push   %rbp
   140001183:	57                   	push   %rdi
   140001184:	56                   	push   %rsi
   140001185:	53                   	push   %rbx
   140001186:	48 83 ec 20          	sub    $0x20,%rsp
   14000118a:	48 8b 1d cf 32 00 00 	mov    0x32cf(%rip),%rbx        # 140004460 <.refptr.__native_startup_lock>
   140001191:	48 8b 2d 50 71 00 00 	mov    0x7150(%rip),%rbp        # 1400082e8 <__imp_Sleep>
   140001198:	31 ff                	xor    %edi,%edi
   14000119a:	65 48 8b 04 25 30 00 	mov    %gs:0x30,%rax
   1400011a1:	00 00 
   1400011a3:	48 8b 70 08          	mov    0x8(%rax),%rsi
   1400011a7:	eb 17                	jmp    1400011c0 <__tmainCRTStartup+0x40>
   1400011a9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   1400011b0:	48 39 c6             	cmp    %rax,%rsi
   1400011b3:	0f 84 67 01 00 00    	je     140001320 <__tmainCRTStartup+0x1a0>
   1400011b9:	b9 e8 03 00 00       	mov    $0x3e8,%ecx
   1400011be:	ff d5                	call   *%rbp
   1400011c0:	48 89 f8             	mov    %rdi,%rax
   1400011c3:	f0 48 0f b1 33       	lock cmpxchg %rsi,(%rbx)
   1400011c8:	48 85 c0             	test   %rax,%rax
   1400011cb:	75 e3                	jne    1400011b0 <__tmainCRTStartup+0x30>
   1400011cd:	48 8b 35 9c 32 00 00 	mov    0x329c(%rip),%rsi        # 140004470 <.refptr.__native_startup_state>
   1400011d4:	31 ff                	xor    %edi,%edi
   1400011d6:	8b 06                	mov    (%rsi),%eax
   1400011d8:	83 f8 01             	cmp    $0x1,%eax
   1400011db:	0f 84 56 01 00 00    	je     140001337 <__tmainCRTStartup+0x1b7>
   1400011e1:	8b 06                	mov    (%rsi),%eax
   1400011e3:	85 c0                	test   %eax,%eax
   1400011e5:	0f 84 b5 01 00 00    	je     1400013a0 <__tmainCRTStartup+0x220>
   1400011eb:	c7 05 13 5e 00 00 01 	movl   $0x1,0x5e13(%rip)        # 140007008 <has_cctor>
   1400011f2:	00 00 00 
   1400011f5:	8b 06                	mov    (%rsi),%eax
   1400011f7:	83 f8 01             	cmp    $0x1,%eax
   1400011fa:	0f 84 4c 01 00 00    	je     14000134c <__tmainCRTStartup+0x1cc>
   140001200:	85 ff                	test   %edi,%edi
   140001202:	0f 84 65 01 00 00    	je     14000136d <__tmainCRTStartup+0x1ed>
   140001208:	48 8b 05 c1 31 00 00 	mov    0x31c1(%rip),%rax        # 1400043d0 <.refptr.__dyn_tls_init_callback>
   14000120f:	48 8b 00             	mov    (%rax),%rax
   140001212:	48 85 c0             	test   %rax,%rax
   140001215:	74 0c                	je     140001223 <__tmainCRTStartup+0xa3>
   140001217:	45 31 c0             	xor    %r8d,%r8d
   14000121a:	ba 02 00 00 00       	mov    $0x2,%edx
   14000121f:	31 c9                	xor    %ecx,%ecx
   140001221:	ff d0                	call   *%rax
   140001223:	e8 38 07 00 00       	call   140001960 <_pei386_runtime_relocator>
   140001228:	48 8b 0d c1 32 00 00 	mov    0x32c1(%rip),%rcx        # 1400044f0 <.refptr._gnu_exception_handler>
   14000122f:	ff 15 ab 70 00 00    	call   *0x70ab(%rip)        # 1400082e0 <__imp_SetUnhandledExceptionFilter>
   140001235:	48 8b 15 14 32 00 00 	mov    0x3214(%rip),%rdx        # 140004450 <.refptr.__mingw_oldexcpt_handler>
   14000123c:	48 8d 0d bd fd ff ff 	lea    -0x243(%rip),%rcx        # 140001000 <__mingw_invalidParameterHandler>
   140001243:	48 89 02             	mov    %rax,(%rdx)
   140001246:	e8 cd 16 00 00       	call   140002918 <_set_invalid_parameter_handler>
   14000124b:	e8 20 05 00 00       	call   140001770 <_fpreset>
   140001250:	8b 1d d2 5d 00 00    	mov    0x5dd2(%rip),%ebx        # 140007028 <argc>
   140001256:	8d 7b 01             	lea    0x1(%rbx),%edi
   140001259:	48 63 ff             	movslq %edi,%rdi
   14000125c:	48 c1 e7 03          	shl    $0x3,%rdi
   140001260:	48 89 f9             	mov    %rdi,%rcx
   140001263:	e8 10 17 00 00       	call   140002978 <malloc>
   140001268:	4c 8b 25 b1 5d 00 00 	mov    0x5db1(%rip),%r12        # 140007020 <argv>
   14000126f:	48 89 c5             	mov    %rax,%rbp
   140001272:	85 db                	test   %ebx,%ebx
   140001274:	0f 8e 46 01 00 00    	jle    1400013c0 <__tmainCRTStartup+0x240>
   14000127a:	48 83 ef 08          	sub    $0x8,%rdi
   14000127e:	31 db                	xor    %ebx,%ebx
   140001280:	49 8b 0c 1c          	mov    (%r12,%rbx,1),%rcx
   140001284:	e8 e7 15 00 00       	call   140002870 <strlen>
   140001289:	48 8d 70 01          	lea    0x1(%rax),%rsi
   14000128d:	48 89 f1             	mov    %rsi,%rcx
   140001290:	e8 e3 16 00 00       	call   140002978 <malloc>
   140001295:	49 89 f0             	mov    %rsi,%r8
   140001298:	48 89 44 1d 00       	mov    %rax,0x0(%rbp,%rbx,1)
   14000129d:	49 8b 14 1c          	mov    (%r12,%rbx,1),%rdx
   1400012a1:	48 89 c1             	mov    %rax,%rcx
   1400012a4:	48 83 c3 08          	add    $0x8,%rbx
   1400012a8:	e8 9b 16 00 00       	call   140002948 <memcpy>
   1400012ad:	48 39 df             	cmp    %rbx,%rdi
   1400012b0:	75 ce                	jne    140001280 <__tmainCRTStartup+0x100>
   1400012b2:	48 01 ef             	add    %rbp,%rdi
   1400012b5:	48 c7 07 00 00 00 00 	movq   $0x0,(%rdi)
   1400012bc:	48 89 2d 5d 5d 00 00 	mov    %rbp,0x5d5d(%rip)        # 140007020 <argv>
   1400012c3:	e8 a8 02 00 00       	call   140001570 <__main>
   1400012c8:	48 8b 05 11 31 00 00 	mov    0x3111(%rip),%rax        # 1400043e0 <.refptr.__imp___initenv>
   1400012cf:	4c 8b 05 42 5d 00 00 	mov    0x5d42(%rip),%r8        # 140007018 <envp>
   1400012d6:	8b 0d 4c 5d 00 00    	mov    0x5d4c(%rip),%ecx        # 140007028 <argc>
   1400012dc:	48 8b 00             	mov    (%rax),%rax
   1400012df:	4c 89 00             	mov    %r8,(%rax)
   1400012e2:	48 8b 15 37 5d 00 00 	mov    0x5d37(%rip),%rdx        # 140007020 <argv>
   1400012e9:	e8 7b 01 00 00       	call   140001469 <main>
   1400012ee:	8b 0d 18 5d 00 00    	mov    0x5d18(%rip),%ecx        # 14000700c <managedapp>
   1400012f4:	89 05 16 5d 00 00    	mov    %eax,0x5d16(%rip)        # 140007010 <mainret>
   1400012fa:	85 c9                	test   %ecx,%ecx
   1400012fc:	0f 84 c6 00 00 00    	je     1400013c8 <__tmainCRTStartup+0x248>
   140001302:	8b 15 00 5d 00 00    	mov    0x5d00(%rip),%edx        # 140007008 <has_cctor>
   140001308:	85 d2                	test   %edx,%edx
   14000130a:	74 74                	je     140001380 <__tmainCRTStartup+0x200>
   14000130c:	48 83 c4 20          	add    $0x20,%rsp
   140001310:	5b                   	pop    %rbx
   140001311:	5e                   	pop    %rsi
   140001312:	5f                   	pop    %rdi
   140001313:	5d                   	pop    %rbp
   140001314:	41 5c                	pop    %r12
   140001316:	c3                   	ret
   140001317:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   14000131e:	00 00 
   140001320:	48 8b 35 49 31 00 00 	mov    0x3149(%rip),%rsi        # 140004470 <.refptr.__native_startup_state>
   140001327:	bf 01 00 00 00       	mov    $0x1,%edi
   14000132c:	8b 06                	mov    (%rsi),%eax
   14000132e:	83 f8 01             	cmp    $0x1,%eax
   140001331:	0f 85 aa fe ff ff    	jne    1400011e1 <__tmainCRTStartup+0x61>
   140001337:	b9 1f 00 00 00       	mov    $0x1f,%ecx
   14000133c:	e8 1f 14 00 00       	call   140002760 <_amsg_exit>
   140001341:	8b 06                	mov    (%rsi),%eax
   140001343:	83 f8 01             	cmp    $0x1,%eax
   140001346:	0f 85 b4 fe ff ff    	jne    140001200 <__tmainCRTStartup+0x80>
   14000134c:	48 8b 15 3d 31 00 00 	mov    0x313d(%rip),%rdx        # 140004490 <.refptr.__xc_z>
   140001353:	48 8b 0d 26 31 00 00 	mov    0x3126(%rip),%rcx        # 140004480 <.refptr.__xc_a>
   14000135a:	e8 a9 15 00 00       	call   140002908 <_initterm>
   14000135f:	c7 06 02 00 00 00    	movl   $0x2,(%rsi)
   140001365:	85 ff                	test   %edi,%edi
   140001367:	0f 85 9b fe ff ff    	jne    140001208 <__tmainCRTStartup+0x88>
   14000136d:	31 c0                	xor    %eax,%eax
   14000136f:	48 87 03             	xchg   %rax,(%rbx)
   140001372:	e9 91 fe ff ff       	jmp    140001208 <__tmainCRTStartup+0x88>
   140001377:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   14000137e:	00 00 
   140001380:	e8 43 15 00 00       	call   1400028c8 <_cexit>
   140001385:	8b 05 85 5c 00 00    	mov    0x5c85(%rip),%eax        # 140007010 <mainret>
   14000138b:	48 83 c4 20          	add    $0x20,%rsp
   14000138f:	5b                   	pop    %rbx
   140001390:	5e                   	pop    %rsi
   140001391:	5f                   	pop    %rdi
   140001392:	5d                   	pop    %rbp
   140001393:	41 5c                	pop    %r12
   140001395:	c3                   	ret
   140001396:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   14000139d:	00 00 00 
   1400013a0:	48 8b 15 09 31 00 00 	mov    0x3109(%rip),%rdx        # 1400044b0 <.refptr.__xi_z>
   1400013a7:	48 8b 0d f2 30 00 00 	mov    0x30f2(%rip),%rcx        # 1400044a0 <.refptr.__xi_a>
   1400013ae:	c7 06 01 00 00 00    	movl   $0x1,(%rsi)
   1400013b4:	e8 4f 15 00 00       	call   140002908 <_initterm>
   1400013b9:	e9 37 fe ff ff       	jmp    1400011f5 <__tmainCRTStartup+0x75>
   1400013be:	66 90                	xchg   %ax,%ax
   1400013c0:	48 89 c7             	mov    %rax,%rdi
   1400013c3:	e9 ed fe ff ff       	jmp    1400012b5 <__tmainCRTStartup+0x135>
   1400013c8:	89 c1                	mov    %eax,%ecx
   1400013ca:	e8 59 15 00 00       	call   140002928 <exit>
   1400013cf:	90                   	nop

00000001400013d0 <WinMainCRTStartup>:
   1400013d0:	48 83 ec 28          	sub    $0x28,%rsp

00000001400013d4 <.l_startw>:
   1400013d4:	48 8b 05 25 30 00 00 	mov    0x3025(%rip),%rax        # 140004400 <.refptr.__mingw_app_type>
   1400013db:	c7 00 01 00 00 00    	movl   $0x1,(%rax)
   1400013e1:	e8 9a fd ff ff       	call   140001180 <__tmainCRTStartup>
   1400013e6:	90                   	nop

00000001400013e7 <.l_endw>:
   1400013e7:	90                   	nop
   1400013e8:	48 83 c4 28          	add    $0x28,%rsp
   1400013ec:	c3                   	ret
   1400013ed:	0f 1f 00             	nopl   (%rax)

00000001400013f0 <mainCRTStartup>:
   1400013f0:	48 83 ec 28          	sub    $0x28,%rsp

00000001400013f4 <.l_start>:
   1400013f4:	48 8b 05 05 30 00 00 	mov    0x3005(%rip),%rax        # 140004400 <.refptr.__mingw_app_type>
   1400013fb:	c7 00 00 00 00 00    	movl   $0x0,(%rax)
   140001401:	e8 7a fd ff ff       	call   140001180 <__tmainCRTStartup>
   140001406:	90                   	nop

0000000140001407 <.l_end>:
   140001407:	90                   	nop
   140001408:	48 83 c4 28          	add    $0x28,%rsp
   14000140c:	c3                   	ret
   14000140d:	0f 1f 00             	nopl   (%rax)

0000000140001410 <atexit>:
   140001410:	48 83 ec 28          	sub    $0x28,%rsp
   140001414:	e8 07 13 00 00       	call   140002720 <_onexit>
   140001419:	48 83 f8 01          	cmp    $0x1,%rax
   14000141d:	19 c0                	sbb    %eax,%eax
   14000141f:	48 83 c4 28          	add    $0x28,%rsp
   140001423:	c3                   	ret
   140001424:	90                   	nop
   140001425:	90                   	nop
   140001426:	90                   	nop
   140001427:	90                   	nop
   140001428:	90                   	nop
   140001429:	90                   	nop
   14000142a:	90                   	nop
   14000142b:	90                   	nop
   14000142c:	90                   	nop
   14000142d:	90                   	nop
   14000142e:	90                   	nop
   14000142f:	90                   	nop

0000000140001430 <__gcc_register_frame>:
   140001430:	48 8d 0d 09 00 00 00 	lea    0x9(%rip),%rcx        # 140001440 <__gcc_deregister_frame>
   140001437:	e9 d4 ff ff ff       	jmp    140001410 <atexit>
   14000143c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000140001440 <__gcc_deregister_frame>:
   140001440:	c3                   	ret
   140001441:	90                   	nop
   140001442:	90                   	nop
   140001443:	90                   	nop
   140001444:	90                   	nop
   140001445:	90                   	nop
   140001446:	90                   	nop
   140001447:	90                   	nop
   140001448:	90                   	nop
   140001449:	90                   	nop
   14000144a:	90                   	nop
   14000144b:	90                   	nop
   14000144c:	90                   	nop
   14000144d:	90                   	nop
   14000144e:	90                   	nop
   14000144f:	90                   	nop

0000000140001450 <plus>:
   140001450:	8d 04 11             	lea    (%rcx,%rdx,1),%eax
   140001453:	c3                   	ret

0000000140001454 <sumstore>:
   140001454:	53                   	push   %rbx
   140001455:	48 83 ec 20          	sub    $0x20,%rsp
   140001459:	4c 89 c3             	mov    %r8,%rbx
   14000145c:	e8 ef ff ff ff       	call   140001450 <plus>
   140001461:	89 03                	mov    %eax,(%rbx)
   140001463:	48 83 c4 20          	add    $0x20,%rsp
   140001467:	5b                   	pop    %rbx
   140001468:	c3                   	ret

0000000140001469 <main>:
   140001469:	56                   	push   %rsi
   14000146a:	53                   	push   %rbx
   14000146b:	48 83 ec 38          	sub    $0x38,%rsp
   14000146f:	48 89 d6             	mov    %rdx,%rsi
   140001472:	e8 f9 00 00 00       	call   140001570 <__main>
   140001477:	48 8b 4e 08          	mov    0x8(%rsi),%rcx
   14000147b:	e8 10 15 00 00       	call   140002990 <atoi>
   140001480:	89 c3                	mov    %eax,%ebx
   140001482:	48 8b 4e 10          	mov    0x10(%rsi),%rcx
   140001486:	e8 05 15 00 00       	call   140002990 <atoi>
   14000148b:	89 c6                	mov    %eax,%esi
   14000148d:	4c 8d 44 24 2c       	lea    0x2c(%rsp),%r8
   140001492:	89 c2                	mov    %eax,%edx
   140001494:	89 d9                	mov    %ebx,%ecx
   140001496:	e8 b9 ff ff ff       	call   140001454 <sumstore>
   14000149b:	44 8b 4c 24 2c       	mov    0x2c(%rsp),%r9d
   1400014a0:	41 89 f0             	mov    %esi,%r8d
   1400014a3:	89 da                	mov    %ebx,%edx
   1400014a5:	48 8d 0d 54 2b 00 00 	lea    0x2b54(%rip),%rcx        # 140004000 <.rdata>
   1400014ac:	e8 ef 10 00 00       	call   1400025a0 <printf>
   1400014b1:	b8 00 00 00 00       	mov    $0x0,%eax
   1400014b6:	48 83 c4 38          	add    $0x38,%rsp
   1400014ba:	5b                   	pop    %rbx
   1400014bb:	5e                   	pop    %rsi
   1400014bc:	c3                   	ret
   1400014bd:	90                   	nop
   1400014be:	90                   	nop
   1400014bf:	90                   	nop

00000001400014c0 <__do_global_dtors>:
   1400014c0:	48 83 ec 28          	sub    $0x28,%rsp
   1400014c4:	48 8b 05 35 1b 00 00 	mov    0x1b35(%rip),%rax        # 140003000 <__data_start__>
   1400014cb:	48 8b 00             	mov    (%rax),%rax
   1400014ce:	48 85 c0             	test   %rax,%rax
   1400014d1:	74 22                	je     1400014f5 <__do_global_dtors+0x35>
   1400014d3:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   1400014d8:	ff d0                	call   *%rax
   1400014da:	48 8b 05 1f 1b 00 00 	mov    0x1b1f(%rip),%rax        # 140003000 <__data_start__>
   1400014e1:	48 8d 50 08          	lea    0x8(%rax),%rdx
   1400014e5:	48 8b 40 08          	mov    0x8(%rax),%rax
   1400014e9:	48 89 15 10 1b 00 00 	mov    %rdx,0x1b10(%rip)        # 140003000 <__data_start__>
   1400014f0:	48 85 c0             	test   %rax,%rax
   1400014f3:	75 e3                	jne    1400014d8 <__do_global_dtors+0x18>
   1400014f5:	48 83 c4 28          	add    $0x28,%rsp
   1400014f9:	c3                   	ret
   1400014fa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000140001500 <__do_global_ctors>:
   140001500:	56                   	push   %rsi
   140001501:	53                   	push   %rbx
   140001502:	48 83 ec 28          	sub    $0x28,%rsp
   140001506:	48 8b 15 83 2e 00 00 	mov    0x2e83(%rip),%rdx        # 140004390 <.refptr.__CTOR_LIST__>
   14000150d:	48 8b 02             	mov    (%rdx),%rax
   140001510:	89 c1                	mov    %eax,%ecx
   140001512:	83 f8 ff             	cmp    $0xffffffff,%eax
   140001515:	74 39                	je     140001550 <__do_global_ctors+0x50>
   140001517:	85 c9                	test   %ecx,%ecx
   140001519:	74 20                	je     14000153b <__do_global_ctors+0x3b>
   14000151b:	89 c8                	mov    %ecx,%eax
   14000151d:	83 e9 01             	sub    $0x1,%ecx
   140001520:	48 8d 1c c2          	lea    (%rdx,%rax,8),%rbx
   140001524:	48 29 c8             	sub    %rcx,%rax
   140001527:	48 8d 74 c2 f8       	lea    -0x8(%rdx,%rax,8),%rsi
   14000152c:	0f 1f 40 00          	nopl   0x0(%rax)
   140001530:	ff 13                	call   *(%rbx)
   140001532:	48 83 eb 08          	sub    $0x8,%rbx
   140001536:	48 39 f3             	cmp    %rsi,%rbx
   140001539:	75 f5                	jne    140001530 <__do_global_ctors+0x30>
   14000153b:	48 8d 0d 7e ff ff ff 	lea    -0x82(%rip),%rcx        # 1400014c0 <__do_global_dtors>
   140001542:	48 83 c4 28          	add    $0x28,%rsp
   140001546:	5b                   	pop    %rbx
   140001547:	5e                   	pop    %rsi
   140001548:	e9 c3 fe ff ff       	jmp    140001410 <atexit>
   14000154d:	0f 1f 00             	nopl   (%rax)
   140001550:	31 c0                	xor    %eax,%eax
   140001552:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
   140001558:	44 8d 40 01          	lea    0x1(%rax),%r8d
   14000155c:	89 c1                	mov    %eax,%ecx
   14000155e:	4a 83 3c c2 00       	cmpq   $0x0,(%rdx,%r8,8)
   140001563:	4c 89 c0             	mov    %r8,%rax
   140001566:	75 f0                	jne    140001558 <__do_global_ctors+0x58>
   140001568:	eb ad                	jmp    140001517 <__do_global_ctors+0x17>
   14000156a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000140001570 <__main>:
   140001570:	8b 05 ba 5a 00 00    	mov    0x5aba(%rip),%eax        # 140007030 <initialized>
   140001576:	85 c0                	test   %eax,%eax
   140001578:	74 06                	je     140001580 <__main+0x10>
   14000157a:	c3                   	ret
   14000157b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   140001580:	c7 05 a6 5a 00 00 01 	movl   $0x1,0x5aa6(%rip)        # 140007030 <initialized>
   140001587:	00 00 00 
   14000158a:	e9 71 ff ff ff       	jmp    140001500 <__do_global_ctors>
   14000158f:	90                   	nop

0000000140001590 <_setargv>:
   140001590:	31 c0                	xor    %eax,%eax
   140001592:	c3                   	ret
   140001593:	90                   	nop
   140001594:	90                   	nop
   140001595:	90                   	nop
   140001596:	90                   	nop
   140001597:	90                   	nop
   140001598:	90                   	nop
   140001599:	90                   	nop
   14000159a:	90                   	nop
   14000159b:	90                   	nop
   14000159c:	90                   	nop
   14000159d:	90                   	nop
   14000159e:	90                   	nop
   14000159f:	90                   	nop

00000001400015a0 <__dyn_tls_dtor>:
   1400015a0:	48 83 ec 28          	sub    $0x28,%rsp
   1400015a4:	83 fa 03             	cmp    $0x3,%edx
   1400015a7:	74 17                	je     1400015c0 <__dyn_tls_dtor+0x20>
   1400015a9:	85 d2                	test   %edx,%edx
   1400015ab:	74 13                	je     1400015c0 <__dyn_tls_dtor+0x20>
   1400015ad:	b8 01 00 00 00       	mov    $0x1,%eax
   1400015b2:	48 83 c4 28          	add    $0x28,%rsp
   1400015b6:	c3                   	ret
   1400015b7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   1400015be:	00 00 
   1400015c0:	e8 7b 0a 00 00       	call   140002040 <__mingw_TLScallback>
   1400015c5:	b8 01 00 00 00       	mov    $0x1,%eax
   1400015ca:	48 83 c4 28          	add    $0x28,%rsp
   1400015ce:	c3                   	ret
   1400015cf:	90                   	nop

00000001400015d0 <__dyn_tls_init>:
   1400015d0:	56                   	push   %rsi
   1400015d1:	53                   	push   %rbx
   1400015d2:	48 83 ec 28          	sub    $0x28,%rsp
   1400015d6:	48 8b 05 93 2d 00 00 	mov    0x2d93(%rip),%rax        # 140004370 <.refptr._CRT_MT>
   1400015dd:	83 38 02             	cmpl   $0x2,(%rax)
   1400015e0:	74 06                	je     1400015e8 <__dyn_tls_init+0x18>
   1400015e2:	c7 00 02 00 00 00    	movl   $0x2,(%rax)
   1400015e8:	83 fa 02             	cmp    $0x2,%edx
   1400015eb:	74 13                	je     140001600 <__dyn_tls_init+0x30>
   1400015ed:	83 fa 01             	cmp    $0x1,%edx
   1400015f0:	74 4e                	je     140001640 <__dyn_tls_init+0x70>
   1400015f2:	b8 01 00 00 00       	mov    $0x1,%eax
   1400015f7:	48 83 c4 28          	add    $0x28,%rsp
   1400015fb:	5b                   	pop    %rbx
   1400015fc:	5e                   	pop    %rsi
   1400015fd:	c3                   	ret
   1400015fe:	66 90                	xchg   %ax,%ax
   140001600:	48 8d 1d 51 7a 00 00 	lea    0x7a51(%rip),%rbx        # 140009058 <__xd_z>
   140001607:	48 8d 35 4a 7a 00 00 	lea    0x7a4a(%rip),%rsi        # 140009058 <__xd_z>
   14000160e:	48 39 f3             	cmp    %rsi,%rbx
   140001611:	74 df                	je     1400015f2 <__dyn_tls_init+0x22>
   140001613:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   140001618:	48 8b 03             	mov    (%rbx),%rax
   14000161b:	48 85 c0             	test   %rax,%rax
   14000161e:	74 02                	je     140001622 <__dyn_tls_init+0x52>
   140001620:	ff d0                	call   *%rax
   140001622:	48 83 c3 08          	add    $0x8,%rbx
   140001626:	48 39 f3             	cmp    %rsi,%rbx
   140001629:	75 ed                	jne    140001618 <__dyn_tls_init+0x48>
   14000162b:	b8 01 00 00 00       	mov    $0x1,%eax
   140001630:	48 83 c4 28          	add    $0x28,%rsp
   140001634:	5b                   	pop    %rbx
   140001635:	5e                   	pop    %rsi
   140001636:	c3                   	ret
   140001637:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   14000163e:	00 00 
   140001640:	e8 fb 09 00 00       	call   140002040 <__mingw_TLScallback>
   140001645:	b8 01 00 00 00       	mov    $0x1,%eax
   14000164a:	48 83 c4 28          	add    $0x28,%rsp
   14000164e:	5b                   	pop    %rbx
   14000164f:	5e                   	pop    %rsi
   140001650:	c3                   	ret
   140001651:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
   140001658:	00 00 00 00 
   14000165c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000140001660 <__tlregdtor>:
   140001660:	31 c0                	xor    %eax,%eax
   140001662:	c3                   	ret
   140001663:	90                   	nop
   140001664:	90                   	nop
   140001665:	90                   	nop
   140001666:	90                   	nop
   140001667:	90                   	nop
   140001668:	90                   	nop
   140001669:	90                   	nop
   14000166a:	90                   	nop
   14000166b:	90                   	nop
   14000166c:	90                   	nop
   14000166d:	90                   	nop
   14000166e:	90                   	nop
   14000166f:	90                   	nop

0000000140001670 <_matherr>:
   140001670:	56                   	push   %rsi
   140001671:	53                   	push   %rbx
   140001672:	48 83 ec 78          	sub    $0x78,%rsp
   140001676:	0f 11 74 24 40       	movups %xmm6,0x40(%rsp)
   14000167b:	0f 11 7c 24 50       	movups %xmm7,0x50(%rsp)
   140001680:	44 0f 11 44 24 60    	movups %xmm8,0x60(%rsp)
   140001686:	83 39 06             	cmpl   $0x6,(%rcx)
   140001689:	0f 87 cd 00 00 00    	ja     14000175c <_matherr+0xec>
   14000168f:	8b 01                	mov    (%rcx),%eax
   140001691:	48 8d 15 0c 2b 00 00 	lea    0x2b0c(%rip),%rdx        # 1400041a4 <.rdata+0x124>
   140001698:	48 63 04 82          	movslq (%rdx,%rax,4),%rax
   14000169c:	48 01 d0             	add    %rdx,%rax
   14000169f:	ff e0                	jmp    *%rax
   1400016a1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   1400016a8:	48 8d 1d f0 29 00 00 	lea    0x29f0(%rip),%rbx        # 14000409f <.rdata+0x1f>
   1400016af:	f2 44 0f 10 41 20    	movsd  0x20(%rcx),%xmm8
   1400016b5:	f2 0f 10 79 18       	movsd  0x18(%rcx),%xmm7
   1400016ba:	f2 0f 10 71 10       	movsd  0x10(%rcx),%xmm6
   1400016bf:	48 8b 71 08          	mov    0x8(%rcx),%rsi
   1400016c3:	b9 02 00 00 00       	mov    $0x2,%ecx
   1400016c8:	e8 b3 11 00 00       	call   140002880 <__acrt_iob_func>
   1400016cd:	f2 44 0f 11 44 24 30 	movsd  %xmm8,0x30(%rsp)
   1400016d4:	49 89 d8             	mov    %rbx,%r8
   1400016d7:	48 8d 15 9a 2a 00 00 	lea    0x2a9a(%rip),%rdx        # 140004178 <.rdata+0xf8>
   1400016de:	f2 0f 11 7c 24 28    	movsd  %xmm7,0x28(%rsp)
   1400016e4:	48 89 c1             	mov    %rax,%rcx
   1400016e7:	49 89 f1             	mov    %rsi,%r9
   1400016ea:	f2 0f 11 74 24 20    	movsd  %xmm6,0x20(%rsp)
   1400016f0:	e8 fb 0e 00 00       	call   1400025f0 <fprintf>
   1400016f5:	90                   	nop
   1400016f6:	0f 10 74 24 40       	movups 0x40(%rsp),%xmm6
   1400016fb:	0f 10 7c 24 50       	movups 0x50(%rsp),%xmm7
   140001700:	31 c0                	xor    %eax,%eax
   140001702:	44 0f 10 44 24 60    	movups 0x60(%rsp),%xmm8
   140001708:	48 83 c4 78          	add    $0x78,%rsp
   14000170c:	5b                   	pop    %rbx
   14000170d:	5e                   	pop    %rsi
   14000170e:	c3                   	ret
   14000170f:	90                   	nop
   140001710:	48 8d 1d 69 29 00 00 	lea    0x2969(%rip),%rbx        # 140004080 <.rdata>
   140001717:	eb 96                	jmp    1400016af <_matherr+0x3f>
   140001719:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140001720:	48 8d 1d b9 29 00 00 	lea    0x29b9(%rip),%rbx        # 1400040e0 <.rdata+0x60>
   140001727:	eb 86                	jmp    1400016af <_matherr+0x3f>
   140001729:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140001730:	48 8d 1d 89 29 00 00 	lea    0x2989(%rip),%rbx        # 1400040c0 <.rdata+0x40>
   140001737:	e9 73 ff ff ff       	jmp    1400016af <_matherr+0x3f>
   14000173c:	0f 1f 40 00          	nopl   0x0(%rax)
   140001740:	48 8d 1d e9 29 00 00 	lea    0x29e9(%rip),%rbx        # 140004130 <.rdata+0xb0>
   140001747:	e9 63 ff ff ff       	jmp    1400016af <_matherr+0x3f>
   14000174c:	0f 1f 40 00          	nopl   0x0(%rax)
   140001750:	48 8d 1d b1 29 00 00 	lea    0x29b1(%rip),%rbx        # 140004108 <.rdata+0x88>
   140001757:	e9 53 ff ff ff       	jmp    1400016af <_matherr+0x3f>
   14000175c:	48 8d 1d 03 2a 00 00 	lea    0x2a03(%rip),%rbx        # 140004166 <.rdata+0xe6>
   140001763:	e9 47 ff ff ff       	jmp    1400016af <_matherr+0x3f>
   140001768:	90                   	nop
   140001769:	90                   	nop
   14000176a:	90                   	nop
   14000176b:	90                   	nop
   14000176c:	90                   	nop
   14000176d:	90                   	nop
   14000176e:	90                   	nop
   14000176f:	90                   	nop

0000000140001770 <_fpreset>:
   140001770:	db e3                	fninit
   140001772:	c3                   	ret
   140001773:	90                   	nop
   140001774:	90                   	nop
   140001775:	90                   	nop
   140001776:	90                   	nop
   140001777:	90                   	nop
   140001778:	90                   	nop
   140001779:	90                   	nop
   14000177a:	90                   	nop
   14000177b:	90                   	nop
   14000177c:	90                   	nop
   14000177d:	90                   	nop
   14000177e:	90                   	nop
   14000177f:	90                   	nop

0000000140001780 <__report_error>:
   140001780:	56                   	push   %rsi
   140001781:	53                   	push   %rbx
   140001782:	48 83 ec 38          	sub    $0x38,%rsp
   140001786:	48 89 cb             	mov    %rcx,%rbx
   140001789:	48 8d 44 24 58       	lea    0x58(%rsp),%rax
   14000178e:	b9 02 00 00 00       	mov    $0x2,%ecx
   140001793:	48 89 54 24 58       	mov    %rdx,0x58(%rsp)
   140001798:	4c 89 44 24 60       	mov    %r8,0x60(%rsp)
   14000179d:	4c 89 4c 24 68       	mov    %r9,0x68(%rsp)
   1400017a2:	48 89 44 24 28       	mov    %rax,0x28(%rsp)
   1400017a7:	e8 d4 10 00 00       	call   140002880 <__acrt_iob_func>
   1400017ac:	41 b8 1b 00 00 00    	mov    $0x1b,%r8d
   1400017b2:	ba 01 00 00 00       	mov    $0x1,%edx
   1400017b7:	48 8d 0d 02 2a 00 00 	lea    0x2a02(%rip),%rcx        # 1400041c0 <.rdata>
   1400017be:	49 89 c1             	mov    %rax,%r9
   1400017c1:	e8 e2 10 00 00       	call   1400028a8 <fwrite>
   1400017c6:	48 8b 74 24 28       	mov    0x28(%rsp),%rsi
   1400017cb:	b9 02 00 00 00       	mov    $0x2,%ecx
   1400017d0:	e8 ab 10 00 00       	call   140002880 <__acrt_iob_func>
   1400017d5:	48 89 da             	mov    %rbx,%rdx
   1400017d8:	48 89 c1             	mov    %rax,%rcx
   1400017db:	49 89 f0             	mov    %rsi,%r8
   1400017de:	e8 9d 0d 00 00       	call   140002580 <vfprintf>
   1400017e3:	e8 38 11 00 00       	call   140002920 <abort>
   1400017e8:	90                   	nop
   1400017e9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000001400017f0 <mark_section_writable>:
   1400017f0:	57                   	push   %rdi
   1400017f1:	56                   	push   %rsi
   1400017f2:	53                   	push   %rbx
   1400017f3:	48 83 ec 50          	sub    $0x50,%rsp
   1400017f7:	48 63 35 96 58 00 00 	movslq 0x5896(%rip),%rsi        # 140007094 <maxSections>
   1400017fe:	48 89 cb             	mov    %rcx,%rbx
   140001801:	85 f6                	test   %esi,%esi
   140001803:	0f 8e 17 01 00 00    	jle    140001920 <mark_section_writable+0x130>
   140001809:	48 8b 05 88 58 00 00 	mov    0x5888(%rip),%rax        # 140007098 <the_secs>
   140001810:	45 31 c9             	xor    %r9d,%r9d
   140001813:	48 83 c0 18          	add    $0x18,%rax
   140001817:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   14000181e:	00 00 
   140001820:	4c 8b 00             	mov    (%rax),%r8
   140001823:	4c 39 c3             	cmp    %r8,%rbx
   140001826:	72 13                	jb     14000183b <mark_section_writable+0x4b>
   140001828:	48 8b 50 08          	mov    0x8(%rax),%rdx
   14000182c:	8b 52 08             	mov    0x8(%rdx),%edx
   14000182f:	49 01 d0             	add    %rdx,%r8
   140001832:	4c 39 c3             	cmp    %r8,%rbx
   140001835:	0f 82 8a 00 00 00    	jb     1400018c5 <mark_section_writable+0xd5>
   14000183b:	41 83 c1 01          	add    $0x1,%r9d
   14000183f:	48 83 c0 28          	add    $0x28,%rax
   140001843:	41 39 f1             	cmp    %esi,%r9d
   140001846:	75 d8                	jne    140001820 <mark_section_writable+0x30>
   140001848:	48 89 d9             	mov    %rbx,%rcx
   14000184b:	e8 10 0a 00 00       	call   140002260 <__mingw_GetSectionForAddress>
   140001850:	48 89 c7             	mov    %rax,%rdi
   140001853:	48 85 c0             	test   %rax,%rax
   140001856:	0f 84 e6 00 00 00    	je     140001942 <mark_section_writable+0x152>
   14000185c:	48 8b 05 35 58 00 00 	mov    0x5835(%rip),%rax        # 140007098 <the_secs>
   140001863:	48 8d 1c b6          	lea    (%rsi,%rsi,4),%rbx
   140001867:	48 c1 e3 03          	shl    $0x3,%rbx
   14000186b:	48 01 d8             	add    %rbx,%rax
   14000186e:	48 89 78 20          	mov    %rdi,0x20(%rax)
   140001872:	c7 00 00 00 00 00    	movl   $0x0,(%rax)
   140001878:	e8 23 0b 00 00       	call   1400023a0 <_GetPEImageBase>
   14000187d:	8b 57 0c             	mov    0xc(%rdi),%edx
   140001880:	41 b8 30 00 00 00    	mov    $0x30,%r8d
   140001886:	48 8d 0c 10          	lea    (%rax,%rdx,1),%rcx
   14000188a:	48 8b 05 07 58 00 00 	mov    0x5807(%rip),%rax        # 140007098 <the_secs>
   140001891:	48 8d 54 24 20       	lea    0x20(%rsp),%rdx
   140001896:	48 89 4c 18 18       	mov    %rcx,0x18(%rax,%rbx,1)
   14000189b:	ff 15 5f 6a 00 00    	call   *0x6a5f(%rip)        # 140008300 <__imp_VirtualQuery>
   1400018a1:	48 85 c0             	test   %rax,%rax
   1400018a4:	0f 84 7d 00 00 00    	je     140001927 <mark_section_writable+0x137>
   1400018aa:	8b 44 24 44          	mov    0x44(%rsp),%eax
   1400018ae:	8d 50 fc             	lea    -0x4(%rax),%edx
   1400018b1:	83 e2 fb             	and    $0xfffffffb,%edx
   1400018b4:	74 08                	je     1400018be <mark_section_writable+0xce>
   1400018b6:	8d 50 c0             	lea    -0x40(%rax),%edx
   1400018b9:	83 e2 bf             	and    $0xffffffbf,%edx
   1400018bc:	75 12                	jne    1400018d0 <mark_section_writable+0xe0>
   1400018be:	83 05 cf 57 00 00 01 	addl   $0x1,0x57cf(%rip)        # 140007094 <maxSections>
   1400018c5:	48 83 c4 50          	add    $0x50,%rsp
   1400018c9:	5b                   	pop    %rbx
   1400018ca:	5e                   	pop    %rsi
   1400018cb:	5f                   	pop    %rdi
   1400018cc:	c3                   	ret
   1400018cd:	0f 1f 00             	nopl   (%rax)
   1400018d0:	83 f8 02             	cmp    $0x2,%eax
   1400018d3:	48 8b 4c 24 20       	mov    0x20(%rsp),%rcx
   1400018d8:	48 8b 54 24 38       	mov    0x38(%rsp),%rdx
   1400018dd:	41 b8 40 00 00 00    	mov    $0x40,%r8d
   1400018e3:	b8 04 00 00 00       	mov    $0x4,%eax
   1400018e8:	44 0f 44 c0          	cmove  %eax,%r8d
   1400018ec:	48 03 1d a5 57 00 00 	add    0x57a5(%rip),%rbx        # 140007098 <the_secs>
   1400018f3:	48 89 4b 08          	mov    %rcx,0x8(%rbx)
   1400018f7:	49 89 d9             	mov    %rbx,%r9
   1400018fa:	48 89 53 10          	mov    %rdx,0x10(%rbx)
   1400018fe:	ff 15 f4 69 00 00    	call   *0x69f4(%rip)        # 1400082f8 <__imp_VirtualProtect>
   140001904:	85 c0                	test   %eax,%eax
   140001906:	75 b6                	jne    1400018be <mark_section_writable+0xce>
   140001908:	ff 15 ba 69 00 00    	call   *0x69ba(%rip)        # 1400082c8 <__imp_GetLastError>
   14000190e:	48 8d 0d 23 29 00 00 	lea    0x2923(%rip),%rcx        # 140004238 <.rdata+0x78>
   140001915:	89 c2                	mov    %eax,%edx
   140001917:	e8 64 fe ff ff       	call   140001780 <__report_error>
   14000191c:	0f 1f 40 00          	nopl   0x0(%rax)
   140001920:	31 f6                	xor    %esi,%esi
   140001922:	e9 21 ff ff ff       	jmp    140001848 <mark_section_writable+0x58>
   140001927:	48 8b 05 6a 57 00 00 	mov    0x576a(%rip),%rax        # 140007098 <the_secs>
   14000192e:	8b 57 08             	mov    0x8(%rdi),%edx
   140001931:	48 8d 0d c8 28 00 00 	lea    0x28c8(%rip),%rcx        # 140004200 <.rdata+0x40>
   140001938:	4c 8b 44 18 18       	mov    0x18(%rax,%rbx,1),%r8
   14000193d:	e8 3e fe ff ff       	call   140001780 <__report_error>
   140001942:	48 89 da             	mov    %rbx,%rdx
   140001945:	48 8d 0d 94 28 00 00 	lea    0x2894(%rip),%rcx        # 1400041e0 <.rdata+0x20>
   14000194c:	e8 2f fe ff ff       	call   140001780 <__report_error>
   140001951:	90                   	nop
   140001952:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
   140001959:	00 00 00 00 
   14000195d:	0f 1f 00             	nopl   (%rax)

0000000140001960 <_pei386_runtime_relocator>:
   140001960:	55                   	push   %rbp
   140001961:	41 57                	push   %r15
   140001963:	41 56                	push   %r14
   140001965:	41 55                	push   %r13
   140001967:	41 54                	push   %r12
   140001969:	57                   	push   %rdi
   14000196a:	56                   	push   %rsi
   14000196b:	53                   	push   %rbx
   14000196c:	48 83 ec 48          	sub    $0x48,%rsp
   140001970:	48 8d 6c 24 40       	lea    0x40(%rsp),%rbp
   140001975:	44 8b 25 14 57 00 00 	mov    0x5714(%rip),%r12d        # 140007090 <was_init.0>
   14000197c:	45 85 e4             	test   %r12d,%r12d
   14000197f:	74 17                	je     140001998 <_pei386_runtime_relocator+0x38>
   140001981:	48 8d 65 08          	lea    0x8(%rbp),%rsp
   140001985:	5b                   	pop    %rbx
   140001986:	5e                   	pop    %rsi
   140001987:	5f                   	pop    %rdi
   140001988:	41 5c                	pop    %r12
   14000198a:	41 5d                	pop    %r13
   14000198c:	41 5e                	pop    %r14
   14000198e:	41 5f                	pop    %r15
   140001990:	5d                   	pop    %rbp
   140001991:	c3                   	ret
   140001992:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
   140001998:	c7 05 ee 56 00 00 01 	movl   $0x1,0x56ee(%rip)        # 140007090 <was_init.0>
   14000199f:	00 00 00 
   1400019a2:	e8 39 09 00 00       	call   1400022e0 <__mingw_GetSectionCount>
   1400019a7:	48 98                	cltq
   1400019a9:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
   1400019ad:	48 8d 04 c5 0f 00 00 	lea    0xf(,%rax,8),%rax
   1400019b4:	00 
   1400019b5:	48 83 e0 f0          	and    $0xfffffffffffffff0,%rax
   1400019b9:	e8 82 0b 00 00       	call   140002540 <___chkstk_ms>
   1400019be:	4c 8b 2d eb 29 00 00 	mov    0x29eb(%rip),%r13        # 1400043b0 <.refptr.__RUNTIME_PSEUDO_RELOC_LIST_END__>
   1400019c5:	48 8b 1d f4 29 00 00 	mov    0x29f4(%rip),%rbx        # 1400043c0 <.refptr.__RUNTIME_PSEUDO_RELOC_LIST__>
   1400019cc:	c7 05 be 56 00 00 00 	movl   $0x0,0x56be(%rip)        # 140007094 <maxSections>
   1400019d3:	00 00 00 
   1400019d6:	48 29 c4             	sub    %rax,%rsp
   1400019d9:	48 8d 44 24 30       	lea    0x30(%rsp),%rax
   1400019de:	48 89 05 b3 56 00 00 	mov    %rax,0x56b3(%rip)        # 140007098 <the_secs>
   1400019e5:	4c 89 e8             	mov    %r13,%rax
   1400019e8:	48 29 d8             	sub    %rbx,%rax
   1400019eb:	48 83 f8 07          	cmp    $0x7,%rax
   1400019ef:	7e 90                	jle    140001981 <_pei386_runtime_relocator+0x21>
   1400019f1:	8b 13                	mov    (%rbx),%edx
   1400019f3:	48 83 f8 0b          	cmp    $0xb,%rax
   1400019f7:	0f 8f 03 01 00 00    	jg     140001b00 <_pei386_runtime_relocator+0x1a0>
   1400019fd:	8b 03                	mov    (%rbx),%eax
   1400019ff:	85 c0                	test   %eax,%eax
   140001a01:	0f 85 69 02 00 00    	jne    140001c70 <_pei386_runtime_relocator+0x310>
   140001a07:	8b 43 04             	mov    0x4(%rbx),%eax
   140001a0a:	85 c0                	test   %eax,%eax
   140001a0c:	0f 85 5e 02 00 00    	jne    140001c70 <_pei386_runtime_relocator+0x310>
   140001a12:	8b 53 08             	mov    0x8(%rbx),%edx
   140001a15:	83 fa 01             	cmp    $0x1,%edx
   140001a18:	0f 85 92 02 00 00    	jne    140001cb0 <_pei386_runtime_relocator+0x350>
   140001a1e:	48 83 c3 0c          	add    $0xc,%rbx
   140001a22:	4c 39 eb             	cmp    %r13,%rbx
   140001a25:	0f 83 56 ff ff ff    	jae    140001981 <_pei386_runtime_relocator+0x21>
   140001a2b:	4c 8b 35 6e 29 00 00 	mov    0x296e(%rip),%r14        # 1400043a0 <.refptr.__ImageBase>
   140001a32:	41 bf ff ff ff ff    	mov    $0xffffffff,%r15d
   140001a38:	eb 65                	jmp    140001a9f <_pei386_runtime_relocator+0x13f>
   140001a3a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
   140001a40:	83 f9 08             	cmp    $0x8,%ecx
   140001a43:	0f 84 d7 00 00 00    	je     140001b20 <_pei386_runtime_relocator+0x1c0>
   140001a49:	83 f9 10             	cmp    $0x10,%ecx
   140001a4c:	0f 85 50 02 00 00    	jne    140001ca2 <_pei386_runtime_relocator+0x342>
   140001a52:	0f b7 37             	movzwl (%rdi),%esi
   140001a55:	81 e2 c0 00 00 00    	and    $0xc0,%edx
   140001a5b:	66 85 f6             	test   %si,%si
   140001a5e:	0f 89 cc 01 00 00    	jns    140001c30 <_pei386_runtime_relocator+0x2d0>
   140001a64:	48 81 ce 00 00 ff ff 	or     $0xffffffffffff0000,%rsi
   140001a6b:	48 29 c6             	sub    %rax,%rsi
   140001a6e:	4c 01 ce             	add    %r9,%rsi
   140001a71:	85 d2                	test   %edx,%edx
   140001a73:	75 12                	jne    140001a87 <_pei386_runtime_relocator+0x127>
   140001a75:	48 81 fe 00 80 ff ff 	cmp    $0xffffffffffff8000,%rsi
   140001a7c:	7c 65                	jl     140001ae3 <_pei386_runtime_relocator+0x183>
   140001a7e:	48 81 fe ff ff 00 00 	cmp    $0xffff,%rsi
   140001a85:	7f 5c                	jg     140001ae3 <_pei386_runtime_relocator+0x183>
   140001a87:	48 89 f9             	mov    %rdi,%rcx
   140001a8a:	e8 61 fd ff ff       	call   1400017f0 <mark_section_writable>
   140001a8f:	66 89 37             	mov    %si,(%rdi)
   140001a92:	48 83 c3 0c          	add    $0xc,%rbx
   140001a96:	4c 39 eb             	cmp    %r13,%rbx
   140001a99:	0f 83 d1 00 00 00    	jae    140001b70 <_pei386_runtime_relocator+0x210>
   140001a9f:	8b 03                	mov    (%rbx),%eax
   140001aa1:	8b 53 08             	mov    0x8(%rbx),%edx
   140001aa4:	8b 7b 04             	mov    0x4(%rbx),%edi
   140001aa7:	4c 01 f0             	add    %r14,%rax
   140001aaa:	0f b6 ca             	movzbl %dl,%ecx
   140001aad:	4c 8b 08             	mov    (%rax),%r9
   140001ab0:	4c 01 f7             	add    %r14,%rdi
   140001ab3:	83 f9 20             	cmp    $0x20,%ecx
   140001ab6:	0f 84 0c 01 00 00    	je     140001bc8 <_pei386_runtime_relocator+0x268>
   140001abc:	76 82                	jbe    140001a40 <_pei386_runtime_relocator+0xe0>
   140001abe:	83 f9 40             	cmp    $0x40,%ecx
   140001ac1:	0f 85 db 01 00 00    	jne    140001ca2 <_pei386_runtime_relocator+0x342>
   140001ac7:	48 8b 37             	mov    (%rdi),%rsi
   140001aca:	89 d1                	mov    %edx,%ecx
   140001acc:	48 29 c6             	sub    %rax,%rsi
   140001acf:	4c 01 ce             	add    %r9,%rsi
   140001ad2:	81 e1 c0 00 00 00    	and    $0xc0,%ecx
   140001ad8:	0f 85 42 01 00 00    	jne    140001c20 <_pei386_runtime_relocator+0x2c0>
   140001ade:	48 85 f6             	test   %rsi,%rsi
   140001ae1:	78 af                	js     140001a92 <_pei386_runtime_relocator+0x132>
   140001ae3:	48 89 74 24 20       	mov    %rsi,0x20(%rsp)
   140001ae8:	89 ca                	mov    %ecx,%edx
   140001aea:	49 89 f8             	mov    %rdi,%r8
   140001aed:	48 8d 0d d4 27 00 00 	lea    0x27d4(%rip),%rcx        # 1400042c8 <.rdata+0x108>
   140001af4:	e8 87 fc ff ff       	call   140001780 <__report_error>
   140001af9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140001b00:	85 d2                	test   %edx,%edx
   140001b02:	0f 85 68 01 00 00    	jne    140001c70 <_pei386_runtime_relocator+0x310>
   140001b08:	8b 43 04             	mov    0x4(%rbx),%eax
   140001b0b:	89 c2                	mov    %eax,%edx
   140001b0d:	0b 53 08             	or     0x8(%rbx),%edx
   140001b10:	0f 85 f4 fe ff ff    	jne    140001a0a <_pei386_runtime_relocator+0xaa>
   140001b16:	48 83 c3 0c          	add    $0xc,%rbx
   140001b1a:	e9 de fe ff ff       	jmp    1400019fd <_pei386_runtime_relocator+0x9d>
   140001b1f:	90                   	nop
   140001b20:	0f b6 37             	movzbl (%rdi),%esi
   140001b23:	81 e2 c0 00 00 00    	and    $0xc0,%edx
   140001b29:	40 84 f6             	test   %sil,%sil
   140001b2c:	0f 89 26 01 00 00    	jns    140001c58 <_pei386_runtime_relocator+0x2f8>
   140001b32:	48 81 ce 00 ff ff ff 	or     $0xffffffffffffff00,%rsi
   140001b39:	48 29 c6             	sub    %rax,%rsi
   140001b3c:	4c 01 ce             	add    %r9,%rsi
   140001b3f:	85 d2                	test   %edx,%edx
   140001b41:	75 0f                	jne    140001b52 <_pei386_runtime_relocator+0x1f2>
   140001b43:	48 81 fe ff 00 00 00 	cmp    $0xff,%rsi
   140001b4a:	7f 97                	jg     140001ae3 <_pei386_runtime_relocator+0x183>
   140001b4c:	48 83 fe 80          	cmp    $0xffffffffffffff80,%rsi
   140001b50:	7c 91                	jl     140001ae3 <_pei386_runtime_relocator+0x183>
   140001b52:	48 89 f9             	mov    %rdi,%rcx
   140001b55:	48 83 c3 0c          	add    $0xc,%rbx
   140001b59:	e8 92 fc ff ff       	call   1400017f0 <mark_section_writable>
   140001b5e:	40 88 37             	mov    %sil,(%rdi)
   140001b61:	4c 39 eb             	cmp    %r13,%rbx
   140001b64:	0f 82 35 ff ff ff    	jb     140001a9f <_pei386_runtime_relocator+0x13f>
   140001b6a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
   140001b70:	8b 15 1e 55 00 00    	mov    0x551e(%rip),%edx        # 140007094 <maxSections>
   140001b76:	85 d2                	test   %edx,%edx
   140001b78:	0f 8e 03 fe ff ff    	jle    140001981 <_pei386_runtime_relocator+0x21>
   140001b7e:	48 8b 35 73 67 00 00 	mov    0x6773(%rip),%rsi        # 1400082f8 <__imp_VirtualProtect>
   140001b85:	31 db                	xor    %ebx,%ebx
   140001b87:	48 8d 7d fc          	lea    -0x4(%rbp),%rdi
   140001b8b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   140001b90:	48 8b 05 01 55 00 00 	mov    0x5501(%rip),%rax        # 140007098 <the_secs>
   140001b97:	48 01 d8             	add    %rbx,%rax
   140001b9a:	44 8b 00             	mov    (%rax),%r8d
   140001b9d:	45 85 c0             	test   %r8d,%r8d
   140001ba0:	74 0d                	je     140001baf <_pei386_runtime_relocator+0x24f>
   140001ba2:	48 8b 50 10          	mov    0x10(%rax),%rdx
   140001ba6:	48 8b 48 08          	mov    0x8(%rax),%rcx
   140001baa:	49 89 f9             	mov    %rdi,%r9
   140001bad:	ff d6                	call   *%rsi
   140001baf:	41 83 c4 01          	add    $0x1,%r12d
   140001bb3:	48 83 c3 28          	add    $0x28,%rbx
   140001bb7:	44 3b 25 d6 54 00 00 	cmp    0x54d6(%rip),%r12d        # 140007094 <maxSections>
   140001bbe:	7c d0                	jl     140001b90 <_pei386_runtime_relocator+0x230>
   140001bc0:	e9 bc fd ff ff       	jmp    140001981 <_pei386_runtime_relocator+0x21>
   140001bc5:	0f 1f 00             	nopl   (%rax)
   140001bc8:	8b 37                	mov    (%rdi),%esi
   140001bca:	81 e2 c0 00 00 00    	and    $0xc0,%edx
   140001bd0:	85 f6                	test   %esi,%esi
   140001bd2:	79 74                	jns    140001c48 <_pei386_runtime_relocator+0x2e8>
   140001bd4:	49 bb 00 00 00 00 ff 	movabs $0xffffffff00000000,%r11
   140001bdb:	ff ff ff 
   140001bde:	4c 09 de             	or     %r11,%rsi
   140001be1:	48 29 c6             	sub    %rax,%rsi
   140001be4:	4c 01 ce             	add    %r9,%rsi
   140001be7:	85 d2                	test   %edx,%edx
   140001be9:	75 1c                	jne    140001c07 <_pei386_runtime_relocator+0x2a7>
   140001beb:	4c 39 fe             	cmp    %r15,%rsi
   140001bee:	0f 8f ef fe ff ff    	jg     140001ae3 <_pei386_runtime_relocator+0x183>
   140001bf4:	48 b8 ff ff ff 7f ff 	movabs $0xffffffff7fffffff,%rax
   140001bfb:	ff ff ff 
   140001bfe:	48 39 c6             	cmp    %rax,%rsi
   140001c01:	0f 8e dc fe ff ff    	jle    140001ae3 <_pei386_runtime_relocator+0x183>
   140001c07:	48 89 f9             	mov    %rdi,%rcx
   140001c0a:	e8 e1 fb ff ff       	call   1400017f0 <mark_section_writable>
   140001c0f:	89 37                	mov    %esi,(%rdi)
   140001c11:	e9 7c fe ff ff       	jmp    140001a92 <_pei386_runtime_relocator+0x132>
   140001c16:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   140001c1d:	00 00 00 
   140001c20:	48 89 f9             	mov    %rdi,%rcx
   140001c23:	e8 c8 fb ff ff       	call   1400017f0 <mark_section_writable>
   140001c28:	48 89 37             	mov    %rsi,(%rdi)
   140001c2b:	e9 62 fe ff ff       	jmp    140001a92 <_pei386_runtime_relocator+0x132>
   140001c30:	48 29 c6             	sub    %rax,%rsi
   140001c33:	4c 01 ce             	add    %r9,%rsi
   140001c36:	85 d2                	test   %edx,%edx
   140001c38:	0f 84 37 fe ff ff    	je     140001a75 <_pei386_runtime_relocator+0x115>
   140001c3e:	e9 44 fe ff ff       	jmp    140001a87 <_pei386_runtime_relocator+0x127>
   140001c43:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   140001c48:	48 29 c6             	sub    %rax,%rsi
   140001c4b:	4c 01 ce             	add    %r9,%rsi
   140001c4e:	85 d2                	test   %edx,%edx
   140001c50:	74 99                	je     140001beb <_pei386_runtime_relocator+0x28b>
   140001c52:	eb b3                	jmp    140001c07 <_pei386_runtime_relocator+0x2a7>
   140001c54:	0f 1f 40 00          	nopl   0x0(%rax)
   140001c58:	48 29 c6             	sub    %rax,%rsi
   140001c5b:	4c 01 ce             	add    %r9,%rsi
   140001c5e:	85 d2                	test   %edx,%edx
   140001c60:	0f 84 dd fe ff ff    	je     140001b43 <_pei386_runtime_relocator+0x1e3>
   140001c66:	e9 e7 fe ff ff       	jmp    140001b52 <_pei386_runtime_relocator+0x1f2>
   140001c6b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   140001c70:	4c 39 eb             	cmp    %r13,%rbx
   140001c73:	0f 83 08 fd ff ff    	jae    140001981 <_pei386_runtime_relocator+0x21>
   140001c79:	4c 8b 35 20 27 00 00 	mov    0x2720(%rip),%r14        # 1400043a0 <.refptr.__ImageBase>
   140001c80:	8b 73 04             	mov    0x4(%rbx),%esi
   140001c83:	8b 3b                	mov    (%rbx),%edi
   140001c85:	48 83 c3 08          	add    $0x8,%rbx
   140001c89:	4c 01 f6             	add    %r14,%rsi
   140001c8c:	03 3e                	add    (%rsi),%edi
   140001c8e:	48 89 f1             	mov    %rsi,%rcx
   140001c91:	e8 5a fb ff ff       	call   1400017f0 <mark_section_writable>
   140001c96:	89 3e                	mov    %edi,(%rsi)
   140001c98:	4c 39 eb             	cmp    %r13,%rbx
   140001c9b:	72 e3                	jb     140001c80 <_pei386_runtime_relocator+0x320>
   140001c9d:	e9 ce fe ff ff       	jmp    140001b70 <_pei386_runtime_relocator+0x210>
   140001ca2:	89 ca                	mov    %ecx,%edx
   140001ca4:	48 8d 0d ed 25 00 00 	lea    0x25ed(%rip),%rcx        # 140004298 <.rdata+0xd8>
   140001cab:	e8 d0 fa ff ff       	call   140001780 <__report_error>
   140001cb0:	48 8d 0d a9 25 00 00 	lea    0x25a9(%rip),%rcx        # 140004260 <.rdata+0xa0>
   140001cb7:	e8 c4 fa ff ff       	call   140001780 <__report_error>
   140001cbc:	90                   	nop
   140001cbd:	90                   	nop
   140001cbe:	90                   	nop
   140001cbf:	90                   	nop

0000000140001cc0 <__mingw_raise_matherr>:
   140001cc0:	48 83 ec 58          	sub    $0x58,%rsp
   140001cc4:	48 8b 05 d5 53 00 00 	mov    0x53d5(%rip),%rax        # 1400070a0 <stUserMathErr>
   140001ccb:	66 0f 14 d3          	unpcklpd %xmm3,%xmm2
   140001ccf:	48 85 c0             	test   %rax,%rax
   140001cd2:	74 25                	je     140001cf9 <__mingw_raise_matherr+0x39>
   140001cd4:	f2 0f 10 84 24 80 00 	movsd  0x80(%rsp),%xmm0
   140001cdb:	00 00 
   140001cdd:	89 4c 24 20          	mov    %ecx,0x20(%rsp)
   140001ce1:	48 8d 4c 24 20       	lea    0x20(%rsp),%rcx
   140001ce6:	48 89 54 24 28       	mov    %rdx,0x28(%rsp)
   140001ceb:	0f 11 54 24 30       	movups %xmm2,0x30(%rsp)
   140001cf0:	f2 0f 11 44 24 40    	movsd  %xmm0,0x40(%rsp)
   140001cf6:	ff d0                	call   *%rax
   140001cf8:	90                   	nop
   140001cf9:	48 83 c4 58          	add    $0x58,%rsp
   140001cfd:	c3                   	ret
   140001cfe:	66 90                	xchg   %ax,%ax

0000000140001d00 <__mingw_setusermatherr>:
   140001d00:	48 89 0d 99 53 00 00 	mov    %rcx,0x5399(%rip)        # 1400070a0 <stUserMathErr>
   140001d07:	e9 44 0c 00 00       	jmp    140002950 <__setusermatherr>
   140001d0c:	90                   	nop
   140001d0d:	90                   	nop
   140001d0e:	90                   	nop
   140001d0f:	90                   	nop

0000000140001d10 <_gnu_exception_handler>:
   140001d10:	53                   	push   %rbx
   140001d11:	48 83 ec 20          	sub    $0x20,%rsp
   140001d15:	48 8b 11             	mov    (%rcx),%rdx
   140001d18:	8b 02                	mov    (%rdx),%eax
   140001d1a:	48 89 cb             	mov    %rcx,%rbx
   140001d1d:	89 c1                	mov    %eax,%ecx
   140001d1f:	81 e1 ff ff ff 20    	and    $0x20ffffff,%ecx
   140001d25:	81 f9 43 43 47 20    	cmp    $0x20474343,%ecx
   140001d2b:	0f 84 bf 00 00 00    	je     140001df0 <_gnu_exception_handler+0xe0>
   140001d31:	3d 96 00 00 c0       	cmp    $0xc0000096,%eax
   140001d36:	77 47                	ja     140001d7f <_gnu_exception_handler+0x6f>
   140001d38:	3d 8b 00 00 c0       	cmp    $0xc000008b,%eax
   140001d3d:	76 61                	jbe    140001da0 <_gnu_exception_handler+0x90>
   140001d3f:	05 73 ff ff 3f       	add    $0x3fffff73,%eax
   140001d44:	83 f8 09             	cmp    $0x9,%eax
   140001d47:	0f 87 93 00 00 00    	ja     140001de0 <_gnu_exception_handler+0xd0>
   140001d4d:	48 8d 15 cc 25 00 00 	lea    0x25cc(%rip),%rdx        # 140004320 <.rdata>
   140001d54:	48 63 04 82          	movslq (%rdx,%rax,4),%rax
   140001d58:	48 01 d0             	add    %rdx,%rax
   140001d5b:	ff e0                	jmp    *%rax
   140001d5d:	0f 1f 00             	nopl   (%rax)
   140001d60:	31 d2                	xor    %edx,%edx
   140001d62:	b9 08 00 00 00       	mov    $0x8,%ecx
   140001d67:	e8 c4 0b 00 00       	call   140002930 <signal>
   140001d6c:	48 83 f8 01          	cmp    $0x1,%rax
   140001d70:	0f 84 3e 01 00 00    	je     140001eb4 <_gnu_exception_handler+0x1a4>
   140001d76:	48 85 c0             	test   %rax,%rax
   140001d79:	0f 85 01 01 00 00    	jne    140001e80 <_gnu_exception_handler+0x170>
   140001d7f:	48 8b 05 3a 53 00 00 	mov    0x533a(%rip),%rax        # 1400070c0 <__mingw_oldexcpt_handler>
   140001d86:	48 85 c0             	test   %rax,%rax
   140001d89:	74 75                	je     140001e00 <_gnu_exception_handler+0xf0>
   140001d8b:	48 89 d9             	mov    %rbx,%rcx
   140001d8e:	48 83 c4 20          	add    $0x20,%rsp
   140001d92:	5b                   	pop    %rbx
   140001d93:	48 ff e0             	rex.W jmp *%rax
   140001d96:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   140001d9d:	00 00 00 
   140001da0:	3d 05 00 00 c0       	cmp    $0xc0000005,%eax
   140001da5:	0f 84 a5 00 00 00    	je     140001e50 <_gnu_exception_handler+0x140>
   140001dab:	76 63                	jbe    140001e10 <_gnu_exception_handler+0x100>
   140001dad:	3d 08 00 00 c0       	cmp    $0xc0000008,%eax
   140001db2:	74 2c                	je     140001de0 <_gnu_exception_handler+0xd0>
   140001db4:	3d 1d 00 00 c0       	cmp    $0xc000001d,%eax
   140001db9:	75 c4                	jne    140001d7f <_gnu_exception_handler+0x6f>
   140001dbb:	31 d2                	xor    %edx,%edx
   140001dbd:	b9 04 00 00 00       	mov    $0x4,%ecx
   140001dc2:	e8 69 0b 00 00       	call   140002930 <signal>
   140001dc7:	48 83 f8 01          	cmp    $0x1,%rax
   140001dcb:	0f 84 cf 00 00 00    	je     140001ea0 <_gnu_exception_handler+0x190>
   140001dd1:	48 85 c0             	test   %rax,%rax
   140001dd4:	74 a9                	je     140001d7f <_gnu_exception_handler+0x6f>
   140001dd6:	b9 04 00 00 00       	mov    $0x4,%ecx
   140001ddb:	ff d0                	call   *%rax
   140001ddd:	0f 1f 00             	nopl   (%rax)
   140001de0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
   140001de5:	eb 1b                	jmp    140001e02 <_gnu_exception_handler+0xf2>
   140001de7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   140001dee:	00 00 
   140001df0:	f6 42 04 01          	testb  $0x1,0x4(%rdx)
   140001df4:	0f 85 37 ff ff ff    	jne    140001d31 <_gnu_exception_handler+0x21>
   140001dfa:	eb e4                	jmp    140001de0 <_gnu_exception_handler+0xd0>
   140001dfc:	0f 1f 40 00          	nopl   0x0(%rax)
   140001e00:	31 c0                	xor    %eax,%eax
   140001e02:	48 83 c4 20          	add    $0x20,%rsp
   140001e06:	5b                   	pop    %rbx
   140001e07:	c3                   	ret
   140001e08:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
   140001e0f:	00 
   140001e10:	3d 02 00 00 80       	cmp    $0x80000002,%eax
   140001e15:	0f 85 64 ff ff ff    	jne    140001d7f <_gnu_exception_handler+0x6f>
   140001e1b:	eb c3                	jmp    140001de0 <_gnu_exception_handler+0xd0>
   140001e1d:	0f 1f 00             	nopl   (%rax)
   140001e20:	31 d2                	xor    %edx,%edx
   140001e22:	b9 08 00 00 00       	mov    $0x8,%ecx
   140001e27:	e8 04 0b 00 00       	call   140002930 <signal>
   140001e2c:	48 83 f8 01          	cmp    $0x1,%rax
   140001e30:	0f 85 40 ff ff ff    	jne    140001d76 <_gnu_exception_handler+0x66>
   140001e36:	ba 01 00 00 00       	mov    $0x1,%edx
   140001e3b:	b9 08 00 00 00       	mov    $0x8,%ecx
   140001e40:	e8 eb 0a 00 00       	call   140002930 <signal>
   140001e45:	eb 99                	jmp    140001de0 <_gnu_exception_handler+0xd0>
   140001e47:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   140001e4e:	00 00 
   140001e50:	31 d2                	xor    %edx,%edx
   140001e52:	b9 0b 00 00 00       	mov    $0xb,%ecx
   140001e57:	e8 d4 0a 00 00       	call   140002930 <signal>
   140001e5c:	48 83 f8 01          	cmp    $0x1,%rax
   140001e60:	74 2a                	je     140001e8c <_gnu_exception_handler+0x17c>
   140001e62:	48 85 c0             	test   %rax,%rax
   140001e65:	0f 84 14 ff ff ff    	je     140001d7f <_gnu_exception_handler+0x6f>
   140001e6b:	b9 0b 00 00 00       	mov    $0xb,%ecx
   140001e70:	ff d0                	call   *%rax
   140001e72:	e9 69 ff ff ff       	jmp    140001de0 <_gnu_exception_handler+0xd0>
   140001e77:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   140001e7e:	00 00 
   140001e80:	b9 08 00 00 00       	mov    $0x8,%ecx
   140001e85:	ff d0                	call   *%rax
   140001e87:	e9 54 ff ff ff       	jmp    140001de0 <_gnu_exception_handler+0xd0>
   140001e8c:	ba 01 00 00 00       	mov    $0x1,%edx
   140001e91:	b9 0b 00 00 00       	mov    $0xb,%ecx
   140001e96:	e8 95 0a 00 00       	call   140002930 <signal>
   140001e9b:	e9 40 ff ff ff       	jmp    140001de0 <_gnu_exception_handler+0xd0>
   140001ea0:	ba 01 00 00 00       	mov    $0x1,%edx
   140001ea5:	b9 04 00 00 00       	mov    $0x4,%ecx
   140001eaa:	e8 81 0a 00 00       	call   140002930 <signal>
   140001eaf:	e9 2c ff ff ff       	jmp    140001de0 <_gnu_exception_handler+0xd0>
   140001eb4:	ba 01 00 00 00       	mov    $0x1,%edx
   140001eb9:	b9 08 00 00 00       	mov    $0x8,%ecx
   140001ebe:	e8 6d 0a 00 00       	call   140002930 <signal>
   140001ec3:	e8 a8 f8 ff ff       	call   140001770 <_fpreset>
   140001ec8:	e9 13 ff ff ff       	jmp    140001de0 <_gnu_exception_handler+0xd0>
   140001ecd:	90                   	nop
   140001ece:	90                   	nop
   140001ecf:	90                   	nop

0000000140001ed0 <__mingwthr_run_key_dtors.part.0>:
   140001ed0:	41 54                	push   %r12
   140001ed2:	55                   	push   %rbp
   140001ed3:	57                   	push   %rdi
   140001ed4:	56                   	push   %rsi
   140001ed5:	53                   	push   %rbx
   140001ed6:	48 83 ec 20          	sub    $0x20,%rsp
   140001eda:	4c 8d 25 1f 52 00 00 	lea    0x521f(%rip),%r12        # 140007100 <__mingwthr_cs>
   140001ee1:	4c 89 e1             	mov    %r12,%rcx
   140001ee4:	ff 15 d6 63 00 00    	call   *0x63d6(%rip)        # 1400082c0 <__imp_EnterCriticalSection>
   140001eea:	48 8b 1d ef 51 00 00 	mov    0x51ef(%rip),%rbx        # 1400070e0 <key_dtor_list>
   140001ef1:	48 85 db             	test   %rbx,%rbx
   140001ef4:	74 36                	je     140001f2c <__mingwthr_run_key_dtors.part.0+0x5c>
   140001ef6:	48 8b 2d f3 63 00 00 	mov    0x63f3(%rip),%rbp        # 1400082f0 <__imp_TlsGetValue>
   140001efd:	48 8b 3d c4 63 00 00 	mov    0x63c4(%rip),%rdi        # 1400082c8 <__imp_GetLastError>
   140001f04:	0f 1f 40 00          	nopl   0x0(%rax)
   140001f08:	8b 0b                	mov    (%rbx),%ecx
   140001f0a:	ff d5                	call   *%rbp
   140001f0c:	48 89 c6             	mov    %rax,%rsi
   140001f0f:	ff d7                	call   *%rdi
   140001f11:	48 85 f6             	test   %rsi,%rsi
   140001f14:	74 0d                	je     140001f23 <__mingwthr_run_key_dtors.part.0+0x53>
   140001f16:	85 c0                	test   %eax,%eax
   140001f18:	75 09                	jne    140001f23 <__mingwthr_run_key_dtors.part.0+0x53>
   140001f1a:	48 8b 43 08          	mov    0x8(%rbx),%rax
   140001f1e:	48 89 f1             	mov    %rsi,%rcx
   140001f21:	ff d0                	call   *%rax
   140001f23:	48 8b 5b 10          	mov    0x10(%rbx),%rbx
   140001f27:	48 85 db             	test   %rbx,%rbx
   140001f2a:	75 dc                	jne    140001f08 <__mingwthr_run_key_dtors.part.0+0x38>
   140001f2c:	4c 89 e1             	mov    %r12,%rcx
   140001f2f:	48 83 c4 20          	add    $0x20,%rsp
   140001f33:	5b                   	pop    %rbx
   140001f34:	5e                   	pop    %rsi
   140001f35:	5f                   	pop    %rdi
   140001f36:	5d                   	pop    %rbp
   140001f37:	41 5c                	pop    %r12
   140001f39:	48 ff 25 98 63 00 00 	rex.W jmp *0x6398(%rip)        # 1400082d8 <__imp_LeaveCriticalSection>

0000000140001f40 <___w64_mingwthr_add_key_dtor>:
   140001f40:	57                   	push   %rdi
   140001f41:	56                   	push   %rsi
   140001f42:	53                   	push   %rbx
   140001f43:	48 83 ec 20          	sub    $0x20,%rsp
   140001f47:	8b 05 9b 51 00 00    	mov    0x519b(%rip),%eax        # 1400070e8 <__mingwthr_cs_init>
   140001f4d:	89 cf                	mov    %ecx,%edi
   140001f4f:	48 89 d6             	mov    %rdx,%rsi
   140001f52:	85 c0                	test   %eax,%eax
   140001f54:	75 0a                	jne    140001f60 <___w64_mingwthr_add_key_dtor+0x20>
   140001f56:	31 c0                	xor    %eax,%eax
   140001f58:	48 83 c4 20          	add    $0x20,%rsp
   140001f5c:	5b                   	pop    %rbx
   140001f5d:	5e                   	pop    %rsi
   140001f5e:	5f                   	pop    %rdi
   140001f5f:	c3                   	ret
   140001f60:	ba 18 00 00 00       	mov    $0x18,%edx
   140001f65:	b9 01 00 00 00       	mov    $0x1,%ecx
   140001f6a:	e8 f9 09 00 00       	call   140002968 <calloc>
   140001f6f:	48 89 c3             	mov    %rax,%rbx
   140001f72:	48 85 c0             	test   %rax,%rax
   140001f75:	74 33                	je     140001faa <___w64_mingwthr_add_key_dtor+0x6a>
   140001f77:	48 89 70 08          	mov    %rsi,0x8(%rax)
   140001f7b:	48 8d 35 7e 51 00 00 	lea    0x517e(%rip),%rsi        # 140007100 <__mingwthr_cs>
   140001f82:	89 38                	mov    %edi,(%rax)
   140001f84:	48 89 f1             	mov    %rsi,%rcx
   140001f87:	ff 15 33 63 00 00    	call   *0x6333(%rip)        # 1400082c0 <__imp_EnterCriticalSection>
   140001f8d:	48 8b 05 4c 51 00 00 	mov    0x514c(%rip),%rax        # 1400070e0 <key_dtor_list>
   140001f94:	48 89 f1             	mov    %rsi,%rcx
   140001f97:	48 89 1d 42 51 00 00 	mov    %rbx,0x5142(%rip)        # 1400070e0 <key_dtor_list>
   140001f9e:	48 89 43 10          	mov    %rax,0x10(%rbx)
   140001fa2:	ff 15 30 63 00 00    	call   *0x6330(%rip)        # 1400082d8 <__imp_LeaveCriticalSection>
   140001fa8:	eb ac                	jmp    140001f56 <___w64_mingwthr_add_key_dtor+0x16>
   140001faa:	83 c8 ff             	or     $0xffffffff,%eax
   140001fad:	eb a9                	jmp    140001f58 <___w64_mingwthr_add_key_dtor+0x18>
   140001faf:	90                   	nop

0000000140001fb0 <___w64_mingwthr_remove_key_dtor>:
   140001fb0:	56                   	push   %rsi
   140001fb1:	53                   	push   %rbx
   140001fb2:	48 83 ec 28          	sub    $0x28,%rsp
   140001fb6:	8b 05 2c 51 00 00    	mov    0x512c(%rip),%eax        # 1400070e8 <__mingwthr_cs_init>
   140001fbc:	89 cb                	mov    %ecx,%ebx
   140001fbe:	85 c0                	test   %eax,%eax
   140001fc0:	75 0e                	jne    140001fd0 <___w64_mingwthr_remove_key_dtor+0x20>
   140001fc2:	31 c0                	xor    %eax,%eax
   140001fc4:	48 83 c4 28          	add    $0x28,%rsp
   140001fc8:	5b                   	pop    %rbx
   140001fc9:	5e                   	pop    %rsi
   140001fca:	c3                   	ret
   140001fcb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   140001fd0:	48 8d 35 29 51 00 00 	lea    0x5129(%rip),%rsi        # 140007100 <__mingwthr_cs>
   140001fd7:	48 89 f1             	mov    %rsi,%rcx
   140001fda:	ff 15 e0 62 00 00    	call   *0x62e0(%rip)        # 1400082c0 <__imp_EnterCriticalSection>
   140001fe0:	48 8b 0d f9 50 00 00 	mov    0x50f9(%rip),%rcx        # 1400070e0 <key_dtor_list>
   140001fe7:	48 85 c9             	test   %rcx,%rcx
   140001fea:	74 27                	je     140002013 <___w64_mingwthr_remove_key_dtor+0x63>
   140001fec:	31 d2                	xor    %edx,%edx
   140001fee:	eb 0b                	jmp    140001ffb <___w64_mingwthr_remove_key_dtor+0x4b>
   140001ff0:	48 89 ca             	mov    %rcx,%rdx
   140001ff3:	48 85 c0             	test   %rax,%rax
   140001ff6:	74 1b                	je     140002013 <___w64_mingwthr_remove_key_dtor+0x63>
   140001ff8:	48 89 c1             	mov    %rax,%rcx
   140001ffb:	8b 01                	mov    (%rcx),%eax
   140001ffd:	39 d8                	cmp    %ebx,%eax
   140001fff:	48 8b 41 10          	mov    0x10(%rcx),%rax
   140002003:	75 eb                	jne    140001ff0 <___w64_mingwthr_remove_key_dtor+0x40>
   140002005:	48 85 d2             	test   %rdx,%rdx
   140002008:	74 1e                	je     140002028 <___w64_mingwthr_remove_key_dtor+0x78>
   14000200a:	48 89 42 10          	mov    %rax,0x10(%rdx)
   14000200e:	e8 5d 09 00 00       	call   140002970 <free>
   140002013:	48 89 f1             	mov    %rsi,%rcx
   140002016:	ff 15 bc 62 00 00    	call   *0x62bc(%rip)        # 1400082d8 <__imp_LeaveCriticalSection>
   14000201c:	31 c0                	xor    %eax,%eax
   14000201e:	48 83 c4 28          	add    $0x28,%rsp
   140002022:	5b                   	pop    %rbx
   140002023:	5e                   	pop    %rsi
   140002024:	c3                   	ret
   140002025:	0f 1f 00             	nopl   (%rax)
   140002028:	48 89 05 b1 50 00 00 	mov    %rax,0x50b1(%rip)        # 1400070e0 <key_dtor_list>
   14000202f:	eb dd                	jmp    14000200e <___w64_mingwthr_remove_key_dtor+0x5e>
   140002031:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
   140002038:	00 00 00 00 
   14000203c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000140002040 <__mingw_TLScallback>:
   140002040:	53                   	push   %rbx
   140002041:	48 83 ec 20          	sub    $0x20,%rsp
   140002045:	83 fa 02             	cmp    $0x2,%edx
   140002048:	0f 84 b2 00 00 00    	je     140002100 <__mingw_TLScallback+0xc0>
   14000204e:	77 30                	ja     140002080 <__mingw_TLScallback+0x40>
   140002050:	85 d2                	test   %edx,%edx
   140002052:	74 4c                	je     1400020a0 <__mingw_TLScallback+0x60>
   140002054:	8b 05 8e 50 00 00    	mov    0x508e(%rip),%eax        # 1400070e8 <__mingwthr_cs_init>
   14000205a:	85 c0                	test   %eax,%eax
   14000205c:	0f 84 be 00 00 00    	je     140002120 <__mingw_TLScallback+0xe0>
   140002062:	c7 05 7c 50 00 00 01 	movl   $0x1,0x507c(%rip)        # 1400070e8 <__mingwthr_cs_init>
   140002069:	00 00 00 
   14000206c:	b8 01 00 00 00       	mov    $0x1,%eax
   140002071:	48 83 c4 20          	add    $0x20,%rsp
   140002075:	5b                   	pop    %rbx
   140002076:	c3                   	ret
   140002077:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   14000207e:	00 00 
   140002080:	83 fa 03             	cmp    $0x3,%edx
   140002083:	75 e7                	jne    14000206c <__mingw_TLScallback+0x2c>
   140002085:	8b 05 5d 50 00 00    	mov    0x505d(%rip),%eax        # 1400070e8 <__mingwthr_cs_init>
   14000208b:	85 c0                	test   %eax,%eax
   14000208d:	74 dd                	je     14000206c <__mingw_TLScallback+0x2c>
   14000208f:	e8 3c fe ff ff       	call   140001ed0 <__mingwthr_run_key_dtors.part.0>
   140002094:	eb d6                	jmp    14000206c <__mingw_TLScallback+0x2c>
   140002096:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   14000209d:	00 00 00 
   1400020a0:	8b 05 42 50 00 00    	mov    0x5042(%rip),%eax        # 1400070e8 <__mingwthr_cs_init>
   1400020a6:	85 c0                	test   %eax,%eax
   1400020a8:	75 66                	jne    140002110 <__mingw_TLScallback+0xd0>
   1400020aa:	8b 05 38 50 00 00    	mov    0x5038(%rip),%eax        # 1400070e8 <__mingwthr_cs_init>
   1400020b0:	83 f8 01             	cmp    $0x1,%eax
   1400020b3:	75 b7                	jne    14000206c <__mingw_TLScallback+0x2c>
   1400020b5:	48 8b 1d 24 50 00 00 	mov    0x5024(%rip),%rbx        # 1400070e0 <key_dtor_list>
   1400020bc:	48 85 db             	test   %rbx,%rbx
   1400020bf:	74 18                	je     1400020d9 <__mingw_TLScallback+0x99>
   1400020c1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   1400020c8:	48 89 d9             	mov    %rbx,%rcx
   1400020cb:	48 8b 5b 10          	mov    0x10(%rbx),%rbx
   1400020cf:	e8 9c 08 00 00       	call   140002970 <free>
   1400020d4:	48 85 db             	test   %rbx,%rbx
   1400020d7:	75 ef                	jne    1400020c8 <__mingw_TLScallback+0x88>
   1400020d9:	48 8d 0d 20 50 00 00 	lea    0x5020(%rip),%rcx        # 140007100 <__mingwthr_cs>
   1400020e0:	48 c7 05 f5 4f 00 00 	movq   $0x0,0x4ff5(%rip)        # 1400070e0 <key_dtor_list>
   1400020e7:	00 00 00 00 
   1400020eb:	c7 05 f3 4f 00 00 00 	movl   $0x0,0x4ff3(%rip)        # 1400070e8 <__mingwthr_cs_init>
   1400020f2:	00 00 00 
   1400020f5:	ff 15 bd 61 00 00    	call   *0x61bd(%rip)        # 1400082b8 <__IAT_start__>
   1400020fb:	e9 6c ff ff ff       	jmp    14000206c <__mingw_TLScallback+0x2c>
   140002100:	e8 6b f6 ff ff       	call   140001770 <_fpreset>
   140002105:	b8 01 00 00 00       	mov    $0x1,%eax
   14000210a:	48 83 c4 20          	add    $0x20,%rsp
   14000210e:	5b                   	pop    %rbx
   14000210f:	c3                   	ret
   140002110:	e8 bb fd ff ff       	call   140001ed0 <__mingwthr_run_key_dtors.part.0>
   140002115:	eb 93                	jmp    1400020aa <__mingw_TLScallback+0x6a>
   140002117:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   14000211e:	00 00 
   140002120:	48 8d 0d d9 4f 00 00 	lea    0x4fd9(%rip),%rcx        # 140007100 <__mingwthr_cs>
   140002127:	ff 15 a3 61 00 00    	call   *0x61a3(%rip)        # 1400082d0 <__imp_InitializeCriticalSection>
   14000212d:	e9 30 ff ff ff       	jmp    140002062 <__mingw_TLScallback+0x22>
   140002132:	90                   	nop
   140002133:	90                   	nop
   140002134:	90                   	nop
   140002135:	90                   	nop
   140002136:	90                   	nop
   140002137:	90                   	nop
   140002138:	90                   	nop
   140002139:	90                   	nop
   14000213a:	90                   	nop
   14000213b:	90                   	nop
   14000213c:	90                   	nop
   14000213d:	90                   	nop
   14000213e:	90                   	nop
   14000213f:	90                   	nop

0000000140002140 <_ValidateImageBase>:
   140002140:	31 c0                	xor    %eax,%eax
   140002142:	66 81 39 4d 5a       	cmpw   $0x5a4d,(%rcx)
   140002147:	75 0f                	jne    140002158 <_ValidateImageBase+0x18>
   140002149:	48 63 51 3c          	movslq 0x3c(%rcx),%rdx
   14000214d:	48 01 d1             	add    %rdx,%rcx
   140002150:	81 39 50 45 00 00    	cmpl   $0x4550,(%rcx)
   140002156:	74 08                	je     140002160 <_ValidateImageBase+0x20>
   140002158:	c3                   	ret
   140002159:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140002160:	31 c0                	xor    %eax,%eax
   140002162:	66 81 79 18 0b 02    	cmpw   $0x20b,0x18(%rcx)
   140002168:	0f 94 c0             	sete   %al
   14000216b:	c3                   	ret
   14000216c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000140002170 <_FindPESection>:
   140002170:	48 63 41 3c          	movslq 0x3c(%rcx),%rax
   140002174:	48 01 c1             	add    %rax,%rcx
   140002177:	0f b7 41 14          	movzwl 0x14(%rcx),%eax
   14000217b:	44 0f b7 41 06       	movzwl 0x6(%rcx),%r8d
   140002180:	48 8d 44 01 18       	lea    0x18(%rcx,%rax,1),%rax
   140002185:	66 45 85 c0          	test   %r8w,%r8w
   140002189:	74 32                	je     1400021bd <_FindPESection+0x4d>
   14000218b:	41 8d 48 ff          	lea    -0x1(%r8),%ecx
   14000218f:	48 8d 0c 89          	lea    (%rcx,%rcx,4),%rcx
   140002193:	4c 8d 4c c8 28       	lea    0x28(%rax,%rcx,8),%r9
   140002198:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
   14000219f:	00 
   1400021a0:	44 8b 40 0c          	mov    0xc(%rax),%r8d
   1400021a4:	4c 89 c1             	mov    %r8,%rcx
   1400021a7:	4c 39 c2             	cmp    %r8,%rdx
   1400021aa:	72 08                	jb     1400021b4 <_FindPESection+0x44>
   1400021ac:	03 48 08             	add    0x8(%rax),%ecx
   1400021af:	48 39 ca             	cmp    %rcx,%rdx
   1400021b2:	72 0b                	jb     1400021bf <_FindPESection+0x4f>
   1400021b4:	48 83 c0 28          	add    $0x28,%rax
   1400021b8:	4c 39 c8             	cmp    %r9,%rax
   1400021bb:	75 e3                	jne    1400021a0 <_FindPESection+0x30>
   1400021bd:	31 c0                	xor    %eax,%eax
   1400021bf:	c3                   	ret

00000001400021c0 <_FindPESectionByName>:
   1400021c0:	57                   	push   %rdi
   1400021c1:	56                   	push   %rsi
   1400021c2:	53                   	push   %rbx
   1400021c3:	48 83 ec 20          	sub    $0x20,%rsp
   1400021c7:	48 89 ce             	mov    %rcx,%rsi
   1400021ca:	e8 a1 06 00 00       	call   140002870 <strlen>
   1400021cf:	48 83 f8 08          	cmp    $0x8,%rax
   1400021d3:	77 7b                	ja     140002250 <_FindPESectionByName+0x90>
   1400021d5:	48 8b 15 c4 21 00 00 	mov    0x21c4(%rip),%rdx        # 1400043a0 <.refptr.__ImageBase>
   1400021dc:	31 db                	xor    %ebx,%ebx
   1400021de:	66 81 3a 4d 5a       	cmpw   $0x5a4d,(%rdx)
   1400021e3:	75 59                	jne    14000223e <_FindPESectionByName+0x7e>
   1400021e5:	48 63 42 3c          	movslq 0x3c(%rdx),%rax
   1400021e9:	48 01 d0             	add    %rdx,%rax
   1400021ec:	81 38 50 45 00 00    	cmpl   $0x4550,(%rax)
   1400021f2:	75 4a                	jne    14000223e <_FindPESectionByName+0x7e>
   1400021f4:	66 81 78 18 0b 02    	cmpw   $0x20b,0x18(%rax)
   1400021fa:	75 42                	jne    14000223e <_FindPESectionByName+0x7e>
   1400021fc:	0f b7 50 14          	movzwl 0x14(%rax),%edx
   140002200:	48 8d 5c 10 18       	lea    0x18(%rax,%rdx,1),%rbx
   140002205:	0f b7 50 06          	movzwl 0x6(%rax),%edx
   140002209:	66 85 d2             	test   %dx,%dx
   14000220c:	74 42                	je     140002250 <_FindPESectionByName+0x90>
   14000220e:	8d 42 ff             	lea    -0x1(%rdx),%eax
   140002211:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
   140002215:	48 8d 7c c3 28       	lea    0x28(%rbx,%rax,8),%rdi
   14000221a:	eb 0d                	jmp    140002229 <_FindPESectionByName+0x69>
   14000221c:	0f 1f 40 00          	nopl   0x0(%rax)
   140002220:	48 83 c3 28          	add    $0x28,%rbx
   140002224:	48 39 fb             	cmp    %rdi,%rbx
   140002227:	74 27                	je     140002250 <_FindPESectionByName+0x90>
   140002229:	41 b8 08 00 00 00    	mov    $0x8,%r8d
   14000222f:	48 89 f2             	mov    %rsi,%rdx
   140002232:	48 89 d9             	mov    %rbx,%rcx
   140002235:	e8 3e 06 00 00       	call   140002878 <strncmp>
   14000223a:	85 c0                	test   %eax,%eax
   14000223c:	75 e2                	jne    140002220 <_FindPESectionByName+0x60>
   14000223e:	48 89 d8             	mov    %rbx,%rax
   140002241:	48 83 c4 20          	add    $0x20,%rsp
   140002245:	5b                   	pop    %rbx
   140002246:	5e                   	pop    %rsi
   140002247:	5f                   	pop    %rdi
   140002248:	c3                   	ret
   140002249:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140002250:	31 db                	xor    %ebx,%ebx
   140002252:	48 89 d8             	mov    %rbx,%rax
   140002255:	48 83 c4 20          	add    $0x20,%rsp
   140002259:	5b                   	pop    %rbx
   14000225a:	5e                   	pop    %rsi
   14000225b:	5f                   	pop    %rdi
   14000225c:	c3                   	ret
   14000225d:	0f 1f 00             	nopl   (%rax)

0000000140002260 <__mingw_GetSectionForAddress>:
   140002260:	48 8b 15 39 21 00 00 	mov    0x2139(%rip),%rdx        # 1400043a0 <.refptr.__ImageBase>
   140002267:	31 c0                	xor    %eax,%eax
   140002269:	66 81 3a 4d 5a       	cmpw   $0x5a4d,(%rdx)
   14000226e:	75 10                	jne    140002280 <__mingw_GetSectionForAddress+0x20>
   140002270:	4c 63 42 3c          	movslq 0x3c(%rdx),%r8
   140002274:	49 01 d0             	add    %rdx,%r8
   140002277:	41 81 38 50 45 00 00 	cmpl   $0x4550,(%r8)
   14000227e:	74 08                	je     140002288 <__mingw_GetSectionForAddress+0x28>
   140002280:	c3                   	ret
   140002281:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140002288:	66 41 81 78 18 0b 02 	cmpw   $0x20b,0x18(%r8)
   14000228f:	75 ef                	jne    140002280 <__mingw_GetSectionForAddress+0x20>
   140002291:	41 0f b7 40 14       	movzwl 0x14(%r8),%eax
   140002296:	48 29 d1             	sub    %rdx,%rcx
   140002299:	49 8d 44 00 18       	lea    0x18(%r8,%rax,1),%rax
   14000229e:	45 0f b7 40 06       	movzwl 0x6(%r8),%r8d
   1400022a3:	66 45 85 c0          	test   %r8w,%r8w
   1400022a7:	74 34                	je     1400022dd <__mingw_GetSectionForAddress+0x7d>
   1400022a9:	41 8d 50 ff          	lea    -0x1(%r8),%edx
   1400022ad:	48 8d 14 92          	lea    (%rdx,%rdx,4),%rdx
   1400022b1:	4c 8d 4c d0 28       	lea    0x28(%rax,%rdx,8),%r9
   1400022b6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   1400022bd:	00 00 00 
   1400022c0:	44 8b 40 0c          	mov    0xc(%rax),%r8d
   1400022c4:	4c 89 c2             	mov    %r8,%rdx
   1400022c7:	4c 39 c1             	cmp    %r8,%rcx
   1400022ca:	72 08                	jb     1400022d4 <__mingw_GetSectionForAddress+0x74>
   1400022cc:	03 50 08             	add    0x8(%rax),%edx
   1400022cf:	48 39 d1             	cmp    %rdx,%rcx
   1400022d2:	72 ac                	jb     140002280 <__mingw_GetSectionForAddress+0x20>
   1400022d4:	48 83 c0 28          	add    $0x28,%rax
   1400022d8:	4c 39 c8             	cmp    %r9,%rax
   1400022db:	75 e3                	jne    1400022c0 <__mingw_GetSectionForAddress+0x60>
   1400022dd:	31 c0                	xor    %eax,%eax
   1400022df:	c3                   	ret

00000001400022e0 <__mingw_GetSectionCount>:
   1400022e0:	48 8b 05 b9 20 00 00 	mov    0x20b9(%rip),%rax        # 1400043a0 <.refptr.__ImageBase>
   1400022e7:	31 c9                	xor    %ecx,%ecx
   1400022e9:	66 81 38 4d 5a       	cmpw   $0x5a4d,(%rax)
   1400022ee:	75 0f                	jne    1400022ff <__mingw_GetSectionCount+0x1f>
   1400022f0:	48 63 50 3c          	movslq 0x3c(%rax),%rdx
   1400022f4:	48 01 d0             	add    %rdx,%rax
   1400022f7:	81 38 50 45 00 00    	cmpl   $0x4550,(%rax)
   1400022fd:	74 09                	je     140002308 <__mingw_GetSectionCount+0x28>
   1400022ff:	89 c8                	mov    %ecx,%eax
   140002301:	c3                   	ret
   140002302:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
   140002308:	66 81 78 18 0b 02    	cmpw   $0x20b,0x18(%rax)
   14000230e:	75 ef                	jne    1400022ff <__mingw_GetSectionCount+0x1f>
   140002310:	0f b7 48 06          	movzwl 0x6(%rax),%ecx
   140002314:	89 c8                	mov    %ecx,%eax
   140002316:	c3                   	ret
   140002317:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
   14000231e:	00 00 

0000000140002320 <_FindPESectionExec>:
   140002320:	4c 8b 05 79 20 00 00 	mov    0x2079(%rip),%r8        # 1400043a0 <.refptr.__ImageBase>
   140002327:	31 c0                	xor    %eax,%eax
   140002329:	66 41 81 38 4d 5a    	cmpw   $0x5a4d,(%r8)
   14000232f:	75 0f                	jne    140002340 <_FindPESectionExec+0x20>
   140002331:	49 63 50 3c          	movslq 0x3c(%r8),%rdx
   140002335:	4c 01 c2             	add    %r8,%rdx
   140002338:	81 3a 50 45 00 00    	cmpl   $0x4550,(%rdx)
   14000233e:	74 08                	je     140002348 <_FindPESectionExec+0x28>
   140002340:	c3                   	ret
   140002341:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140002348:	66 81 7a 18 0b 02    	cmpw   $0x20b,0x18(%rdx)
   14000234e:	75 f0                	jne    140002340 <_FindPESectionExec+0x20>
   140002350:	0f b7 42 14          	movzwl 0x14(%rdx),%eax
   140002354:	44 0f b7 42 06       	movzwl 0x6(%rdx),%r8d
   140002359:	48 8d 44 02 18       	lea    0x18(%rdx,%rax,1),%rax
   14000235e:	66 45 85 c0          	test   %r8w,%r8w
   140002362:	74 2c                	je     140002390 <_FindPESectionExec+0x70>
   140002364:	41 8d 50 ff          	lea    -0x1(%r8),%edx
   140002368:	48 8d 14 92          	lea    (%rdx,%rdx,4),%rdx
   14000236c:	48 8d 54 d0 28       	lea    0x28(%rax,%rdx,8),%rdx
   140002371:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140002378:	f6 40 27 20          	testb  $0x20,0x27(%rax)
   14000237c:	74 09                	je     140002387 <_FindPESectionExec+0x67>
   14000237e:	48 85 c9             	test   %rcx,%rcx
   140002381:	74 bd                	je     140002340 <_FindPESectionExec+0x20>
   140002383:	48 83 e9 01          	sub    $0x1,%rcx
   140002387:	48 83 c0 28          	add    $0x28,%rax
   14000238b:	48 39 c2             	cmp    %rax,%rdx
   14000238e:	75 e8                	jne    140002378 <_FindPESectionExec+0x58>
   140002390:	31 c0                	xor    %eax,%eax
   140002392:	c3                   	ret
   140002393:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
   14000239a:	00 00 00 00 
   14000239e:	66 90                	xchg   %ax,%ax

00000001400023a0 <_GetPEImageBase>:
   1400023a0:	48 8b 05 f9 1f 00 00 	mov    0x1ff9(%rip),%rax        # 1400043a0 <.refptr.__ImageBase>
   1400023a7:	31 d2                	xor    %edx,%edx
   1400023a9:	66 81 38 4d 5a       	cmpw   $0x5a4d,(%rax)
   1400023ae:	75 0f                	jne    1400023bf <_GetPEImageBase+0x1f>
   1400023b0:	48 63 48 3c          	movslq 0x3c(%rax),%rcx
   1400023b4:	48 01 c1             	add    %rax,%rcx
   1400023b7:	81 39 50 45 00 00    	cmpl   $0x4550,(%rcx)
   1400023bd:	74 09                	je     1400023c8 <_GetPEImageBase+0x28>
   1400023bf:	48 89 d0             	mov    %rdx,%rax
   1400023c2:	c3                   	ret
   1400023c3:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
   1400023c8:	66 81 79 18 0b 02    	cmpw   $0x20b,0x18(%rcx)
   1400023ce:	48 0f 44 d0          	cmove  %rax,%rdx
   1400023d2:	48 89 d0             	mov    %rdx,%rax
   1400023d5:	c3                   	ret
   1400023d6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   1400023dd:	00 00 00 

00000001400023e0 <_IsNonwritableInCurrentImage>:
   1400023e0:	48 8b 15 b9 1f 00 00 	mov    0x1fb9(%rip),%rdx        # 1400043a0 <.refptr.__ImageBase>
   1400023e7:	31 c0                	xor    %eax,%eax
   1400023e9:	66 81 3a 4d 5a       	cmpw   $0x5a4d,(%rdx)
   1400023ee:	75 10                	jne    140002400 <_IsNonwritableInCurrentImage+0x20>
   1400023f0:	4c 63 42 3c          	movslq 0x3c(%rdx),%r8
   1400023f4:	49 01 d0             	add    %rdx,%r8
   1400023f7:	41 81 38 50 45 00 00 	cmpl   $0x4550,(%r8)
   1400023fe:	74 08                	je     140002408 <_IsNonwritableInCurrentImage+0x28>
   140002400:	c3                   	ret
   140002401:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
   140002408:	66 41 81 78 18 0b 02 	cmpw   $0x20b,0x18(%r8)
   14000240f:	75 ef                	jne    140002400 <_IsNonwritableInCurrentImage+0x20>
   140002411:	48 29 d1             	sub    %rdx,%rcx
   140002414:	45 0f b7 48 06       	movzwl 0x6(%r8),%r9d
   140002419:	41 0f b7 50 14       	movzwl 0x14(%r8),%edx
   14000241e:	49 8d 54 10 18       	lea    0x18(%r8,%rdx,1),%rdx
   140002423:	66 45 85 c9          	test   %r9w,%r9w
   140002427:	74 d7                	je     140002400 <_IsNonwritableInCurrentImage+0x20>
   140002429:	41 8d 41 ff          	lea    -0x1(%r9),%eax
   14000242d:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
   140002431:	4c 8d 4c c2 28       	lea    0x28(%rdx,%rax,8),%r9
   140002436:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   14000243d:	00 00 00 
   140002440:	44 8b 42 0c          	mov    0xc(%rdx),%r8d
   140002444:	4c 89 c0             	mov    %r8,%rax
   140002447:	4c 39 c1             	cmp    %r8,%rcx
   14000244a:	72 08                	jb     140002454 <_IsNonwritableInCurrentImage+0x74>
   14000244c:	03 42 08             	add    0x8(%rdx),%eax
   14000244f:	48 39 c1             	cmp    %rax,%rcx
   140002452:	72 0c                	jb     140002460 <_IsNonwritableInCurrentImage+0x80>
   140002454:	48 83 c2 28          	add    $0x28,%rdx
   140002458:	4c 39 ca             	cmp    %r9,%rdx
   14000245b:	75 e3                	jne    140002440 <_IsNonwritableInCurrentImage+0x60>
   14000245d:	31 c0                	xor    %eax,%eax
   14000245f:	c3                   	ret
   140002460:	8b 42 24             	mov    0x24(%rdx),%eax
   140002463:	f7 d0                	not    %eax
   140002465:	c1 e8 1f             	shr    $0x1f,%eax
   140002468:	c3                   	ret
   140002469:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000140002470 <__mingw_enum_import_library_names>:
   140002470:	4c 8b 1d 29 1f 00 00 	mov    0x1f29(%rip),%r11        # 1400043a0 <.refptr.__ImageBase>
   140002477:	45 31 c9             	xor    %r9d,%r9d
   14000247a:	66 41 81 3b 4d 5a    	cmpw   $0x5a4d,(%r11)
   140002480:	75 10                	jne    140002492 <__mingw_enum_import_library_names+0x22>
   140002482:	4d 63 43 3c          	movslq 0x3c(%r11),%r8
   140002486:	4d 01 d8             	add    %r11,%r8
   140002489:	41 81 38 50 45 00 00 	cmpl   $0x4550,(%r8)
   140002490:	74 0e                	je     1400024a0 <__mingw_enum_import_library_names+0x30>
   140002492:	4c 89 c8             	mov    %r9,%rax
   140002495:	c3                   	ret
   140002496:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   14000249d:	00 00 00 
   1400024a0:	66 41 81 78 18 0b 02 	cmpw   $0x20b,0x18(%r8)
   1400024a7:	75 e9                	jne    140002492 <__mingw_enum_import_library_names+0x22>
   1400024a9:	41 8b 80 90 00 00 00 	mov    0x90(%r8),%eax
   1400024b0:	85 c0                	test   %eax,%eax
   1400024b2:	74 de                	je     140002492 <__mingw_enum_import_library_names+0x22>
   1400024b4:	41 0f b7 50 14       	movzwl 0x14(%r8),%edx
   1400024b9:	45 0f b7 50 06       	movzwl 0x6(%r8),%r10d
   1400024be:	49 8d 54 10 18       	lea    0x18(%r8,%rdx,1),%rdx
   1400024c3:	66 45 85 d2          	test   %r10w,%r10w
   1400024c7:	74 c9                	je     140002492 <__mingw_enum_import_library_names+0x22>
   1400024c9:	45 8d 42 ff          	lea    -0x1(%r10),%r8d
   1400024cd:	4f 8d 04 80          	lea    (%r8,%r8,4),%r8
   1400024d1:	4e 8d 54 c2 28       	lea    0x28(%rdx,%r8,8),%r10
   1400024d6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   1400024dd:	00 00 00 
   1400024e0:	44 8b 4a 0c          	mov    0xc(%rdx),%r9d
   1400024e4:	4d 89 c8             	mov    %r9,%r8
   1400024e7:	4c 39 c8             	cmp    %r9,%rax
   1400024ea:	72 09                	jb     1400024f5 <__mingw_enum_import_library_names+0x85>
   1400024ec:	44 03 42 08          	add    0x8(%rdx),%r8d
   1400024f0:	4c 39 c0             	cmp    %r8,%rax
   1400024f3:	72 13                	jb     140002508 <__mingw_enum_import_library_names+0x98>
   1400024f5:	48 83 c2 28          	add    $0x28,%rdx
   1400024f9:	49 39 d2             	cmp    %rdx,%r10
   1400024fc:	75 e2                	jne    1400024e0 <__mingw_enum_import_library_names+0x70>
   1400024fe:	45 31 c9             	xor    %r9d,%r9d
   140002501:	4c 89 c8             	mov    %r9,%rax
   140002504:	c3                   	ret
   140002505:	0f 1f 00             	nopl   (%rax)
   140002508:	4c 01 d8             	add    %r11,%rax
   14000250b:	eb 0a                	jmp    140002517 <__mingw_enum_import_library_names+0xa7>
   14000250d:	0f 1f 00             	nopl   (%rax)
   140002510:	83 e9 01             	sub    $0x1,%ecx
   140002513:	48 83 c0 14          	add    $0x14,%rax
   140002517:	44 8b 40 04          	mov    0x4(%rax),%r8d
   14000251b:	45 85 c0             	test   %r8d,%r8d
   14000251e:	75 07                	jne    140002527 <__mingw_enum_import_library_names+0xb7>
   140002520:	8b 50 0c             	mov    0xc(%rax),%edx
   140002523:	85 d2                	test   %edx,%edx
   140002525:	74 d7                	je     1400024fe <__mingw_enum_import_library_names+0x8e>
   140002527:	85 c9                	test   %ecx,%ecx
   140002529:	7f e5                	jg     140002510 <__mingw_enum_import_library_names+0xa0>
   14000252b:	44 8b 48 0c          	mov    0xc(%rax),%r9d
   14000252f:	4d 01 d9             	add    %r11,%r9
   140002532:	4c 89 c8             	mov    %r9,%rax
   140002535:	c3                   	ret
   140002536:	90                   	nop
   140002537:	90                   	nop
   140002538:	90                   	nop
   140002539:	90                   	nop
   14000253a:	90                   	nop
   14000253b:	90                   	nop
   14000253c:	90                   	nop
   14000253d:	90                   	nop
   14000253e:	90                   	nop
   14000253f:	90                   	nop

0000000140002540 <___chkstk_ms>:
   140002540:	51                   	push   %rcx
   140002541:	50                   	push   %rax
   140002542:	48 3d 00 10 00 00    	cmp    $0x1000,%rax
   140002548:	48 8d 4c 24 18       	lea    0x18(%rsp),%rcx
   14000254d:	72 19                	jb     140002568 <___chkstk_ms+0x28>
   14000254f:	48 81 e9 00 10 00 00 	sub    $0x1000,%rcx
   140002556:	48 83 09 00          	orq    $0x0,(%rcx)
   14000255a:	48 2d 00 10 00 00    	sub    $0x1000,%rax
   140002560:	48 3d 00 10 00 00    	cmp    $0x1000,%rax
   140002566:	77 e7                	ja     14000254f <___chkstk_ms+0xf>
   140002568:	48 29 c1             	sub    %rax,%rcx
   14000256b:	48 83 09 00          	orq    $0x0,(%rcx)
   14000256f:	58                   	pop    %rax
   140002570:	59                   	pop    %rcx
   140002571:	c3                   	ret
   140002572:	90                   	nop
   140002573:	90                   	nop
   140002574:	90                   	nop
   140002575:	90                   	nop
   140002576:	90                   	nop
   140002577:	90                   	nop
   140002578:	90                   	nop
   140002579:	90                   	nop
   14000257a:	90                   	nop
   14000257b:	90                   	nop
   14000257c:	90                   	nop
   14000257d:	90                   	nop
   14000257e:	90                   	nop
   14000257f:	90                   	nop

0000000140002580 <vfprintf>:
   140002580:	48 83 ec 38          	sub    $0x38,%rsp
   140002584:	45 31 c9             	xor    %r9d,%r9d
   140002587:	4c 89 44 24 20       	mov    %r8,0x20(%rsp)
   14000258c:	49 89 d0             	mov    %rdx,%r8
   14000258f:	48 89 ca             	mov    %rcx,%rdx
   140002592:	31 c9                	xor    %ecx,%ecx
   140002594:	e8 ff 02 00 00       	call   140002898 <__stdio_common_vfprintf>
   140002599:	48 83 c4 38          	add    $0x38,%rsp
   14000259d:	c3                   	ret
   14000259e:	90                   	nop
   14000259f:	90                   	nop

00000001400025a0 <printf>:
   1400025a0:	56                   	push   %rsi
   1400025a1:	53                   	push   %rbx
   1400025a2:	48 83 ec 48          	sub    $0x48,%rsp
   1400025a6:	48 89 cb             	mov    %rcx,%rbx
   1400025a9:	48 8d 74 24 68       	lea    0x68(%rsp),%rsi
   1400025ae:	b9 01 00 00 00       	mov    $0x1,%ecx
   1400025b3:	48 89 54 24 68       	mov    %rdx,0x68(%rsp)
   1400025b8:	4c 89 44 24 70       	mov    %r8,0x70(%rsp)
   1400025bd:	4c 89 4c 24 78       	mov    %r9,0x78(%rsp)
   1400025c2:	48 89 74 24 38       	mov    %rsi,0x38(%rsp)
   1400025c7:	e8 b4 02 00 00       	call   140002880 <__acrt_iob_func>
   1400025cc:	48 89 74 24 20       	mov    %rsi,0x20(%rsp)
   1400025d1:	45 31 c9             	xor    %r9d,%r9d
   1400025d4:	49 89 d8             	mov    %rbx,%r8
   1400025d7:	48 89 c2             	mov    %rax,%rdx
   1400025da:	31 c9                	xor    %ecx,%ecx
   1400025dc:	e8 b7 02 00 00       	call   140002898 <__stdio_common_vfprintf>
   1400025e1:	48 83 c4 48          	add    $0x48,%rsp
   1400025e5:	5b                   	pop    %rbx
   1400025e6:	5e                   	pop    %rsi
   1400025e7:	c3                   	ret
   1400025e8:	90                   	nop
   1400025e9:	90                   	nop
   1400025ea:	90                   	nop
   1400025eb:	90                   	nop
   1400025ec:	90                   	nop
   1400025ed:	90                   	nop
   1400025ee:	90                   	nop
   1400025ef:	90                   	nop

00000001400025f0 <fprintf>:
   1400025f0:	48 83 ec 48          	sub    $0x48,%rsp
   1400025f4:	48 8d 44 24 60       	lea    0x60(%rsp),%rax
   1400025f9:	4c 89 44 24 60       	mov    %r8,0x60(%rsp)
   1400025fe:	49 89 d0             	mov    %rdx,%r8
   140002601:	48 89 ca             	mov    %rcx,%rdx
   140002604:	48 89 44 24 20       	mov    %rax,0x20(%rsp)
   140002609:	31 c9                	xor    %ecx,%ecx
   14000260b:	4c 89 4c 24 68       	mov    %r9,0x68(%rsp)
   140002610:	45 31 c9             	xor    %r9d,%r9d
   140002613:	48 89 44 24 38       	mov    %rax,0x38(%rsp)
   140002618:	e8 7b 02 00 00       	call   140002898 <__stdio_common_vfprintf>
   14000261d:	48 83 c4 48          	add    $0x48,%rsp
   140002621:	c3                   	ret
   140002622:	90                   	nop
   140002623:	90                   	nop
   140002624:	90                   	nop
   140002625:	90                   	nop
   140002626:	90                   	nop
   140002627:	90                   	nop
   140002628:	90                   	nop
   140002629:	90                   	nop
   14000262a:	90                   	nop
   14000262b:	90                   	nop
   14000262c:	90                   	nop
   14000262d:	90                   	nop
   14000262e:	90                   	nop
   14000262f:	90                   	nop

0000000140002630 <_get_output_format>:
   140002630:	31 c0                	xor    %eax,%eax
   140002632:	c3                   	ret
   140002633:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
   14000263a:	00 00 00 00 
   14000263e:	66 90                	xchg   %ax,%ax

0000000140002640 <__getmainargs>:
   140002640:	41 54                	push   %r12
   140002642:	55                   	push   %rbp
   140002643:	57                   	push   %rdi
   140002644:	56                   	push   %rsi
   140002645:	53                   	push   %rbx
   140002646:	48 83 ec 20          	sub    $0x20,%rsp
   14000264a:	4c 8b 64 24 70       	mov    0x70(%rsp),%r12
   14000264f:	44 89 cd             	mov    %r9d,%ebp
   140002652:	48 89 d6             	mov    %rdx,%rsi
   140002655:	4c 89 c3             	mov    %r8,%rbx
   140002658:	48 89 cf             	mov    %rcx,%rdi
   14000265b:	e8 98 02 00 00       	call   1400028f8 <_initialize_narrow_environment>
   140002660:	83 fd 01             	cmp    $0x1,%ebp
   140002663:	b9 01 00 00 00       	mov    $0x1,%ecx
   140002668:	83 d9 ff             	sbb    $0xffffffff,%ecx
   14000266b:	e8 60 02 00 00       	call   1400028d0 <_configure_narrow_argv>
   140002670:	e8 3b 02 00 00       	call   1400028b0 <__p___argc>
   140002675:	8b 00                	mov    (%rax),%eax
   140002677:	89 07                	mov    %eax,(%rdi)
   140002679:	e8 3a 02 00 00       	call   1400028b8 <__p___argv>
   14000267e:	48 8b 00             	mov    (%rax),%rax
   140002681:	48 89 06             	mov    %rax,(%rsi)
   140002684:	e8 f7 02 00 00       	call   140002980 <__p__environ>
   140002689:	48 8b 00             	mov    (%rax),%rax
   14000268c:	48 89 03             	mov    %rax,(%rbx)
   14000268f:	4d 85 e4             	test   %r12,%r12
   140002692:	74 09                	je     14000269d <__getmainargs+0x5d>
   140002694:	41 8b 0c 24          	mov    (%r12),%ecx
   140002698:	e8 c3 02 00 00       	call   140002960 <_set_new_mode>
   14000269d:	31 c0                	xor    %eax,%eax
   14000269f:	48 83 c4 20          	add    $0x20,%rsp
   1400026a3:	5b                   	pop    %rbx
   1400026a4:	5e                   	pop    %rsi
   1400026a5:	5f                   	pop    %rdi
   1400026a6:	5d                   	pop    %rbp
   1400026a7:	41 5c                	pop    %r12
   1400026a9:	c3                   	ret
   1400026aa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000001400026b0 <__wgetmainargs>:
   1400026b0:	41 54                	push   %r12
   1400026b2:	55                   	push   %rbp
   1400026b3:	57                   	push   %rdi
   1400026b4:	56                   	push   %rsi
   1400026b5:	53                   	push   %rbx
   1400026b6:	48 83 ec 20          	sub    $0x20,%rsp
   1400026ba:	4c 8b 64 24 70       	mov    0x70(%rsp),%r12
   1400026bf:	44 89 cd             	mov    %r9d,%ebp
   1400026c2:	48 89 d6             	mov    %rdx,%rsi
   1400026c5:	4c 89 c3             	mov    %r8,%rbx
   1400026c8:	48 89 cf             	mov    %rcx,%rdi
   1400026cb:	e8 30 02 00 00       	call   140002900 <_initialize_wide_environment>
   1400026d0:	83 fd 01             	cmp    $0x1,%ebp
   1400026d3:	b9 01 00 00 00       	mov    $0x1,%ecx
   1400026d8:	83 d9 ff             	sbb    $0xffffffff,%ecx
   1400026db:	e8 f8 01 00 00       	call   1400028d8 <_configure_wide_argv>
   1400026e0:	e8 cb 01 00 00       	call   1400028b0 <__p___argc>
   1400026e5:	8b 00                	mov    (%rax),%eax
   1400026e7:	89 07                	mov    %eax,(%rdi)
   1400026e9:	e8 d2 01 00 00       	call   1400028c0 <__p___wargv>
   1400026ee:	48 8b 00             	mov    (%rax),%rax
   1400026f1:	48 89 06             	mov    %rax,(%rsi)
   1400026f4:	e8 8f 02 00 00       	call   140002988 <__p__wenviron>
   1400026f9:	48 8b 00             	mov    (%rax),%rax
   1400026fc:	48 89 03             	mov    %rax,(%rbx)
   1400026ff:	4d 85 e4             	test   %r12,%r12
   140002702:	74 09                	je     14000270d <__wgetmainargs+0x5d>
   140002704:	41 8b 0c 24          	mov    (%r12),%ecx
   140002708:	e8 53 02 00 00       	call   140002960 <_set_new_mode>
   14000270d:	31 c0                	xor    %eax,%eax
   14000270f:	48 83 c4 20          	add    $0x20,%rsp
   140002713:	5b                   	pop    %rbx
   140002714:	5e                   	pop    %rsi
   140002715:	5f                   	pop    %rdi
   140002716:	5d                   	pop    %rbp
   140002717:	41 5c                	pop    %r12
   140002719:	c3                   	ret
   14000271a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000140002720 <_onexit>:
   140002720:	53                   	push   %rbx
   140002721:	48 83 ec 20          	sub    $0x20,%rsp
   140002725:	48 89 cb             	mov    %rcx,%rbx
   140002728:	e8 bb 01 00 00       	call   1400028e8 <_crt_atexit>
   14000272d:	85 c0                	test   %eax,%eax
   14000272f:	b8 00 00 00 00       	mov    $0x0,%eax
   140002734:	48 0f 44 c3          	cmove  %rbx,%rax
   140002738:	48 83 c4 20          	add    $0x20,%rsp
   14000273c:	5b                   	pop    %rbx
   14000273d:	c3                   	ret
   14000273e:	66 90                	xchg   %ax,%ax

0000000140002740 <at_quick_exit>:
   140002740:	48 8b 05 f9 1c 00 00 	mov    0x1cf9(%rip),%rax        # 140004440 <.refptr.__mingw_module_is_dll>
   140002747:	80 38 00             	cmpb   $0x0,(%rax)
   14000274a:	74 04                	je     140002750 <at_quick_exit+0x10>
   14000274c:	31 c0                	xor    %eax,%eax
   14000274e:	c3                   	ret
   14000274f:	90                   	nop
   140002750:	e9 8b 01 00 00       	jmp    1400028e0 <_crt_at_quick_exit>
   140002755:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
   14000275c:	00 00 00 00 

0000000140002760 <_amsg_exit>:
   140002760:	53                   	push   %rbx
   140002761:	48 83 ec 20          	sub    $0x20,%rsp
   140002765:	89 cb                	mov    %ecx,%ebx
   140002767:	b9 02 00 00 00       	mov    $0x2,%ecx
   14000276c:	e8 0f 01 00 00       	call   140002880 <__acrt_iob_func>
   140002771:	41 89 d8             	mov    %ebx,%r8d
   140002774:	48 8d 15 d5 1b 00 00 	lea    0x1bd5(%rip),%rdx        # 140004350 <.rdata>
   14000277b:	48 89 c1             	mov    %rax,%rcx
   14000277e:	e8 6d fe ff ff       	call   1400025f0 <fprintf>
   140002783:	b9 ff 00 00 00       	mov    $0xff,%ecx
   140002788:	e8 63 01 00 00       	call   1400028f0 <_exit>
   14000278d:	90                   	nop
   14000278e:	66 90                	xchg   %ax,%ax

0000000140002790 <__ms_fwprintf>:
   140002790:	48 83 ec 48          	sub    $0x48,%rsp
   140002794:	48 8d 44 24 60       	lea    0x60(%rsp),%rax
   140002799:	4c 89 44 24 60       	mov    %r8,0x60(%rsp)
   14000279e:	49 89 d0             	mov    %rdx,%r8
   1400027a1:	48 89 ca             	mov    %rcx,%rdx
   1400027a4:	48 89 44 24 20       	mov    %rax,0x20(%rsp)
   1400027a9:	b9 04 00 00 00       	mov    $0x4,%ecx
   1400027ae:	4c 89 4c 24 68       	mov    %r9,0x68(%rsp)
   1400027b3:	45 31 c9             	xor    %r9d,%r9d
   1400027b6:	48 89 44 24 38       	mov    %rax,0x38(%rsp)
   1400027bb:	e8 e0 00 00 00       	call   1400028a0 <__stdio_common_vfwprintf>
   1400027c0:	48 83 c4 48          	add    $0x48,%rsp
   1400027c4:	c3                   	ret
   1400027c5:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
   1400027cc:	00 00 00 00 

00000001400027d0 <tzset>:
   1400027d0:	48 83 ec 28          	sub    $0x28,%rsp
   1400027d4:	48 8b 05 15 1c 00 00 	mov    0x1c15(%rip),%rax        # 1400043f0 <.refptr.__imp__tzset>
   1400027db:	ff 10                	call   *(%rax)
   1400027dd:	e8 7e 00 00 00       	call   140002860 <__tzname>
   1400027e2:	48 89 05 e7 08 00 00 	mov    %rax,0x8e7(%rip)        # 1400030d0 <__imp_tzname>
   1400027e9:	e8 6a 00 00 00       	call   140002858 <__timezone>
   1400027ee:	48 89 05 d3 08 00 00 	mov    %rax,0x8d3(%rip)        # 1400030c8 <__imp_timezone>
   1400027f5:	e8 56 00 00 00       	call   140002850 <__daylight>
   1400027fa:	48 89 05 bf 08 00 00 	mov    %rax,0x8bf(%rip)        # 1400030c0 <__imp_daylight>
   140002801:	48 83 c4 28          	add    $0x28,%rsp
   140002805:	c3                   	ret
   140002806:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
   14000280d:	00 00 00 

0000000140002810 <_tzset>:
   140002810:	48 83 ec 28          	sub    $0x28,%rsp
   140002814:	48 8b 05 d5 1b 00 00 	mov    0x1bd5(%rip),%rax        # 1400043f0 <.refptr.__imp__tzset>
   14000281b:	ff 10                	call   *(%rax)
   14000281d:	e8 3e 00 00 00       	call   140002860 <__tzname>
   140002822:	48 89 05 a7 08 00 00 	mov    %rax,0x8a7(%rip)        # 1400030d0 <__imp_tzname>
   140002829:	e8 2a 00 00 00       	call   140002858 <__timezone>
   14000282e:	48 89 05 93 08 00 00 	mov    %rax,0x893(%rip)        # 1400030c8 <__imp_timezone>
   140002835:	e8 16 00 00 00       	call   140002850 <__daylight>
   14000283a:	48 89 05 7f 08 00 00 	mov    %rax,0x87f(%rip)        # 1400030c0 <__imp_daylight>
   140002841:	48 83 c4 28          	add    $0x28,%rsp
   140002845:	c3                   	ret
   140002846:	90                   	nop
   140002847:	90                   	nop
   140002848:	90                   	nop
   140002849:	90                   	nop
   14000284a:	90                   	nop
   14000284b:	90                   	nop
   14000284c:	90                   	nop
   14000284d:	90                   	nop
   14000284e:	90                   	nop
   14000284f:	90                   	nop

0000000140002850 <__daylight>:
   140002850:	ff 25 12 5c 00 00    	jmp    *0x5c12(%rip)        # 140008468 <__imp___daylight>
   140002856:	90                   	nop
   140002857:	90                   	nop

0000000140002858 <__timezone>:
   140002858:	ff 25 12 5c 00 00    	jmp    *0x5c12(%rip)        # 140008470 <__imp___timezone>
   14000285e:	90                   	nop
   14000285f:	90                   	nop

0000000140002860 <__tzname>:
   140002860:	ff 25 12 5c 00 00    	jmp    *0x5c12(%rip)        # 140008478 <__imp___tzname>
   140002866:	90                   	nop
   140002867:	90                   	nop

0000000140002868 <.text>:
   140002868:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
   14000286f:	00 

0000000140002870 <strlen>:
   140002870:	ff 25 da 5b 00 00    	jmp    *0x5bda(%rip)        # 140008450 <__imp_strlen>
   140002876:	90                   	nop
   140002877:	90                   	nop

0000000140002878 <strncmp>:
   140002878:	ff 25 da 5b 00 00    	jmp    *0x5bda(%rip)        # 140008458 <__imp_strncmp>
   14000287e:	90                   	nop
   14000287f:	90                   	nop

0000000140002880 <__acrt_iob_func>:
   140002880:	ff 25 92 5b 00 00    	jmp    *0x5b92(%rip)        # 140008418 <__imp___acrt_iob_func>
   140002886:	90                   	nop
   140002887:	90                   	nop

0000000140002888 <__p__commode>:
   140002888:	ff 25 92 5b 00 00    	jmp    *0x5b92(%rip)        # 140008420 <__imp___p__commode>
   14000288e:	90                   	nop
   14000288f:	90                   	nop

0000000140002890 <__p__fmode>:
   140002890:	ff 25 92 5b 00 00    	jmp    *0x5b92(%rip)        # 140008428 <__imp___p__fmode>
   140002896:	90                   	nop
   140002897:	90                   	nop

0000000140002898 <__stdio_common_vfprintf>:
   140002898:	ff 25 92 5b 00 00    	jmp    *0x5b92(%rip)        # 140008430 <__imp___stdio_common_vfprintf>
   14000289e:	90                   	nop
   14000289f:	90                   	nop

00000001400028a0 <__stdio_common_vfwprintf>:
   1400028a0:	ff 25 92 5b 00 00    	jmp    *0x5b92(%rip)        # 140008438 <__imp___stdio_common_vfwprintf>
   1400028a6:	90                   	nop
   1400028a7:	90                   	nop

00000001400028a8 <fwrite>:
   1400028a8:	ff 25 92 5b 00 00    	jmp    *0x5b92(%rip)        # 140008440 <__imp_fwrite>
   1400028ae:	90                   	nop
   1400028af:	90                   	nop

00000001400028b0 <__p___argc>:
   1400028b0:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 140008388 <__imp___p___argc>
   1400028b6:	90                   	nop
   1400028b7:	90                   	nop

00000001400028b8 <__p___argv>:
   1400028b8:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 140008390 <__imp___p___argv>
   1400028be:	90                   	nop
   1400028bf:	90                   	nop

00000001400028c0 <__p___wargv>:
   1400028c0:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 140008398 <__imp___p___wargv>
   1400028c6:	90                   	nop
   1400028c7:	90                   	nop

00000001400028c8 <_cexit>:
   1400028c8:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083a0 <__imp__cexit>
   1400028ce:	90                   	nop
   1400028cf:	90                   	nop

00000001400028d0 <_configure_narrow_argv>:
   1400028d0:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083a8 <__imp__configure_narrow_argv>
   1400028d6:	90                   	nop
   1400028d7:	90                   	nop

00000001400028d8 <_configure_wide_argv>:
   1400028d8:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083b0 <__imp__configure_wide_argv>
   1400028de:	90                   	nop
   1400028df:	90                   	nop

00000001400028e0 <_crt_at_quick_exit>:
   1400028e0:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083b8 <__imp__crt_at_quick_exit>
   1400028e6:	90                   	nop
   1400028e7:	90                   	nop

00000001400028e8 <_crt_atexit>:
   1400028e8:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083c0 <__imp__crt_atexit>
   1400028ee:	90                   	nop
   1400028ef:	90                   	nop

00000001400028f0 <_exit>:
   1400028f0:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083c8 <__imp__exit>
   1400028f6:	90                   	nop
   1400028f7:	90                   	nop

00000001400028f8 <_initialize_narrow_environment>:
   1400028f8:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083d0 <__imp__initialize_narrow_environment>
   1400028fe:	90                   	nop
   1400028ff:	90                   	nop

0000000140002900 <_initialize_wide_environment>:
   140002900:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083d8 <__imp__initialize_wide_environment>
   140002906:	90                   	nop
   140002907:	90                   	nop

0000000140002908 <_initterm>:
   140002908:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083e0 <__imp__initterm>
   14000290e:	90                   	nop
   14000290f:	90                   	nop

0000000140002910 <__set_app_type>:
   140002910:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083e8 <__imp___set_app_type>
   140002916:	90                   	nop
   140002917:	90                   	nop

0000000140002918 <_set_invalid_parameter_handler>:
   140002918:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083f0 <__imp__set_invalid_parameter_handler>
   14000291e:	90                   	nop
   14000291f:	90                   	nop

0000000140002920 <abort>:
   140002920:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 1400083f8 <__imp_abort>
   140002926:	90                   	nop
   140002927:	90                   	nop

0000000140002928 <exit>:
   140002928:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 140008400 <__imp_exit>
   14000292e:	90                   	nop
   14000292f:	90                   	nop

0000000140002930 <signal>:
   140002930:	ff 25 d2 5a 00 00    	jmp    *0x5ad2(%rip)        # 140008408 <__imp_signal>
   140002936:	90                   	nop
   140002937:	90                   	nop
   140002938:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
   14000293f:	00 

0000000140002940 <__C_specific_handler>:
   140002940:	ff 25 2a 5a 00 00    	jmp    *0x5a2a(%rip)        # 140008370 <__imp___C_specific_handler>
   140002946:	90                   	nop
   140002947:	90                   	nop

0000000140002948 <memcpy>:
   140002948:	ff 25 2a 5a 00 00    	jmp    *0x5a2a(%rip)        # 140008378 <__imp_memcpy>
   14000294e:	90                   	nop
   14000294f:	90                   	nop

0000000140002950 <__setusermatherr>:
   140002950:	ff 25 0a 5a 00 00    	jmp    *0x5a0a(%rip)        # 140008360 <__imp___setusermatherr>
   140002956:	90                   	nop
   140002957:	90                   	nop
   140002958:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
   14000295f:	00 

0000000140002960 <_set_new_mode>:
   140002960:	ff 25 d2 59 00 00    	jmp    *0x59d2(%rip)        # 140008338 <__imp__set_new_mode>
   140002966:	90                   	nop
   140002967:	90                   	nop

0000000140002968 <calloc>:
   140002968:	ff 25 d2 59 00 00    	jmp    *0x59d2(%rip)        # 140008340 <__imp_calloc>
   14000296e:	90                   	nop
   14000296f:	90                   	nop

0000000140002970 <free>:
   140002970:	ff 25 d2 59 00 00    	jmp    *0x59d2(%rip)        # 140008348 <__imp_free>
   140002976:	90                   	nop
   140002977:	90                   	nop

0000000140002978 <malloc>:
   140002978:	ff 25 d2 59 00 00    	jmp    *0x59d2(%rip)        # 140008350 <__imp_malloc>
   14000297e:	90                   	nop
   14000297f:	90                   	nop

0000000140002980 <__p__environ>:
   140002980:	ff 25 9a 59 00 00    	jmp    *0x599a(%rip)        # 140008320 <__imp___p__environ>
   140002986:	90                   	nop
   140002987:	90                   	nop

0000000140002988 <__p__wenviron>:
   140002988:	ff 25 9a 59 00 00    	jmp    *0x599a(%rip)        # 140008328 <__imp___p__wenviron>
   14000298e:	90                   	nop
   14000298f:	90                   	nop

0000000140002990 <atoi>:
   140002990:	ff 25 7a 59 00 00    	jmp    *0x597a(%rip)        # 140008310 <__imp_atoi>
   140002996:	90                   	nop
   140002997:	90                   	nop
   140002998:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
   14000299f:	00 

00000001400029a0 <VirtualQuery>:
   1400029a0:	ff 25 5a 59 00 00    	jmp    *0x595a(%rip)        # 140008300 <__imp_VirtualQuery>
   1400029a6:	90                   	nop
   1400029a7:	90                   	nop

00000001400029a8 <VirtualProtect>:
   1400029a8:	ff 25 4a 59 00 00    	jmp    *0x594a(%rip)        # 1400082f8 <__imp_VirtualProtect>
   1400029ae:	90                   	nop
   1400029af:	90                   	nop

00000001400029b0 <TlsGetValue>:
   1400029b0:	ff 25 3a 59 00 00    	jmp    *0x593a(%rip)        # 1400082f0 <__imp_TlsGetValue>
   1400029b6:	90                   	nop
   1400029b7:	90                   	nop

00000001400029b8 <Sleep>:
   1400029b8:	ff 25 2a 59 00 00    	jmp    *0x592a(%rip)        # 1400082e8 <__imp_Sleep>
   1400029be:	90                   	nop
   1400029bf:	90                   	nop

00000001400029c0 <SetUnhandledExceptionFilter>:
   1400029c0:	ff 25 1a 59 00 00    	jmp    *0x591a(%rip)        # 1400082e0 <__imp_SetUnhandledExceptionFilter>
   1400029c6:	90                   	nop
   1400029c7:	90                   	nop

00000001400029c8 <LeaveCriticalSection>:
   1400029c8:	ff 25 0a 59 00 00    	jmp    *0x590a(%rip)        # 1400082d8 <__imp_LeaveCriticalSection>
   1400029ce:	90                   	nop
   1400029cf:	90                   	nop

00000001400029d0 <InitializeCriticalSection>:
   1400029d0:	ff 25 fa 58 00 00    	jmp    *0x58fa(%rip)        # 1400082d0 <__imp_InitializeCriticalSection>
   1400029d6:	90                   	nop
   1400029d7:	90                   	nop

00000001400029d8 <GetLastError>:
   1400029d8:	ff 25 ea 58 00 00    	jmp    *0x58ea(%rip)        # 1400082c8 <__imp_GetLastError>
   1400029de:	90                   	nop
   1400029df:	90                   	nop

00000001400029e0 <EnterCriticalSection>:
   1400029e0:	ff 25 da 58 00 00    	jmp    *0x58da(%rip)        # 1400082c0 <__imp_EnterCriticalSection>
   1400029e6:	90                   	nop
   1400029e7:	90                   	nop

00000001400029e8 <DeleteCriticalSection>:
   1400029e8:	ff 25 ca 58 00 00    	jmp    *0x58ca(%rip)        # 1400082b8 <__IAT_start__>
   1400029ee:	90                   	nop
   1400029ef:	90                   	nop

00000001400029f0 <register_frame_ctor>:
   1400029f0:	e9 3b ea ff ff       	jmp    140001430 <__gcc_register_frame>
   1400029f5:	90                   	nop
   1400029f6:	90                   	nop
   1400029f7:	90                   	nop
   1400029f8:	90                   	nop
   1400029f9:	90                   	nop
   1400029fa:	90                   	nop
   1400029fb:	90                   	nop
   1400029fc:	90                   	nop
   1400029fd:	90                   	nop
   1400029fe:	90                   	nop
   1400029ff:	90                   	nop

0000000140002a00 <__CTOR_LIST__>:
   140002a00:	ff                   	(bad)
   140002a01:	ff                   	(bad)
   140002a02:	ff                   	(bad)
   140002a03:	ff                   	(bad)
   140002a04:	ff                   	(bad)
   140002a05:	ff                   	(bad)
   140002a06:	ff                   	(bad)
   140002a07:	ff                   	.byte 0xff

0000000140002a08 <.ctors.65535>:
   140002a08:	f0 29 00             	lock sub %eax,(%rax)
   140002a0b:	40 01 00             	rex add %eax,(%rax)
	...

0000000140002a18 <__DTOR_LIST__>:
   140002a18:	ff                   	(bad)
   140002a19:	ff                   	(bad)
   140002a1a:	ff                   	(bad)
   140002a1b:	ff                   	(bad)
   140002a1c:	ff                   	(bad)
   140002a1d:	ff                   	(bad)
   140002a1e:	ff                   	(bad)
   140002a1f:	ff 00                	incl   (%rax)
   140002a21:	00 00                	add    %al,(%rax)
   140002a23:	00 00                	add    %al,(%rax)
   140002a25:	00 00                	add    %al,(%rax)
	...
