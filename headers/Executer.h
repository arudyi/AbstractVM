#ifndef EXECUTER_H
#define EXECUTER_H
#include <vector>
#include <memory>
#include "IOperand.h"
#include "Factory.h"

class Executer
{
    private:
        std::vector<std::shared_ptr<const IOperand> > _Array;
    public:
        Executer();
        ~Executer();
        Executer(const Executer &rhs);
        Executer & operator = ( const Executer &rhs );
        void parsProgramm(std::istream & readStream, bool file_mode);
        short parseInstruction(std::string & buf, std::string *Instruction);
        std::vector<std::string> split(const std::string & src, char delimetr);
        void push(const std::string &instruction, std::string & value);
        void pop();
        void dump(); //Displays each value of the stack
        void add();
        void sub();
        void mul();
        void div();
        void mod();
        void print();//Asserts that the value at the top of the stack is an 8-bit integer
        void assert(std::string type, std::string value);// Asserts that the value at the top of the stack is equal to the one passed as parameter for this instruction.        
        class ExecuterException : public std::exception
        {
            private:
                std::string _Message;
                ExecuterException () noexcept;
            public:
                ExecuterException (std::string its_message) noexcept;
                ExecuterException (const ExecuterException & rhs ) noexcept;
                ExecuterException & operator = (const ExecuterException & rhs ) noexcept;
                virtual ~ExecuterException();
                const std::string GetMessage() const;
        };
};

#endif