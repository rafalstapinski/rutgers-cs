.globl Factorial
	.type	Factorial, @function
Factorial:
    # Your code for Factorial should go here

    pushl	%ebp
	movl	%esp, %ebp
    subl	$16, %esp

    # move n into ecx since it acts as its own counter, and prep eax
    movl    8(%ebp), %ecx
    movl    $1, %eax
    # .if 0 return 1, if negative return 0
    cmpl    $0, %ecx
    je      .FACT_0
    jl      .FACT_ERROR

.FACT_LOOP:
    # move result of previous loop into eax
    # multiply by counter going down, check if overflow
    imull   %ecx, %eax
    jo      .FACT_ERROR

    # decrement ecx, go again if > 0
    subl    $1, %ecx
    cmpl    $1, %ecx
    jg      .FACT_LOOP
    # .if ecx reaches 1, go to end
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

    # compute n - r, store on stack
    movl    8(%ebp), %eax
    subl    12(%ebp), %eax
    pushl   %eax

    # call Factorial on (n - r) and store on stack
    call    Factorial
    cmpl    $0, %eax
    je      .NCR_ERROR
    movl    %eax, -4(%ebp)

    # compute r!
    pushl   12(%ebp)
    call    Factorial
    cmpl    $0, %eax
    je      .NCR_ERROR

    # multiply r! and (n - r)! and store on stack
    imull   -4(%ebp), %eax
    movl    %eax, -4(%ebp)

    # .push n to stack and compute n!
    pushl   8(%ebp)
    call    Factorial
    cmpl    $0, %eax
    je      .NCR_ERROR

    # clear %edx and divide n! by r!(n - r)! which is -4 from bottom
    movl    $0, %edx
    idivl   -4(%ebp)
    jmp     .NCR_END

.NCR_ERROR:
    movl $0, %eax

.NCR_END:
    movl    %ebp, %esp
    popl    %ebp
    ret
