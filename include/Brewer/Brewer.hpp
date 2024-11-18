#pragma once

#include <format>
#include <string>

namespace Brewer
{
    [[noreturn]] void Error(const std::string& message);

    template <typename... Args>
    [[noreturn]] void Error(const std::string& format, Args... args)
    {
        Error(std::format(format, std::make_format_args(args...)));
    }

    class Context;
    class Module;
    class Builder;
    class Parser;

    class Type;
    class VoidType;
    class IntType;
    class FloatType;
    class PointerType;
    class ArrayType;
    class StructType;
    class FunctionType;

    class Value;
    class Constant;
    class GlobalValue;
    class Function;
}
