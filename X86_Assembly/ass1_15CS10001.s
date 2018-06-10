	.file	"test.c"                                # name of the .c file

	.text                                           # This is the starting point of the C code

	.globl	func                                    # func is a global name. Global because it can be used anywhere in the C code

	.type	func, @function                         # func is the name of a function in the C code.

func:                                               # definition of function func starts here

.LFB0:                                              # local label 
													# all assembly code in this label represents a fragment of the C code, possibly the 
													# function func    
	
	.cfi_startproc									# cfi - call frame information
													# startproc means this is the start of a function indicated by LFB0 i.e. func
	
	pushq	%rbp                                    # Save old base pointer (rbp is a register for callee save)
													# rbp stores the pointer which points to the base of the stack. Stack stores many 
													# variables that are used in the C code.
 
	
	.cfi_def_cfa_offset 16							# This tells that the call frame address is at an offset of 16 from the current stack 
													# pointer in the stack. 
													# Call Frame Address is the address of the function that called the current function.
	
	.cfi_offset 6, -16								# Value of register 6 i.e. %rbp (the base pointer) is stored in the stack at an offset 
													# of 16 from the CFA.
	
	movq	%rsp, %rbp								# rbp <-- rsp (save hardware stack pointer in rbp)
													# rsp may change to allocate space for local variables or arrays in the C program. 
													# So this instruction saves the value of stack pointer in rbp forlater use such as
													# referring to some variable stored in the stack .
	
	.cfi_def_cfa_register 6							# This means that from now on register 6 i.e. rbp will be used to compute CFA. Offset 
													# remains unchanged. Before this rsp was being used to compute CFA , but value of rsp
													# may change to allocate space for local variables. That is why rbp is being used to 
													# compute CFA now.
	
	pushq	%rbx									# Push the value stored in rbx into the stack. rbx is a register for callee save. It can 
													# be used to store temporary variables during assembly computations.
	
	subq	$56, %rsp                               # rsp <-- rsp - 56 (allocate memory for variables in the C program)
	
	.cfi_offset 3, -24								# Value of register 3 is stored in the stack at an offset of 16 from the CFA.
	
	movl	%edi, -52(%rbp)							# Mem[rbp - 52] <-- edi (value of 1st argument of the function is saved at location 
													# rbp-52 in stack). Let's assume that Mem[rbp - 52] represents a variable x in the 
													# program. So x = 9. 
	
	movq	%fs:40, %rax                            # rax <-- fs:40         (A __thread type variable declared)
	
	movq	%rax, -24(%rbp)                         # Mem[rbp - 24] <-- rax implies Mem[rbp - 24] = fs:40  (__thread type variable)
	
	xorl	%eax, %eax								# eax <-- eax ^ eax  (32-bit XOR operation). Implies that eax = 0.
	
	movq	%rsp, %rax                              # rax <-- rsp (save hardware stack pointer)
	
	movq	%rax, %rsi                              # rsi <-- rax (stack pointer is saved in register rsi, probably to retrieve this pointer 
													# later.
	
	movl	-52(%rbp), %eax                         # eax <-- Mem[rbp - 52]       (eax = x = 9). 
	
	addl	$1, %eax                                # eax <-- eax + 1          (eax = 10).
	
	movslq	%eax, %rdx                              # sign extension of 32-bit eax to 64-bit rdx       (rdx = 10).
	
	subq	$1, %rdx                                # rdx <-- rdx - 1           (rdx = 9).
	
	movq	%rdx, -40(%rbp)                         # Mem[rbp-40] <-- rdx       (Mem[rbp-40] = 9). 
	
	movslq	%eax, %rdx                              # sign extension of 32-bit eax to 64-bit rdx        (rdx = 10).
	
	movq	%rdx, %r8                               # r8 <-- rdx      (r8 = 10).
	
	movl	$0, %r9d                                # r9d <-- 0      
	
	movslq	%eax, %rdx								# sign extension of 32-bit eax to 64-bit rdx        (rdx = 10).
	
	movq	%rdx, %rcx                              # rcx <-- rdx (assign a value to 4th argument)      (rcx = 10).
	
	movl	$0, %ebx                                # value at ebx = 0;
	
	cltq                                            # sign extension of 32-bit eax to 64-bit rax        (rax = 10).
	
	salq	$2, %rax                                # shift value at rax to left by 2-bits         (rax <-- 4*rax so rax = 40).
	
	leaq	3(%rax), %rdx                           # rdx <-- rax + 3        (rdx = 43).  
	
	movl	$16, %eax                               # eax <-- 16             (hence rax is also 16).
	
	subq	$1, %rax                                # rax <-- rax - 1 		 (rax = 15).
	
	addq	%rdx, %rax                              # rax <-- rax + rdx		 (rax = 58).
	
	movl	$16, %edi                               # edi <-- 16 			 (Hence rdi is also 16).
	
	movl	$0, %edx                                # edx <-- 0				 (Hence rdx is also 0).
	
	divq	%rdi                                    # rax = (rdx:rax)/rdi  and  rdx = (rdx:rax)%rdi   (rax = 3  and  rdx = 10) 
													# rdx:rax represents the 128 bit value obtained by merging the values of rdx and rax
	
	imulq	$16, %rax, %rax 						# rax <-- rax * 16       (rax = 3*16 = 48).
	
	subq	%rax, %rsp								# rsp <-- rsp - rax      (rsp = rsp - 48  i.e.  rsp = rbp - 56 - 48 = rbp - 104) 
													# allocate space for variables in the code, probably an array of size 12.
													# Let's call this array 'arr'.
	
	movq	%rsp, %rax                              # rax <-- rsp            (rax = rsp  i.e.  rax = rbp - 104) save stack pointer.
													# rax stores the pointer of Mem[rbp - 104] i.e. rax = arr.
	
	addq	$3, %rax                                # rax <-- rax + 3  		 (rax = rbp - 101).
	
	shrq	$2, %rax                                # rax <-- rax/4			 (logical right shift by two bits).
	
	salq	$2, %rax                                # rax <-- 4*rax          (shift value at rax to left by 2-bits).
													# rax <-- rbp-104 to rbp-101    (2 bits were lost due to right shift, hence error of 3)  
	
	movq	%rax, -32(%rbp)                         # rbp - 32 <-- rax       (rbp - 32 stores the pointer of Mem[rbp-104] i.e. arr).
	
	movq	-32(%rbp), %rax                         # rax <-- Mem[rbp - 32]  (rax = rbp - 104).
	
	movl	$0, (%rax)                              # *rax <-- 0             (Mem[rbp-104] = 0 implies arr[0] = 0).
	
	movq	-32(%rbp), %rax                         # rax <-- rbp - 32		 (rax = rbp - 104).
	
	movl	$1, 4(%rax)                             # *(rax + 4) <-- 1		 (Mem[rbp-100] = 1, let's say arr[1] = 1).
	
	movl	$2, -44(%rbp)                           # rbp - 44 <-- 2		 (Mem[rbp-44] = 2).
													# let's say that Mem[rbp-44] represents a variable 'i' in the program. So i = 2.
	
	jmp	.L2                                    		# jump to section L2;

