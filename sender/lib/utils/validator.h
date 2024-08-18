#ifndef VALIDATOR__H
#define VALIDATOR__H
#include <string>
#include <vector>
#include <stdint.h>
#include <algorithm>

class ValidateError : public std::exception
{
    public:
        ValidateError(std::string msg) : message(msg) {};
        std::string what()
        {
            return message;
        }
    private:
        std::string message;
};


template <typename T>
class Validator
{
    public:
        virtual void set_data(T* data_) = 0;
        virtual void validate() = 0;
        virtual ~Validator() {};
    protected:
        T* data = nullptr;
};

class StringValidator : public Validator<std::string>
{
    public:
        void set_data(std::string* data_) override
        {
            data = data_;
        }
        virtual void validate() override = 0;
        virtual ~StringValidator() {};
};

class LenghtValidator : public StringValidator
{
    public:
        LenghtValidator(size_t lenght) : valid_lenght(lenght) {};
        void validate() override
        {
            if (data == nullptr || data->length() > valid_lenght)
                throw ValidateError("data lenght must be lower than " + std::to_string(valid_lenght) + ", but have " + std::to_string(data->length()));
        }
    private:
        uint32_t valid_lenght;
};

class IsDigitValidator : public StringValidator
{
    public:
        IsDigitValidator() {};
        void validate() override
        {
            if (data == nullptr || !std::all_of(data->begin(), data->end(), ::isdigit))
                throw ValidateError(std::string("data must be a number"));
        }
};

#endif 