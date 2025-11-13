#include "RingBuffer.hpp"
#include <iostream>
#include <string>

int main()
{
    RingBuffer ring(120);
    std::string input;
    char buffer[256];

    RingBuffer::Telegram message_1{
        20,
        "12345678901234567890",
        0
    };
    RingBuffer::Telegram message_2{
        0,
        new char[100],
        0
    };
    int rep = ring.WriteData(message_1);
    std::cout << rep << std::endl;

    ring.PrintData();
    if (ring.DataAvailible())
    {
        int len = ring.ReadData(message_2);
        ring.PrintMsg(message_2);
    }
    return 0;
}