.L3:												# local label 
													# all assembly code in this label represents a fragment of the C code
	
	movl	-44(%rbp), %eax							# eax = i      (Hence rax = i).
	
	leal	-1(%rax), %edx							# edx = i-1
	
	movq	-32(%rbp), %rax							# rax = Mem[rbp-32] = rbp - 104 = pointer to arr.
	
	movslq	%edx, %rdx								# rdx = i-1
	
	movl	(%rax,%rdx,4), %ecx						# ecx = Mem[rax + 4*rdx] which means that ecx = Mem[rbp-104 + 4*(i-1)].
													# rbp-104 is index 0 of array arr. So ecx = arr[i-1].
	
	movl	-44(%rbp), %eax							# eax = i      (Hence rax = i).
	
	leal	-2(%rax), %edx							# edx = i-2
	
	movq	-32(%rbp), %rax							# rax = Mem[rbp-32] = rbp - 104
	
	movslq	%edx, %rdx								# rdx = i-2
	
	movl	(%rax,%rdx,4), %eax						# eax = Mem[rax + 4*rdx] which means that eax = Mem[rbp-104 + 4*(i-2)]
													# rbp-104 is index 0 of array arr. So eax = arr[i-2].
	
	addl	%eax, %ecx								# ecx <-- ecx + eax     (ecx = arr[i-1] + arr[i-2])
	
	movq	-32(%rbp), %rax							# rax = Mem[rbp-32] = rbp - 104 = pointer to arr.
	
	movl	-44(%rbp), %edx							# edx = i
	
	movslq	%edx, %rdx								# rdx = i
	
	movl	%ecx, (%rax,%rdx,4)						# Mem[rax + 4*rdx] = ecx which means that Mem[rbp-104 + 4*i] = ecx
													# rbp-104 is index 0 of array arr. So arr[i] = ecx.
													# Eventually arr[i] = arr[i-1] + arr[i-2].
													# This tells that arr[i] stores the 'i'th FIBONACCI number where 1st number is 1.
	
	addl	$1, -44(%rbp)							# i <-- i + 1

