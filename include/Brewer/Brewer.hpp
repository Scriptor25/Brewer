#pragma once

#undef NOERR

#include <format>
#include <iostream>
#include <string>
#include <vector>

namespace Brewer
{
    class Context;
    class Module;
    class Builder;
    class Parser;

    class Printer;
    class IRPrinter;
    class RiscVPrinter;
    class X86Printer;

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

    template <typename... Args>
#ifndef NOERR
    [[noreturn]]
#endif
    void Error(const std::string& format, Args... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        std::cerr << message << std::endl;
#ifndef NOERR
        throw std::runtime_error(message);
#endif
    }

    template <typename T = Value>
    void Replace(std::vector<T*>& values, Value* old_value, Value* new_value)
    {
        for (auto& value : values)
            if (value == old_value)
            {
                value = dynamic_cast<T*>(new_value);
                return;
            }
    }

    template <typename T = NamedValue>
    T* Find(const std::vector<T*>& values, Type* type, const std::string& name)
    {
        for (const auto& value : values)
            if (value->GetName() == name)
            {
                if (value->GetType() != type)
                    Error("type mismatch: {} != {}", value->GetType(), type);
                return value;
            }
        return {};
    }

    template <typename T = NamedValue>
    T* Find(const std::vector<T*>& values, const std::string& name)
    {
        for (const auto& value : values)
            if (value->GetName() == name)
                return value;
        return {};
    }

    template <typename T = NamedValue>
    T* Erase(std::vector<T*>& values, const std::string& name)
    {
        for (auto it = values.begin(); it != values.end(); ++it)
            if ((*it)->GetName() == name)
            {
                const auto value = *it;
                values.erase(it);
                return value;
            }
        return {};
    }
}
