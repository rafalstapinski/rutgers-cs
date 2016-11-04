.globl Factorial
	.type	Factorial, @function
Factorial:
    # Your code for Factorial should go here

    pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$1, -4(%ebp)
    jmp

.MULT:
    movl    -4(%ebp), %eax
    imull   8(%ebp), %eax
    jo      .OVERFLOW
    movl    %eax, -4(%ebp)
    subl    $1, 8(%ebp)

.RPT:
    cmpl    $0, 8(%ebp)
    jg      .MULT
    movl    -4(%ebp), %eax
    jmp     .FACT_END

.OVERFLOW:
    movl    $0, %eax

.FACT_END:
    movl    %ebp, %esp
    popl    %ebp
    ret

.globl nCr
	.type	nCr, @function
nCr:
  # Your code for nCr should go here
