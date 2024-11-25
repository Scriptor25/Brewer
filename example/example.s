.section .data
.local message
message: .quad .LC14
.section .rodata
.LC14: .byte 102, 105, 98, 40, 37, 100, 41, 32, 61, 32, 37, 100, 10, 0
.section .text
.global main
main:
pushq %rbp
movq %rsp, %rbp
.L22:
