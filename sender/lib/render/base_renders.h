#ifndef BASE_RENDERS__H
#define BASE_RENDERS__H
#include <lib/utils/reader.h>
#include <lib/utils/validator.h>
#include <algorithm>
#include <string>
#include <vector>


template <class RDR, typename T>
class BaseRender 
{
    public:
        template <typename... Args>
        T render(Args... args)
        {
            T data = RDR::read(args...);
            return data;
        }
};

template <class RDR, typename T>
class BaseRenderWithValidate : public BaseRender<RDR, T>
{
    public:
        template <typename... Args>
        T render(Args... args)
        {
            T data = BaseRender<RDR, T>::template render<Args...>(args...);
            validate(data);
            return data;
        }
        ~BaseRenderWithValidate()
        {
            for_each(validators->begin(), validators->end(), [](Validator<T>* valid) {
                delete valid;
            });
            delete validators;
        }

        void add_validator(Validator<T>* valid)
        {
            validators->push_back(valid);
        }

    protected:
        std::vector<Validator<T>*>* validators = new std::vector<Validator<T>*>();

        void validate(T data)
        {
            for (Validator<T>* valid : *validators)
            {
                valid->set_data(&data);
                valid->validate();
            }
        }
};

#endif