.L2:												# local label 
													# all assembly code in this label represents a fragment of the C code
	
	movl	-44(%rbp), %eax							# eax <-- Mem[rbp-44]        (eax = i).
	
	cmpl	-52(%rbp), %eax							# compare eax and x(Mem[rbp-52]) 
	
	jle	.L3											# currently eax=i=2 and x=9 : eax is less than x, so go to section .L3:
													# Keep going to section .L3 till i<=9. This probably is a while or a for loop. 
	
	movq	-32(%rbp), %rax							# rax = Mem[rbp-32] = rbp-104
	
	movl	-52(%rbp), %edx							# edx = x = 9
	
	movslq	%edx, %rdx								# rdx = 9
	
	movl	(%rax,%rdx,4), %eax						# eax = Mem[rax + 4*rdx] = Mem[rbp-104 + 4*9] = arr[9]
													# return value of func is set to arr[9] which is 34, the 9th FIBONACCI number.
	
	movq	%rsi, %rsp								# rsp <-- rsi      (old stack pointer retrieved. Now rsp = rbp-56)
	
	movq	-24(%rbp), %rbx							# rbx <-- Mem[rbp-24]     (%rbx = %fs:40)
	
	xorq	%fs:40, %rbx							# rbx = 0
	
	je	.L5											# jump to .L5 if rbx equals zero. Since rbx equals zero, jump to section .L5
	
	call	__stack_chk_fail@PLT

.L5:												# Local Label
	
	movq	-8(%rbp), %rbx							# rbx <-- Mem[rbp-8]
	
	leave											# Destroy current stack frame
	
	.cfi_def_cfa 7, 8
	
	ret												# value in eax is returned i.e. arr[9], which is 34, is returned.
	
	.cfi_endproc									# end of func function.

.LFE0:												# Local Label

	.size	func, .-func

	.section	.rodata								# read only data section

.LC0:												# Label

.string	"%d"										# "%d" is a data of type string. This tells that "%d" appears somewhere in the code 

	.text											# This indicates start of code

	.globl	main									# main is a global name

	.type	main, @function							# main is of type function

main:												# definition of function main

