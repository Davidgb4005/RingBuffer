#include "RingBuffer.hpp"
#include <iostream>
#include <string>
RingBuffer buffer(850);
void printer()
{
    uint8_t *ptr = nullptr;
    for (int i = 0; i < 511; i++)
    {
        std::cout << (buffer.start_ptr + i);
    }
    std::cout << std::endl;
}
int main()
{

    struct __attribute__((packed)) MotorDataTelegram : Telegram
    {
        int speed;
        int direction;
        bool active;
        char mode;
        int time;
        MotorDataTelegram()
        {
            type = 78;
            priority = 99;                                                    // Remove Null Termination Char
            len_msb = static_cast<uint8_t>(sizeof(MotorDataTelegram)) >> 8;   // Add Length for Base Telegram info
            len_lsb = static_cast<uint8_t>(sizeof(MotorDataTelegram)) & 0xff; // Add Length for Base Telegram info
        }
    };

    MotorDataTelegram motordata;
    motordata.active = false;
    motordata.direction = 423;
    motordata.speed = 1231;
    motordata.time = 432;
    motordata.mode = 'c';
    MotorDataTelegram motordata2;
    motordata2.active = false;
    motordata2.direction = 423;
    motordata2.speed = 1231;
    motordata2.time = 432;
    motordata2.mode = 'b';
    char test[] = "This is A message";
    char word1[] = {'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r'};
    char word2[11] = {0, 9, 6, 78, 't', 'r', 'a', 'i', 'n', 0, 9};
    char word3[7] = {6, 78, 't', 'r', 'a', 'i', 'n'};
    // char word4[5] = {'l', 'i', 'g', 'h', 't'};
    StringTelegram msg;
    CharArrayTelegram CharArray;
    MotorDataTelegram motortest;
    int offset = 0;
    msg.Set(test, sizeof(test));
    CharArray.Set(reinterpret_cast<uint8_t *>(word1), sizeof(word1));
    int len = 0;
    // offset = buffer.Write(&msg);
    len = buffer.Read(&motortest);
#if 1
    offset = buffer.Write(&CharArray);
    offset = buffer.Write(&motordata);
    offset = buffer.Write(&motordata2);
    // offset = buffer.WriteRaw(reinterpret_cast<uint8_t *>(word2), 11, 0);
    // offset = buffer.WriteRaw(reinterpret_cast<uint8_t *>(word3), 7, 0);
    char bufz[1000];
    buffer.PrintData();
    std::cout<<"DATA ENDS HERE"<<std::endl;
    len = buffer.Read(&bufz);

    offset = buffer.Write(&motordata);
    offset = buffer.Write(&motordata2);

    buffer.PrintData();
#endif
    return 1;
}