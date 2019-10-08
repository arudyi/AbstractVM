#include "Executer.h"
#include <iostream>
#include <sstream>
#include <string>
#include <deque>

Executer::Executer()
{

}

Executer::~Executer()
{

}

Executer::Executer(const Executer &rhs)
{
    *this = rhs;
}

Executer & Executer::operator = ( const Executer &rhs )
{
    if (this == &rhs)
        return *this;
    this->_Array = rhs._Array;
    return *this;
}

Executer::ExecuterException::ExecuterException () noexcept
{

}

Executer::ExecuterException::ExecuterException (std::string its_message) noexcept:
_Message(its_message)
{

}

Executer::ExecuterException::ExecuterException (const ExecuterException & rhs ) noexcept
{
    *this = rhs;
}

Executer::ExecuterException & Executer::ExecuterException::operator = (const ExecuterException & rhs ) noexcept
{
    if (this == &rhs)
        return *this;
    _Message = rhs._Message;
    return *this;
}

Executer::ExecuterException::~ExecuterException()
{

}

const std::string Executer::ExecuterException::GetMessage() const
{
    return _Message;
}

std::vector<std::string> Executer::split(const std::string & src, char delimetr)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenstream(src);
    while (std::getline(tokenstream, token, delimetr))
        tokens.push_back(token);
    return tokens;
}

short Executer::parseInstruction(std::string & buf, std::string *Instruction)
{
    if (buf == "push" || buf == "assert" || buf == "pop" || buf == "dump" || buf == "add" || buf == "sub" || buf == "mul" || buf == "div" || buf == "print" || buf == "mod")
    {
        if (buf == "push" || buf == "assert")
            *Instruction = buf;
        else if (buf == "pop")
            pop();
        else if (buf == "dump")
            dump(_Array);
        else if (buf == "add")
            add();
        else if (buf == "sub")
            sub();
        else if (buf == "mul")
            mul();
        else if (buf == "div")
            div();
        else if (buf == "print")
            print();
        else if (buf == "mod")
            mod();
        return 0;
    }
    else
    {
        size_t pos = buf.find_first_of('(');
        if (pos == std::string::npos)
            return 2;
        else
        {
            std::string instruction = buf.substr(0, pos);
            buf = buf.substr(pos + 1);
            pos = buf.find_first_of(')');
            if (pos == std::string::npos)
                return 2;
            std::string value = buf.substr(0, pos);
            if (instruction == "int8" || instruction == "int16" || instruction == "int32" || instruction == "float" || instruction == "double")
            {
                if (*Instruction == "push")
                    push(instruction, value);
                else if (*Instruction == "assert")
                    assert(instruction, value);
                return 1;
            }
            else
                return 2;
        }
    }
    return 2;
}

void Executer::parsProgramm(std::istream & readStream, bool file_mode)
{
    std::string buf;
    std::stringstream os;
    bool isRun = true;
    bool isInstruction = false;
    bool isValue = false;
    bool isExit = false;
    std::string Instruction = "";
    while (isRun)
    {
        std::getline(readStream, buf);
        if (!readStream.good())
            throw ExecuterException("bad line");
        if (buf.find("exit") != std::string::npos)
        {
            if (file_mode == true)
                isRun = false;
            isExit = true;
        }
        if (buf.find(";;") != std::string::npos)
        {
            if (file_mode == true)
                throw ExecuterException(";; in file_mode");
            if (isExit == true)
                isRun = false;
            else
                throw ExecuterException("No instruction exit");
        }
        if (!buf.empty())
        {
            os << buf;
            std::getline(os, buf, ';');
            os.str("");
            std::vector<std::string> array = split(buf, ' ');
            short returnValue;
            for (auto it = array.begin(); it != array.end(); it++)
            {
                returnValue = parseInstruction(*it, &Instruction);
                if (returnValue == 0)
                {
                    if (isInstruction == true)
                        throw ExecuterException("too much instructions");
                    isInstruction = true;
                }
                else if (returnValue == 1)
                {
                    if (isValue == true )
                        throw ExecuterException("too much values");
                    isValue = true;
                }
                else
                    throw ExecuterException("bad instruction");
            }
            Instruction = "";
            isInstruction = false;
            isValue = false;
        }
    }
}

void Executer::push(const std::string &instruction, std::string & value)
{
    Factory factory;
    if (instruction == "int8")
        _Array.push(factory.createOperand(Int8, value));
    else if (instruction == "int16")
        _Array.push(factory.createOperand(Int16, value));
    else if (instruction == "int32")
        _Array.push(factory.createOperand(Int32, value));
    else if (instruction == "float")
        _Array.push(factory.createOperand(Float, value));
    else if (instruction == "double")
        _Array.push(factory.createOperand(Double, value));
}   

void Executer::pop()
{
    if (_Array.empty())
        throw ExecuterException("Error with pop, stack is empty");
    _Array.pop();
}

void Executer::dump(std::stack<const IOperand *> src)
{
    auto run = (src.size());
    while (run--)
    {
        std::cout << (src.top())->toString() << std::endl;
        src.pop();
    }
}

void Executer::add()
{
    if (_Array.size() < 2)
        throw ExecuterException("when add, size is < 2");
    const IOperand * x = _Array.top();
    _Array.pop(); 
    const IOperand * y = _Array.top();
    _Array.pop();
    _Array.push(*x + *y);
}

void Executer::sub()
{
    if (_Array.size() < 2)
        throw ExecuterException("when sub, size is < 2");
    const IOperand * x = _Array.top();
    _Array.pop(); 
    const IOperand * y = _Array.top();
    _Array.pop();
    _Array.push(*x - *y);
}

void Executer::mul()
{
    if (_Array.size() < 2)
        throw ExecuterException("when mul, size is < 2");
    const IOperand * x = _Array.top();
    _Array.pop(); 
    const IOperand * y = _Array.top();
    _Array.pop();
    _Array.push(*x * *y);
}

void Executer::div()
{
    if (_Array.size() < 2)
        throw ExecuterException("when div, size is < 2");
    const IOperand * x = _Array.top();
    _Array.pop(); 
    const IOperand * y = _Array.top();
    _Array.pop();
    int precision = y->getPrecision();
    if (precision == 0|| precision == 1 || precision == 2)
        if (y->toString() == "0")
            throw ExecuterException("when div, divisior is 0");
    _Array.push(*x / *y);
}

void Executer::mod()
{
    if (_Array.size() < 2)
        throw ExecuterException("when div, size is < 2");
    const IOperand * x = _Array.top();
    _Array.pop(); 
    const IOperand * y = _Array.top();
    _Array.pop();
    int precision = y->getPrecision();
    if (precision == 0|| precision == 1 || precision == 2)
        if (y->toString() == "0")
            throw ExecuterException("when mod, divisior is 0");
    _Array.push(*x % *y);
}

void Executer::print()
{
    if ((_Array.top())->getPrecision() != 0)
        throw ExecuterException("Error print, top value is not int8");
    std::cout << (_Array.top())->toString() << std::endl;
}

void Executer::assert(std::string type, std::string value)
{
    int enumtype;
    if (type == "int8")
        enumtype = 0;
    else if (type == "int16")
        enumtype = 1;
    else if (type == "int32")
        enumtype = 2;
    else if (type == "float")
        enumtype = 3;
    else if (type == "double")
        enumtype = 4;
    if (!(value == (_Array.top())->toString() && enumtype == (_Array.top())->getPrecision()))
        throw ExecuterException("Error with assert");
}