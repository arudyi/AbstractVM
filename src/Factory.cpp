#include <iostream>
#include <string>
#include <math.h>
#include <cfloat>
#include "../headers/Factory.h"
#include "../headers/IOperand.h"
#include "../headers/eOperandType.h"
#include "../headers/operand.h"
#include "../headers/Executer.h"

Factory::Factory()
{
    ArrayPtr.reserve(5);
    ArrayPtr[0] = &Factory::createInt8;
    ArrayPtr[1] = &Factory::createInt16;
    ArrayPtr[2] = &Factory::createInt32;
    ArrayPtr[3] = &Factory::createFloat;
    ArrayPtr[4] = &Factory::createDouble;
}

Factory::~Factory()
{
    
}

Factory::Factory(const Factory & rhs)
{
    *this = rhs;
}
        
Factory & Factory::operator=(const Factory & rhs)
{
    if (this == &rhs)
        return *this;
    return *this;
}    

IOperand const * Factory::createOperand( eOperandType type, std::string const & value ) const
{
    return (this->*ArrayPtr[type])(value);
}

IOperand const * Factory::createInt8( std::string const & value ) const
{
    try
    {
        auto tmp = std::stold(value);
        double intPart, fractPart;
    
        fractPart = modf(tmp, &intPart);
        if (fractPart != 0 || intPart > INT8_MAX || intPart < INT8_MIN)
            throw FactoryException("Bad value");
        else
            return (new operand<int8_t>(static_cast<int8_t>(tmp), Int8));
    }
    catch(FactoryException &e)
    {
        throw Executer::ExecuterException(e.GetMessage());
    }
    catch(std::out_of_range &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::invalid_argument &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::bad_alloc &e)
    {
        throw Executer::ExecuterException(e.what());
    }
}

IOperand const * Factory::createInt16( std::string const & value ) const
{
    try
    {
        auto tmp = std::stold(value);
        double intPart, fractPart;
    
        fractPart = modf(tmp, &intPart);
        if (fractPart != 0 || intPart > INT16_MAX || intPart < INT16_MIN)
            throw FactoryException("Bad value");
        else
            return (new operand<int16_t>(static_cast<int16_t>(tmp), Int16));
    }
    catch(FactoryException &e)
    {
        throw Executer::ExecuterException(e.GetMessage());
    }
    catch(std::out_of_range &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::invalid_argument &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::bad_alloc &e)
    {
        throw Executer::ExecuterException(e.what());
    }
}

IOperand const * Factory::createInt32( std::string const & value ) const
{
    try
    {
        auto tmp = std::stold(value);
        double intPart, fractPart;
    
        fractPart = modf(tmp, &intPart);
        if (fractPart != 0 || intPart > INT32_MAX || intPart < INT32_MIN)
            throw FactoryException("Bad value");
            
        return (new operand<int32_t>(static_cast<int32_t>(tmp), Int32));
    }
    catch(FactoryException &e)
    {
        throw Executer::ExecuterException(e.GetMessage());
    }
    catch(std::out_of_range &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::invalid_argument &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::bad_alloc &e)
    {
        throw Executer::ExecuterException(e.what());
    }
}

IOperand const * Factory::createFloat( std::string const & value ) const
{
    try
    {
        auto tmp = std::stof(value);
        return (new operand<float>(static_cast<float>(tmp), Float));
    }
    catch(FactoryException &e)
    {
        throw Executer::ExecuterException(e.GetMessage());
    }
    catch(std::out_of_range &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::invalid_argument &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::bad_alloc &e)
    {
        throw Executer::ExecuterException(e.what());
    }
}

IOperand const * Factory::createDouble( std::string const & value ) const
{
    try
    {
        auto tmp = std::stod(value);
        return new operand<double>(static_cast<double>(tmp), Double);
    }
    catch(FactoryException &e)
    {
        throw Executer::ExecuterException(e.GetMessage());
    }
    catch(std::out_of_range &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::invalid_argument &e)
    {
        throw Executer::ExecuterException(e.what());
    }
    catch(std::bad_alloc &e)
    {
        throw Executer::ExecuterException(e.what());
    }
}

Factory::FactoryException::FactoryException () noexcept
{

}

Factory::FactoryException::FactoryException (std::string its_message) noexcept
{
    _Message = its_message;
}

Factory::FactoryException::FactoryException (const exception & rhs) noexcept
{
    *this = rhs;
}

Factory::FactoryException & Factory::FactoryException::operator= (const exception & rhs) noexcept
{
    if (this == &rhs)
        return *this;
    return *this;
}

Factory::FactoryException::~FactoryException()
{

}

const std::string Factory::FactoryException::GetMessage() const
{
    return _Message;
}