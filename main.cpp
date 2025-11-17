#include "RingBuffer.hpp"
#include <iostream>
#include <string>
RingBuffer buffer(850);
enum MessageType
{
    STEPPER_MOTOR = 45,
    ENCODER = 34,
    CLAMP = 78

};
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

    struct __attribute__((packed)) StepperMotorTelegram : Telegram
    {
        int speed;
        int direction;
        bool active;
        char mode;
        int time;
        StepperMotorTelegram()
        {
            type = STEPPER_MOTOR;
            address = 88;                                                        // Remove Null Termination Char
            len_msb = static_cast<uint8_t>(sizeof(StepperMotorTelegram)) >> 8;   // Add Length for Base Telegram info
            len_lsb = static_cast<uint8_t>(sizeof(StepperMotorTelegram)) & 0xff; // Add Length for Base Telegram info
        }
    };
    struct __attribute__((packed)) EncoderTelegram : Telegram
    {
        int speed;
        int direction;
        bool active;
        char mode;
        int time;
        int steps;
        EncoderTelegram()
        {
            type = ENCODER;
            address = 76;                                                   // Remove Null Termination Char
            len_msb = static_cast<uint8_t>(sizeof(EncoderTelegram)) >> 8;   // Add Length for Base Telegram info
            len_lsb = static_cast<uint8_t>(sizeof(EncoderTelegram)) & 0xff; // Add Length for Base Telegram info
        }
    };
    StepperMotorTelegram motordata;
    motordata.active = false;
    motordata.direction = 423;
    motordata.speed = 1231;
    motordata.time = 432;
    motordata.mode = 'c';
    EncoderTelegram motordata2;
    motordata2.active = false;
    motordata2.direction = -234;
    motordata2.speed = 47657;
    motordata2.time = 43255;
    motordata2.mode = 'b';
    motordata2.steps = 5000;
    char test[] = "This is A message";
    char word1[] = {'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r', 'a', 'p', 'p', 'l', 'e', 'r'};
    char word2[11] = {0, 9, 6, 78, 't', 'r', 'a', 'i', 'n', 0, 9};
    char word3[7] = {6, 78, 't', 'r', 'a', 'i', 'n'};
    // char word4[5] = {'l', 'i', 'g', 'h', 't'};
    StringTelegram msg;
    CharArrayTelegram CharArray;
    int offset = 0;
    msg.Set(test, sizeof(test), 255);
    CharArray.Set(reinterpret_cast<uint8_t *>(word1), sizeof(word1), 255);
    int len = 0;
    // offset = buffer.Write(&msg);
#if 1
    // offset = buffer.Write(&CharArray);
    offset = buffer.Write(&motordata);
    offset = buffer.Write(&motordata2);
    // offset = buffer.WriteRaw(reinterpret_cast<uint8_t *>(word2), 11, 0);
    // offset = buffer.WriteRaw(reinterpret_cast<uint8_t *>(word3), 7, 0);
    char bufz[1000];
    // buffer.PrintData();

    offset = buffer.Write(&motordata);
    offset = buffer.Write(&motordata2);
    offset = buffer.Write(&motordata2);
    offset = buffer.Write(&motordata);
    offset = buffer.Write(&motordata2);
    offset = buffer.Write(&motordata2);
    offset = buffer.Write(&motordata);
    offset = buffer.Write(&motordata2);
    offset = buffer.Write(&motordata2);
    int16_t bytes_remaining = 0;
    while (!buffer.BufferFull() && !(bytes_remaining < 0))
    {
        bytes_remaining = buffer.Write(&motordata);
        bytes_remaining = buffer.Write(&motordata2);
        std::cout << "Buffer Full" << buffer.BytesAvailible() << std::endl;
    }

    while (buffer.MessageAvailible()>0)
    {
        switch (buffer.GetType())
        {
        case STEPPER_MOTOR:
        {
            StepperMotorTelegram motortest;
            len = buffer.Read(&motortest);
            std::cout << motortest.type << "  ";
            std::cout << motortest.address << "  ";
            std::cout << motortest.mode << "  ";
            std::cout << motortest.speed << "  ";
            std::cout << motortest.time << "  ";
            std::cout << motortest.direction << std::endl;
            break;
        }

        case ENCODER:
        {
            EncoderTelegram encodertest;
            len = buffer.Read(&encodertest);
            std::cout << encodertest.type << "  ";
            std::cout << encodertest.address << "  ";
            std::cout << encodertest.mode << "  ";
            std::cout << encodertest.speed << "  ";
            std::cout << encodertest.time << "  ";
            std::cout << encodertest.steps << "  ";
            std::cout << encodertest.direction << std::endl;
            break;
        }
        case CLAMP:

        default:
            std::cout << buffer.GetType() << std::endl;
        };
    }

    // buffer.PrintData();
#endif
    return 1;
}