#include "../src/RingBuffer.hpp"
#include <iostream>
#include <string>

RingBuffer buffer(256);
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
int main()
{

    char String[] = "This is a Test string";
    char array[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
    StringTelegram str;
    CharArrayTelegram chr;
    chr.Set((uint8_t *)array, 11, 77);
    str.Set(String, sizeof(String), 89);
    // Write several alternating messages
    buffer.Write(&str);
    buffer.Write(&chr);
    int8_t type = buffer.GetType();
    StringTelegram buffer1;
    char temp_buffer[256];
    uint16_t len;
    len = buffer.Read(&buffer1, temp_buffer);
    //buffer.PrintData();
    for (int i = 0; i < len; i++)
    {
        std::cout<<buffer1.data[i]<<std::endl;
    }

    type = buffer.GetType();
    int adr = buffer.GetAdr();
    CharArrayTelegram buffer2;
    len = buffer.Read(&buffer2, temp_buffer);
    for (int i = 0; i < len; i++)
    {
        if(buffer2.data[i] == array[i]){
            std::cout<<"Correct"<<std::endl;
        }
    }

#if 0

    offset = buffer.Write(&Motor);
    offset = buffer.Write(&Encoder);
    offset = buffer.Write(&Motor);
    offset = buffer.Write(&Encoder);
    offset = buffer.Write(&Encoder);
    offset = buffer.Write(&Motor);
    offset = buffer.Write(&Encoder);
    offset = buffer.Write(&Encoder);
    offset = buffer.Write(&Motor);
    offset = buffer.Write(&Encoder);
    offset = buffer.Write(&Encoder);
    int16_t bytes_remaining = 0;
    while (!buffer.BufferFull() && !(bytes_remaining < 0))
    {
        bytes_remaining = buffer.Write(&Motor);
        bytes_remaining = buffer.Write(&Encoder);
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