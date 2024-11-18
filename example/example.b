;
; main function, program entry point
; args:
;  argc -> i32
;  argv -> [[i8]]
; result: i32
;
define i32 @main(i32 %argc, [[i8]] %argv) {
    %0 = call [i32(i32)] @fib, i32 10
    ret i32 %0
}

;
; fib function, computes the Nth number in the fibonacci sequence
; args:
;  n -> i32
; result: i32
;
define i32 @fib(i32 %n) {
    %0 = icmp.le i32 %n, i32 1
    br i1 %0, %then, %else
then:
    br %end
else:
    %1 = isub i32 %n, i32 1
    %2 = call [i32(i32)] @fib, i32 %1
    %3 = isub i32 %n, i32 2
    %4 = call [i32(i32)] @fib, i32 %3
    %5 = iadd i32 %2, i32 %4
    br %end
end:
    %6 = phi %[i32 %n, %then], %[i32 %5, %else]
    ret i32 %6
}
