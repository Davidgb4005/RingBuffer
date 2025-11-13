#include "RingBuffer.hpp"
#include <iostream>
#include <string>

int main()
{
    RingBuffer ring(120);
    std::string input;
    char buffer[256];

    char message[] = {"This is a test"};
    ring.WriteData(message, 14);
    ring.PrintData();
    if (ring.DataAvailible())
        int len = ring.ReadData(buffer);
    
    return 0;
}
