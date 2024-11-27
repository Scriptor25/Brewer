type person = { i32, [i8] }

var @message = [i8 x 14] "Hello, world!\00"
var @name = [i8 x 6] "Felix\00"
var @fmt = [i8 x 23] "{ age: %d, name: %s }\0A\00"

fun i32 @puts([i8] %str)
fun i32 @printf([i8] %fmt, ...)

fun [i32([i8])] intern @foo() {
    ret [i32([i8])] @puts
}

fun [person] @person([person] %this, i32 %age, [i8] %name) {
    %age_ptr = [i32] gep [person] %this
    store [i32] %age_ptr, i32 %age
    %name_ptr = [[i8]] gep [person] %this, i64 0, i64 1
    store [[i8]] %name_ptr, [i8] %name
    ret [person] %this
}

fun i32 @get_age([person] %this) {
    %age_ptr = [i32] gep [person] %this
    %age = i32 load [i32] %age_ptr
    ret i32 %age
}

fun [i8] @get_name([person] %this) {
    %name_ptr = [[i8]] gep [person] %this, i64 0, i64 1
    %name = [i8] load [[i8]] %name_ptr
    ret [i8] %name
}

fun i32 @main(i32 %argc, [[i8]] %argv) {
    %p_ptr = [person] alloca i64 1

    %msg_str = [i8] gep [[i8 x 14]] @message
    %bar = [i32([i8])] call [[i32([i8])]()] @foo
    call [i32([i8])] %bar, [i8] %msg_str

    %name_str = [i8] gep [[i8 x 6]] @name
    call [[person]([person], i32, [i8])] @person, [person] %p_ptr, i32 18, [i8] %name_str

    %fmt_str = [i8] gep [[i8 x 23]] @fmt
    %p_age = i32 call [i32([person])] @get_age, [person] %p_ptr
    %p_name = [i8] call [[i8]([person])] @get_name, [person] %p_ptr
    call [i32([i8], ...)] @printf, [i8] %fmt_str, i32 %p_age, [i8] %p_name

    ret i32 0
}
