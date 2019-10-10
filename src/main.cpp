#include "../headers/AbstractVM.h"

int main(int ac, char**av)
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
            executer.parsProgramm(std::cin, false);
        }
        catch(Executer::ExecuterException &error)
        {
            std::cout << error.GetMessage() << std::endl;
        }
    }
    return (0);
}