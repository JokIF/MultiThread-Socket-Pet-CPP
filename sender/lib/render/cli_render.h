#ifndef CLI_RENDERS__H
#define CLI_RENDERS__H
#include <lib/render/base_renders.h>
#include <lib/utils/reader.h>
#include <string>
#include <istream>
#include <stdexcept>


template <class RDR=CLIReader, typename T = std::string>
class CLIRender : public BaseRenderWithValidate<RDR, T> 
{
    public:
        T render(std::istream &stream)
        {
            std::string str = BaseRenderWithValidate<RDR, std::string>:: template render<std::istream&>(stream);
            
            std::sort(str.begin(), str.end(), std::greater<char>());

            for (size_t i = 0; i < str.size(); i++)
            {
                if ((str[i] - '0') % 2 == 0)
                {
                    str.erase(i, 1);
                    str.insert(i, "KB");
                    i++;
                }
            }

            return str;
        }
};

#endif