#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <vector>
#include <exception>
#include "IOperand.h"
#include "eOperandType.h"

class Factory
{
    private:
        IOperand const * createInt8( std::string const & value ) const;
        IOperand const * createInt16( std::string const & value ) const;
        IOperand const * createInt32( std::string const & value ) const;
        IOperand const * createFloat( std::string const & value ) const;
        IOperand const * createDouble( std::string const & value ) const;
    public:
        Factory();
        ~Factory();
        Factory(const Factory & rhs);
        Factory & operator=(const Factory & rhs);
        IOperand const * createOperand( eOperandType type, std::string const & value ) const;
        typedef IOperand const *( Factory::*Ptr)( std::string const & value ) const;
        std::vector< Ptr > ArrayPtr;
    class FactoryException : public std::exception
    {
        private:
            std::string _Message;
        public:
            FactoryException () noexcept;
            FactoryException (std::string its_message) noexcept;
            FactoryException (const exception & rhs ) noexcept;
            FactoryException & operator = (const exception & rhs ) noexcept;
            virtual ~FactoryException();
            const std::string GetMessage() const;
    };
};
#endif