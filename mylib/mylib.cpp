#include "mylib.h"

using namespace std;

void my_func1(string& str)
{   
    if (str.empty())
    {
        return;
    }
    sort(str.begin(), str.end(), greater<char>());
    
    string new_str;
    new_str.reserve(str.length() * 1.5 + 1);

    for (int i = 0; i < str.length(); i++)
    {
        if (i % 2)
        {
            new_str += "KB";
            continue;
        }
        new_str += str[i];
    }
    str = move(new_str);
}

unsigned my_func2(string str)
{
    unsigned sum = 0;
    unsigned str_len = str.length();
    for (int i = 0; i < str_len; i++)
    {
        if (isdigit(str[i]))
            sum += (int)str[i] - (int)'0';
    }

    return sum;
}

bool my_func3(std::string str)
{
    if (str.length() > 2 && str.length() % 32 == 0)
        return true;
    return false;
}
