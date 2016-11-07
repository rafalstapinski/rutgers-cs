.globl Factorial
	.type	Factorial, @function
Factorial:
    # Your code for Factorial should go here
    pushl	%ebp
	movl	%esp, %ebp
    subl	$16, %esp
    movl	$1, %edx
    movl    8(%ebp), %ecx
    cmpl    $0, %ecx
    je      .FACT_0
    jl      .FACT_ERROR

.FACT_LOOP:
    movl    %edx, %eax
    imull   %ecx, %eax
    jo      .FACT_ERROR
    movl    %eax, %edx
    subl    $1, %ecx
    cmpl    $0, %ecx
    jg      .FACT_LOOP
    movl    %edx, %eax
    jmp     .FACT_END

.FACT_0:
    movl    $1, %eax
    jmp     .FACT_END

.FACT_ERROR:
    movl    $0, %eax

.FACT_END:
    movl    %ebp, %esp
    popl    %ebp
    ret

.globl nCr
	.type	nCr, @function
nCr:
  # Your code for nCr should go here
    pushl   %ebp
    movl    %esp, %ebp
    movl    8(%ebp), %eax
    subl    12(%ebp), %eax
    pushl   %eax
    call    Factorial
    cmpl    $0, %eax
    je      .NCR_ERROR
    movl    %eax, -4(%ebp)
    pushl   12(%ebp)
    call    Factorial
    cmpl    $0, %eax
    je      .NCR_ERROR
    imull   -4(%ebp), %eax
    movl    %eax, -4(%ebp)
    pushl   8(%ebp)
    call    Factorial
    cmpl    $0, %eax
    je      .NCR_ERROR
    movl    $0, %edx
    idivl   -4(%ebp)
    jmp     .NCR_END

.NCR_ERROR:
    movl $0, %eax

.NCR_END:
    movl    %ebp, %esp
    popl    %ebp
    ret

