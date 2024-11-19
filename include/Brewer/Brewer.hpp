#pragma once

#include <format>
#include <sstream>
#include <string>

namespace Brewer
{
    [[noreturn]] void Error(const std::string& message);

    template <typename... Args>
    [[noreturn]] void Error(const std::string& format, Args... args)
    {
        Error(std::vformat(format, std::make_format_args(args...)));
    }

    class Context;
    class Module;
    class Builder;
    class Parser;

    class Type;
    class VoidType;
    class BlockType;
    class IntType;
    class FloatType;
    class PointerType;
    class ArrayType;
    class StructType;
    class FunctionType;

    class Value;

    class Constant;
    class ConstantInt;
    class ConstantFloat;
    class ConstantArray;
    class ConstantStruct;

    class NamedValue;
    class GlobalValue;
    class GlobalVariable;
    class Function;

    class Argument;
    class Block;

    class Instruction;
}
