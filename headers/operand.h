#ifndef OPERAND_H
#define OPERAND_H
#include <exception>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "eOperandType.h"
#include "Factory.h"
#include "Executer.h"

template<typename T>
class operand : public IOperand
{
    private:
        T _Value;
        std::string  _StrValue;
        eOperandType _Type;
        operand();
    public:
        operand(T Itsvalue, eOperandType Ittype);
        ~operand();
        operand(const operand & rhs);
        operand<T> & operator=(const operand & rhs);

        virtual eOperandType getType( void ) const; // Type of the instance
        virtual int getPrecision( void ) const; // Precision of the type of the instance
        virtual IOperand const * operator+( IOperand const & rhs ) const; // Sum
        virtual IOperand const * operator-( IOperand const & rhs ) const; // Difference
        virtual IOperand const * operator*( IOperand const & rhs ) const; // Product
        virtual IOperand const * operator/( IOperand const & rhs ) const; // Quotient
        virtual IOperand const * operator%( IOperand const & rhs ) const; // Modulo
        virtual std::string const & toString( void ) const; // String representation of the instance
    class operandException : public std::exception
    {
        private:
            std::string _Message;
            operandException () noexcept;
        public:
            operandException (std::string its_message) noexcept;
            operandException (const operandException & rhs ) noexcept;
            operandException & operator = (const operandException & rhs ) noexcept;
            virtual ~operandException();
            const std::string GetMessage() const;
    };
};

template <typename T>
operand<T>::operand()
{

};

template <typename T>
operand<T>::operand(T Itsvalue, eOperandType Ittype):
_Value(Itsvalue),
_Type(Ittype)
{
    std::stringstream strStream;
    
    if (_Type == Int8)
    {
        strStream << static_cast<int>(_Value);
        strStream >> _StrValue;
    }
    else if (_Type == Float)
    {
        strStream << std::setprecision(8) << _Value;
        strStream >> std::setprecision(8) >> _StrValue;
    }
    else if (_Type == Double)
    {
        strStream << std::setprecision(8) << _Value;
        strStream >> std::setprecision(8) >> _StrValue;
    }
    else
    {
        strStream << _Value;
        strStream >> _StrValue;
    }
};

template <typename T>
operand<T>::~operand()
{
    
};

template <typename T>
eOperandType operand<T>::getType() const
{
    return _Type;
};

template <typename T>
operand<T>::operand(const operand & rhs)
{
    *this = rhs;
};

template <typename T>
operand<T> & operand<T>::operator=(const operand & rhs)
{
    if (this == &rhs)
        return *this;
    _StrValue = rhs._StrValue;
    _Value = rhs._Value;
    _Type = rhs._Type;
    return *this;
};

template <typename T>
int operand<T>::getPrecision( void ) const
{
    return _Type;
}

template <typename T>
std::string const & operand<T>::toString( void ) const
{
    return _StrValue;
}


template <typename T>
IOperand const * operand<T>::operator+( IOperand const & rhs ) const
{
    Factory factory;
    auto x = this->getType();
    auto y = rhs.getType();
    
    if (x == y)
    {
        return factory.createOperand(x, std::to_string( std::stod(this->_StrValue) + std::stod(rhs.toString())));
    }
    else if (x < y)
    {
        return factory.createOperand(y, std::to_string( std::stod(this->_StrValue) + std::stod(rhs.toString())));
    }
    else if (x > y)
    {
        return factory.createOperand(x, std::to_string( std::stod(this->_StrValue) + std::stod(rhs.toString())));
    }
    return NULL;
}

template <typename T>
IOperand const * operand<T>::operator-( IOperand const & rhs ) const
{
    Factory factory;
    auto x = this->getType();
    auto y = rhs.getType();
    
    if (x == y)
    {
        return factory.createOperand(x, std::to_string( std::stod(this->_StrValue) - std::stod(rhs.toString())));
    }
    else if (x < y)
    {
        return factory.createOperand(y, std::to_string( std::stod(this->_StrValue) - std::stod(rhs.toString())));
    }
    else if (x > y)
    {
        return factory.createOperand(x, std::to_string( std::stod(this->_StrValue) - std::stod(rhs.toString())));
    }
    return NULL;
} // Difference

