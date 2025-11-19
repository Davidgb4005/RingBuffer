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
struct StepperMotorTelegram : Telegram
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
struct EncoderTelegram : Telegram
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
    RingBuffer buffer(500);

    StepperMotorTelegram stp_in;
    stp_in.speed = 71;
    stp_in.direction = 72;
    stp_in.active = true;
    stp_in.time = 73;
    stp_in.mode = 'c';
    StepperMotorTelegram stp_in_base;
    stp_in_base.speed = stp_in.speed;
    stp_in_base.direction = stp_in.direction;
    stp_in_base.active = stp_in.active;
    stp_in_base.time = stp_in.time;
    stp_in_base.mode = stp_in.mode;
    EncoderTelegram enc_in;
    enc_in.speed = 121;
    enc_in.direction = -543;
    enc_in.active = true;
    enc_in.time = -0;
    enc_in.mode = 'R';
    enc_in.steps = -5555;
    EncoderTelegram enc_in_base;
    enc_in_base.speed = enc_in.speed;
    enc_in_base.direction = enc_in.direction;
    enc_in_base.active = enc_in.active;
    enc_in_base.time = enc_in.time;
    enc_in_base.mode = enc_in.mode;
    enc_in_base.steps = enc_in.steps;
    char String[] = "This is a Test string";
    char array[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
    StringTelegram str_in;
    CharArrayTelegram chr_in;
    chr_in.Set((uint8_t *)array, 11, 77);
    str_in.Set(String, sizeof(String), 89);

    // Keep writing until full
    int writes = 0;
    int16_t result = 0;
    int static flip_flop = 0;
    uint8_t bufferz[512];
    result = buffer.Write(&stp_in);
    buffer.PrintData();

    result = buffer.ReadRaw((uint8_t *)bufferz);
    for (int i = 0; i < result; i++)
        std::cout << bufferz[i];
    int ewr = 0;
}