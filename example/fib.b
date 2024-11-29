var @message = [i8 x 14] "fib(%d) = %d\0A\00"

;
; main function, program entry point
; args:
;  argc -> i32
;  argv -> [[i8]]
; result: i32
;
fun i32 @main(i32 %argc, [[i8]] %argv) {
    %n_str_ptr = [[i8]] gep [[i8]] %argv, i64 1
    %n_str = [i8] load [[i8]] %n_str_ptr
    %n = i32 call [i32([i8])] @atoi, [i8] %n_str
    %result = i32 call [i32(i32)] @fib, i32 %n

    %msg_str = [i8] gep [[i8 x 14]] @message, i64 0, i64 0
    call [i32([i8], ...)] @printf, [i8] %msg_str, i32 %n, i32 %result

    ret i32 0
}

fun i32 @printf([i8] %format, ...)
fun i32 @atoi([i8] %str)
fun i32 @puts([i8] %str)

;
; fib function, computes the Nth number in the fibonacci sequence
; args:
;  n -> i32
; result: i32
;
fun i32 local @fib(i32 %n) {
    %i = i32 0
    %a = i32 0
    %b = i32 1
    br %head
head:
    br.lt i32 %i, i32 %n, %loop, %end
loop:
    %c = i32 add i32 %a, i32 %b
    %a = i32 %b
    %b = i32 %c
    %i = i32 add i32 %i, i32 1
    br %head
end:
    ret i32 %a
}
