#include "RingBuffer.hpp"
#include <iostream>
#include <string>

int main()
{
    RingBuffer ring(120);
    std::string input;
    char buffer[256];

    char test[] = "Hello3242";

    int rep = ring.WriteData(test,5);
    std::cout << rep << std::endl;

    ring.PrintData();
    if (ring.DataAvailible())
    {
        int len = ring.ReadData(buffer);
        std::cout<<len<<std::endl;
        ring.PrintMsg(buffer,len);
    }
    return 0;
}
