global intern @message = [i8 x 14] "fib(%d) = %d\0A\00"

;
; main function, program entry point
; args:
;  argc -> i32
;  argv -> [[i8]]
; result: i32
;
define extern i32 @main(i32 %argc, [[i8]] %argv) {
    %0 = i32 call [i32(i32)] @fib, i32 10
    %1 = [i8] ptrcast [[i8 x 14]] @message
    call [i32([i8], ...)] @printf, [i8] %1, i32 10, i32 %0
    ret i32 %0
}

declare extern i32 @printf([i8] %format, ...)

;
; fib function, computes the Nth number in the fibonacci sequence
; args:
;  n -> i32
; result: i32
;
define intern i32 @fib(i32 %n) {
entry:
    br %head
head:
    %0 = i32 phi %entry, i32 0, %loop, i32 %5
    %1 = i32 phi %entry, i32 0, %loop, i32 %2
    %2 = i32 phi %entry, i32 1, %loop, i32 %4
    %3 = i1 icmp.lt i32 %0, i32 %n
    br i1 %3, %loop, %end
loop:
    %4 = i32 iadd i32 %1, i32 %2
    %5 = i32 iadd i32 %0, i32 1
    br %head
end:
    ret i32 %4
}
