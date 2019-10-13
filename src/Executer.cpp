#include "../headers/Executer.h"
#include "../headers/operand.h"
#include "../headers/IOperand.h"
#include <iostream>
#include <sstream>
#include <string>

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

Executer & Executer::operator =( const Executer &rhs )
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
            add(_Array);
        else if (buf == "sub")
            sub(_Array);
        else if (buf == "mul")
            mul(_Array);
        else if (buf == "div")
            div(_Array);
        else if (buf == "print")
            print();
        else if (buf == "mod")
            mod(_Array);
        return 0;
    }
    else
    {
        size_t pos1 = buf.find_first_of('(');
        if (pos1 == std::string::npos)
            return 2;
        std::string instruction = buf.substr(0, pos1);//get instruction
        buf = buf.substr(pos1 + 1);
        size_t pos2 = buf.find_first_of(')');
        if (pos2 == std::string::npos)
            return 2;
        std::string value = buf.substr(0, pos2);//get value
        if (!(buf.substr(pos2 + 1)).empty())
            return 2;

        for(auto it = value.begin(); it != value.end(); it++)
        { 
            if ( *it != '-' && *it != '.' && *it != 'f' && *it != '1' && *it != '2' && *it != '3' && *it != '4' && *it != '5' && *it != '6' && *it != '7' && *it != '8' && *it != '9' && *it != '0')
                return 2;
        }
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
    return 2;
}

void Executer::parsProgramm(std::istream & readStream, bool file_mode)
{
    std::string buf;
    bool isRun = true;
    bool isInstruction = false;
    bool isValue = false;
    bool isExit = false;
    std::string Instruction = "";
    while (isRun)
    {
        std::getline(readStream, buf);
        if (!readStream.good() && file_mode == false)
            throw ExecuterException("bad line");
        if (file_mode == true)
        {
            if (readStream.bad())
                throw ExecuterException("bad line");
            if (readStream.eof() && isRun == true && buf.find("exit") == std::string::npos)
                throw ExecuterException("No instruction exit when EOF");
        }
        if (buf.find("exit") != std::string::npos)
        {
            if (file_mode == true)
            {
                isRun = false;
                isExit = true;
                break ;
            }
            else
            {
                isExit = true;
                continue ;
            }
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
            size_t pos;
            if ((pos = buf.find_first_of(';')) != std::string::npos)
                buf = buf.substr(0, pos);
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

    if (_Array.size() > 1000)
        throw Executer::ExecuterException("Size of stack is too big");
    if (instruction == "int8")
    {
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Int8, value));
        _Array.push(ptr);
    }
    else if (instruction == "int16")
    {
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Int16, value));
        _Array.push(ptr);
    }
    else if (instruction == "int32")
    { 
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Int32, value));
        _Array.push(ptr);
    }
    else if (instruction == "float")
    {
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Float, value));
        _Array.push(ptr);
    }
    else if (instruction == "double")
    {
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Double, value));
        _Array.push(ptr);
    }
}   

void Executer::pop()
{
    if (_Array.empty())
        throw ExecuterException("Error with pop, stack is empty");
    _Array.pop();
}

void Executer::dump(std::stack<std::shared_ptr<const IOperand>> src)
{
    auto run = (src.size());
    while (run--)
    {
        std::cout << src.top().get()->toString() << std::endl;
        src.pop();
    }
}

void Executer::add(std::stack<std::shared_ptr<const IOperand> > src)
{
    if (_Array.size() < 2)
        throw ExecuterException("when add, size is < 2");
    
    const IOperand *x = src.top().get();
    src.pop();
    const IOperand *y = src.top().get();
    src.pop();
    std::shared_ptr<const IOperand> ptr(*x + *y);
    _Array.pop();
    _Array.pop();
    _Array.push(ptr);
}

void Executer::sub(std::stack<std::shared_ptr<const IOperand> > src)
{
    if (_Array.size() < 2)
        throw ExecuterException("when sub, size is < 2");
    const IOperand *x = src.top().get();
    src.pop();
    const IOperand *y = src.top().get();
    src.pop();
    std::shared_ptr<const IOperand> ptr(*x - *y);
    _Array.pop();
    _Array.pop();
    _Array.push(ptr);
}

void Executer::mul(std::stack<std::shared_ptr<const IOperand> > src)
{
    if (_Array.size() < 2)
        throw ExecuterException("when mul, size is < 2");
    const IOperand *x = src.top().get();
    src.pop();
    const IOperand *y = src.top().get();
    src.pop();
    std::shared_ptr<const IOperand> ptr(*x * *y);
    _Array.pop();
    _Array.pop();
    _Array.push(ptr);
}

void Executer::div(std::stack<std::shared_ptr<const IOperand> > src)
{
    if (_Array.size() < 2)
        throw ExecuterException("when div, size is < 2");
    const IOperand *x = src.top().get();
    src.pop();
    const IOperand *y = src.top().get();
    src.pop();
    if (y->toString() == "0")
        throw ExecuterException("when div, divisior is 0");
    std::shared_ptr<const IOperand> ptr(*x / *y);
    _Array.pop();
    _Array.pop();
    _Array.push(ptr);
}

void Executer::mod(std::stack<std::shared_ptr<const IOperand> > src)
{
    if (_Array.size() < 2)
        throw ExecuterException("when mod, size is < 2");
    const IOperand * x = src.top().get();
    src.pop(); 
    const IOperand * y = src.top().get();
    src.pop();
    if (y->toString() == "0")
        throw ExecuterException("when mod, divisior is 0");
    std::shared_ptr<const IOperand> ptr(*x % *y);
    _Array.pop();
    _Array.pop();
    _Array.push(ptr);
}

void Executer::print()
{
    if ( _Array.top().get()->getPrecision() != 0 )
        throw ExecuterException("Error print, top value is not int8");
    std::cout << _Array.top().get()->toString() << std::endl;
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
    if (!(value == _Array.top().get()->toString() && enumtype == _Array.top().get()->getPrecision()))
        throw ExecuterException("Error with assert");
}