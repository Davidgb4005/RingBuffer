#include "RingBuffer.hpp"
#include <iostream>
#include <string>
#include "gtest/gtest.h"
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
    int len = 0;
    StepperMotorTelegram Motor;
    Motor.active = false;
    Motor.direction = 423;
    Motor.speed = 1231;
    Motor.time = 432;
    Motor.mode = 'c';
    StepperMotorTelegram Motor2;
    Motor2.active = false;
    Motor2.direction = 423;
    Motor2.speed = 1231;
    Motor2.time = 432;
    Motor2.mode = 'c';
    EncoderTelegram Encoder;
    Encoder.active = false;
    Encoder.direction = 1;
    Encoder.speed = 2;
    Encoder.time = 3;
    Encoder.mode = '4';
    Encoder.steps = 5;
    EncoderTelegram Encoder2;
    Encoder2.active = false;
    Encoder2.direction = 6;
    Encoder2.speed = 7;
    Encoder2.time = 8;
    Encoder2.mode = '9';
    Encoder2.steps = 10;

    int offset = 0;

    RingBuffer buffer(128);

    StepperMotorTelegram msg;
    msg.speed = 1231;
    msg.direction = 423;
    msg.active = false;
    msg.time = 432;
    msg.mode = 'c';
    // Keep writing until full
    int writes = 0;
    int16_t result = 0;
    while (result != STRUCT_LARGER_THAN_BUFFER)
    {
        result = buffer.Write(&msg);
        std::cout << result << std::endl;
        if (result == STRUCT_LARGER_THAN_BUFFER)
            break;
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