.LFB1:												# a label indicating a fragment of C code i.e. the contents of function main.

	.cfi_startproc									# cfi directive indicating the start of main function.

	pushq	%rbp									# save old base pointer. Points to the base of the previous stack frame. 

	.cfi_def_cfa_offset 16

	.cfi_offset 6, -16

	movq	%rsp, %rbp								# save stack pointer. stack pointer needs to be saved because its value may change to 
													# allocate space to other variables. 

	.cfi_def_cfa_register 6

	subq	$16, %rsp								# rsp <-- rsp - 16. Allocate space for local variables in the code.

	movl	$9, -4(%rbp)							# Mem[rbp-4] <-- 9. Value at location rbp-4 in the stack is 9. A variable is declared   
													# in the code and initialised to 9. Let's call this variable 'a'.

	movl	-4(%rbp), %eax							# eax <-- Mem[rbp-4] implies eax = a. Value of eax is 9;

	movl	%eax, %edi								# edi <-- eax. edi = 9. Possibly first argument of a function to be called later.

	call	func									# a call to the function func with integer 9 as first argument.

	movl	%eax, %esi								# esi <-- eax. Return value from function func is stored in esi.
													# Let's say that a variable t was declared and stores the value returned by func.
													# Possibly the return value is the second argument of a function to be called later.

	leaq	.LC0(%rip), %rdi						# rdi <-- "%d". Possibly "%d" is the first argument of a function to be called later.

	movl	$0, %eax								# eax <-- 0.

	call	printf@PLT								# call to printf with "%d" as first argument and return value from func as 2nd argument.

	call	getchar@PLT								# call to function getchar with no argument. Waiting for the user to input something.

	movl	$0, %eax								# As soon as user presses enter eax is set to 0. Return value of function main is zero.

	leave											# Destroy current stack frame

	.cfi_def_cfa 7, 8

	ret												# value of eax is returned.

	.cfi_endproc									# end of function main.

.LFE1:												# Local label

	.size	main, .-main

	.ident	"GCC: (GNU) 7.1.1 20170630"

	.section	.note.GNU-stack,"",@progbits
	
	
	
	
#  The main function declares a variable (say 'a') and initialises it to 9 as indicated by the instruction 'movl   $9, -4(%rbp)' on line 249.
#  It is something like int a = 9;

#  Then the main function calls the function func with 'a' as the only argument. It is something like func(a);

#  FUNCTION func
#  There is a variable (say x) in func that takes the value passed to func as an argument.This is indicated by the instruction 
#  'movl	   %edi, -52(%rbp)' on line 47. Declaration of func is like :-  int func(int x)

#  Then an array (say 'arr') is declared probably of size 12 (12 = x+3) as indicated by instruction 'subq	%rax, %rsp' on line 104.
#  Value 0 is assigned to arr[0] as indicated by instruction 'movl	$0, (%rax)' on line 122 where rax is a pointer to arr.
#  Value 1 is assigned to arr[1] as indicated by instruction 'movl	$1, 4(%rax)' on line 126 where rax is a pointer to arr. 
#  A variable (say i) is declared and is initialised to 2 as indicated by instruction 'movl	$2, -44(%rbp)' on line 128.

#  The pair of instructions 'cmpl	-52(%rbp), %eax' on line 178 and jle .L3 on line 180 indicate that control jumps to a certain fragment 
#  of code when i <= x. Probably a while or for loop.
#  The instructions 'addl	%eax, %ecx' on line 158 and 'movl	%ecx, (%rax,%rdx,4)' on line 166 indicate that arr[i] = arr[i-1] + arr[i-2].
#  Instruction 'addl	$1, -44(%rbp)' on line 171 is assembly for 'i<--i+1'. The loop is probably a while or for loop.


#  Once the loop execution is complete func returns arr[x] i.e. arr[9]. This returned value is then printed by the main function.
#  After the number is printed the getchar() method waits for the user to input a character and then main returns 0.

#  The function func contains an array which is iteratively assigned with values such that arr[i] = arr[i-1] + arr[i-2]. This means arr 
#  stores the FIBONACCI sequence starting from 1.

#  --------------------------------------------------------------------------------------------------------------------------

#  The conclusion is that the above C program computes 9th FIBONACCI number starting from 1 by computing the first 8 FIBONACCI numbers.
#  This bottom up approach is used in Dynamic Programming.

#  Hence the above C program computes 9th FIBONACCI number starting from 1 using Dynamic Programming.

#  --------------------------------------------------------------------------------------------------------------------------

#  test.c may look like :-

#  int func(int x){
#  		int arr[x+3];
#		arr[0] = 0;
#		arr[1] = 1;
#
#		int i = 2;
#
#		while(i<=x){
#			arr[i] = arr[i-1] + arr[i-2];
#			i++;
#		}

#		return arr[x];
#	}

#	int main(){
#		int a = 9;
#		int t = func(9);
#		printf("%d",t);
#		getchar();
#		return 0;
#	}
