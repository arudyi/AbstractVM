#include "../headers/AbstractVM.h"
#include <istream>

int check_leaks(int ac, char**av)
{
    Executer executer;
    if (ac != 1)
    {
        while (--ac >= 1)
        {
            std::ifstream fs;
            fs.open(av[ac], std::fstream::in);
            if (fs.is_open())
            {
                try
                {
                    executer.parsProgramm(fs, true);
                }
                catch(Executer::ExecuterException &e)
                {
                    std::cout << e.GetMessage() << std::endl;
                }
                fs.close();
            }
            else
                std::cout << "cannot open file " << av[ac] << std::endl;
        }
    }
    else
    {
        try
        {
            std::string buf;
            std::stringstream stream;
            while (std::getline(std::cin, buf, '\n'))
            {
                if (!std::cin.good())
                    throw Executer::ExecuterException("Error EOF");
                stream << buf << '\n';
                if (buf.find(";;") != std::string::npos)
                {
                    stream << EOF;
                    break ;
                }
            }
            executer.parsProgramm(stream, false);
        }
        catch(Executer::ExecuterException &error)
        {
            std::cout << error.GetMessage() << std::endl;
        }
    }
    //system("leaks avm");
    return (0);
}

int * k()
{
    return new int(42);
}

int main(int ac, char**av)
{
    check_leaks(ac, av);
    return (0);
}