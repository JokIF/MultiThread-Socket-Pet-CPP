#ifndef READER__H
#define READER__H
#include <string>
#include <istream>


template <typename T>
class BaseReader
{
    public:
        template <typename... Args>
        static T read(Args...) { return T(); };
};

class CLIReader : public BaseReader<std::string> 
{
    public:
        template <typename... Args>
        static std::string read(std::istream& stream, Args...)
        {
            std::string data;
            stream >> data;
            return data;
        }
};

#endif