template <typename T>
IOperand const * operand<T>::operator*( IOperand const & rhs ) const
{
    Factory factory;
    auto x = this->getType();
    auto y = rhs.getType();
    
    if (x == y)
    {
        return factory.createOperand(x, std::to_string( std::stod(this->_StrValue) * std::stod(rhs.toString())));
    }
    else if (x < y)
    {
        return factory.createOperand(y, std::to_string( std::stod(this->_StrValue) * std::stod(rhs.toString())));
    }
    else if (x > y)
    {
        return factory.createOperand(x, std::to_string( std::stod(this->_StrValue) * std::stod(rhs.toString())));
    }
    return NULL;
} // Product

template <typename T>
IOperand const * operand<T>::operator/( IOperand const & rhs ) const
{
    Factory factory;
    auto x = this->getType();
    auto y = rhs.getType();
    
    if (x == y)
    {
        if (x == 0 || x == 1 || x == 2)
            return factory.createOperand(x, std::to_string( std::stoi(this->_StrValue) / std::stoi(rhs.toString())));
        else if (x == 3)
            return factory.createOperand(x, std::to_string( std::stof(this->_StrValue) / std::stof(rhs.toString())));
        else if (x == 4)
            return factory.createOperand(x, std::to_string( std::stod(this->_StrValue) / std::stod(rhs.toString())));
    }
    else if (x < y)
    {
        if (y == 0 || y == 1 || y == 2)
            return factory.createOperand(y, std::to_string( std::stoi(this->_StrValue) / std::stoi(rhs.toString())));
        else if (y == 3)
            return factory.createOperand(y, std::to_string( std::stof(this->_StrValue) / std::stof(rhs.toString())));
        else if (y == 4)
            return factory.createOperand(y, std::to_string( std::stod(this->_StrValue) / std::stod(rhs.toString())));
    }
    else if (x > y)
    {
        if (x == 0 || x == 1 || x == 2)
            return factory.createOperand(x, std::to_string( std::stoi(this->_StrValue) / std::stoi(rhs.toString())));
        else if (x == 3)
            return factory.createOperand(x, std::to_string( std::stof(this->_StrValue) / std::stof(rhs.toString())));
        else if (x == 4)
            return factory.createOperand(x, std::to_string( std::stod(this->_StrValue) / std::stod(rhs.toString())));
    }
    return NULL;
} // Quotient

template <typename T>
IOperand const * operand<T>::operator%( IOperand const & rhs ) const
{
    Factory factory;
    auto x = this->getType();
    auto y = rhs.getType();
    
    if (x == y)
    {
        if (x == 0 || x == 1 || x == 2)
            return factory.createOperand(x, std::to_string( std::stoi(this->_StrValue) % std::stoi(rhs.toString())));
        else
            throw Executer::ExecuterException("module is not int");
    }
    else if (x < y)
    {
        if (y == 0 || y == 1 || y == 2)
            return factory.createOperand(y, std::to_string( std::stoi(this->_StrValue) % std::stoi(rhs.toString())));
        else
            throw Executer::ExecuterException("module is not int");
    }
    else if (x > y)
    {
        if (x == 0 || x == 1 || x == 2)
            return factory.createOperand(x, std::to_string( std::stoi(this->_StrValue) % std::stoi(rhs.toString())));
        else
            throw Executer::ExecuterException("module is not int");
    }
    return NULL;
} // Modulo

template <typename T>
operand<T>::operandException::operandException () noexcept
{

}

template <typename T>
operand<T>::operandException::operandException (std::string its_message) noexcept:
_Message(its_message)
{

}

template <typename T>
operand<T>::operandException::operandException (const operandException & rhs ) noexcept
{
    *this = rhs;
}

template <typename T>
typename operand<T>::operandException & operand<T>::operandException::operator = (const operandException & rhs ) noexcept
{
    if (this == &rhs)
        return *this;
    this->_Message = rhs.GetMessage();
    return *this;
}

template <typename T>
operand<T>::operandException::~operandException()
{

}

template <typename T>
const std::string operand<T>::operandException::GetMessage() const
{
    return _Message;
}

#endif