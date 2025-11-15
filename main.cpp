#include "RingBuffer.hpp"
#include <iostream>
#include <string>
#include <cstring>
RingBuffer ring(500);
void WriteString(char *buffer, int len)
{
    char temp_buffer[256];
    for (int i = 0; i < len - 1; i++)
    {
        temp_buffer[i + 1] = buffer[i];
    }
    temp_buffer[0] = len;
    ring.WriteData(temp_buffer);
}
void WriteChars(char *buffer, int len)
{
    char temp_buffer[256];
    for (int i = 0; i < len; i++)
    {
        temp_buffer[i + 1] = buffer[i];
    }
    temp_buffer[0] = len + 1;
    ring.WriteData(temp_buffer);
}
void WriteStruct(void *data)
{
    ring.WriteData(static_cast<char *>(data));
}
int main()
{
    std::string input;
    struct TelegramStruct
    {
    };
    struct DefaultBuffer : TelegramStruct
    {
        char len;
        char struct_type;
        char *data;
    };

    struct MotorData : TelegramStruct
    {
        char len = sizeof(MotorData);
        char struct_type = 1;
        uint speed;
        uint position;
        uint direction;
    };
    struct ButtonData : TelegramStruct
    {
        char len = sizeof(ButtonData);
        char struct_type = 2;
        bool active;
    };
    ButtonData button;
    button.active = true;
    MotorData motor;
    motor.speed = 2271560481;
    motor.position = 2271560481;
    motor.direction = 2271560481;

    char test[] = "1234567890";
    char test2[] = {'T', 'h', 'i', 's', ' ', 'i', 's'};
    WriteStruct(&motor);
    WriteStruct(&button);
    // WriteString(test, sizeof(test));
    // WriteChars(test2, sizeof(test2));

    // ring.PrintData();
    DefaultBuffer buffer;
    while (ring.DataAvailible())
    {
        int nxt = ring.GetMessageType();
        int len = ring.ReadData(&buffer);
        // std::cout << static_cast<int>(buffer.struct_type) << std::endl;
        if (nxt == 1)
        {
            MotorData mtr;
            std::memcpy(&mtr, &buffer, sizeof(mtr));
            std::cout << mtr.speed << std::endl;
            std::cout << mtr.position << std::endl;
            std::cout << mtr.direction << std::endl;
        }
        else if (nxt == 2)
        {
            ButtonData btn;
            std::memcpy(&btn, &buffer, sizeof(btn));
            std::cout << btn.active << std::endl;
        }
    }

    return 0;
}
