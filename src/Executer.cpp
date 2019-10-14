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
            dump();
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

    if (_Array.size() >= _Array.max_size())
        throw Executer::ExecuterException("Size of conteiner is too big");
    if (instruction == "int8")
    {
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Int8, value));
        _Array.push_back(ptr);
    }
    else if (instruction == "int16")
    {
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Int16, value));
        _Array.push_back(ptr);
    }
    else if (instruction == "int32")
    { 
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Int32, value));
        _Array.push_back(ptr);
    }
    else if (instruction == "float")
    {
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Float, value));
        _Array.push_back(ptr);
    }
    else if (instruction == "double")
    {
        std::shared_ptr<const IOperand> ptr(factory.createOperand(Double, value));
        _Array.push_back(ptr);
    }
}   

void Executer::pop()
{
    if (_Array.empty())
        throw ExecuterException("Error with pop, stack is empty");
    _Array.pop_back();
}

void Executer::dump()
{
    for (auto it = _Array.rbegin(); it != _Array.rend(); it++)
    {
        std::cout << it->get()->toString() << std::endl;
    }
}

void Executer::add()
{
    if (_Array.size() < 2)
        throw ExecuterException("when add, size is < 2");
    auto it = _Array.rbegin();
    const IOperand *x = it->get();
    it++;
    const IOperand *y = it->get();
    std::shared_ptr<const IOperand> ptr(*x + *y);
    _Array.pop_back();
    _Array.pop_back();
    _Array.push_back(ptr);
}

void Executer::sub()
{
    if (_Array.size() < 2)
        throw ExecuterException("when sub, size is < 2");
    auto it = _Array.rbegin();
    const IOperand *x = it->get();
    it++;
    const IOperand *y = it->get();
    std::shared_ptr<const IOperand> ptr(*x - *y);
    _Array.pop_back();
    _Array.pop_back();
    _Array.push_back(ptr);
}

void Executer::mul()
{
    if (_Array.size() < 2)
        throw ExecuterException("when mul, size is < 2");
    auto it = _Array.rbegin();
    const IOperand *x = it->get();
    it++;
    const IOperand *y = it->get();
    std::shared_ptr<const IOperand> ptr(*x * *y);
    _Array.pop_back();
    _Array.pop_back();
    _Array.push_back(ptr);
}

void Executer::div()
{
    if (_Array.size() < 2)
        throw ExecuterException("when div, size is < 2");
    auto it = _Array.rbegin();
    const IOperand *x = it->get();
    it++;
    const IOperand *y = it->get();
    if (y->toString() == "0")
        throw ExecuterException("when div, divisior is 0");
    std::shared_ptr<const IOperand> ptr(*x / *y);
    _Array.pop_back();
    _Array.pop_back();
    _Array.push_back(ptr);
}

void Executer::mod()
{
    if (_Array.size() < 2)
        throw ExecuterException("when mod, size is < 2");
    auto it = _Array.rbegin();
    const IOperand *x = it->get();
    it++;
    const IOperand *y = it->get();
    if (y->toString() == "0")
        throw ExecuterException("when mod, divisior is 0");
    std::shared_ptr<const IOperand> ptr(*x % *y);
    _Array.pop_back();
    _Array.pop_back();
    _Array.push_back(ptr);
}

void Executer::print()
{
    if (_Array.empty())
        throw ExecuterException("Error print, _Array is empty");
    if ( _Array.back().get()->getPrecision() != 0 )
        throw ExecuterException("Error print, top value is not int8");
    std::cout << static_cast<char>(std::stoi( _Array.back().get()->toString() )) << std::endl;
}

void Executer::assert(std::string type, std::string value)
{    
    if (_Array.empty())
        throw ExecuterException("Error! Assert is empty");

    try
    {
        std::stringstream stream;
        std::string str;
        if (type == "int8")
        {
            if (Int8 != _Array.back().get()->getPrecision() || std::stoi(value) != std::stoi(_Array.back().get()->toString()))
                throw ExecuterException("Error! top value is another");
        }
        else if (type == "int16")
        {
            if (Int16 != _Array.back().get()->getPrecision() || std::stoi(value) != std::stoi(_Array.back().get()->toString()))
                throw ExecuterException("Error! top value is another");
        }
        else if (type == "int32")
        {
            if (Int32 != _Array.back().get()->getPrecision() || std::stoi(value) != std::stoi(_Array.back().get()->toString()))
                throw ExecuterException("Error! top value is another");
        }
        else if (type == "float")
        {
            auto tmp = std::stof(value);
            stream << tmp;
            stream >> str;

            if (Float != _Array.back().get()->getPrecision() || std::stof(str) != std::stof(_Array.back().get()->toString()))
                throw ExecuterException("Error! top value is another");
        }
        else if (type == "double")
        {
            auto tmp = std::stod(value);
            stream << tmp;
            stream >> str;
            if (Double != _Array.back().get()->getPrecision() || std::stod(str) != std::stod(_Array.back().get()->toString()))
                throw ExecuterException("Error! top value is another");
        }
    }
    catch(std::out_of_range &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::invalid_argument &e)
    {
        throw Executer::ExecuterException(e.what());
    }
}