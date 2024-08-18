#ifndef SUM_EXEC
#define SUM_EXEC
#include <string>

template <typename T>
T sum_all_digit(const std::string& str)
{
    T sum = 0;
    for (const char& ch : str)
    {
        if (isdigit(ch))
        {
            sum += ch - '0';
        }
    }
    return sum;
}

#endif