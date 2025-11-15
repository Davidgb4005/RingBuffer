#include "RingBuffer.hpp"
#include <iostream>
#include <string>
#include <cstring>
RingBuffer ring(500);
int main()
{


    struct MotorData : RingBuffer::TelegramStruct
    {
        char len = sizeof(MotorData);
        char struct_type = 1;
        uint speed;
        uint position;
        uint direction;
    };
    struct ButtonData : RingBuffer::TelegramStruct
    {
        char len = sizeof(ButtonData);
        char struct_type = 2;
        bool active;
    };
    std::string input;
    ButtonData button;
    button.active = true;
    MotorData motor;
    motor.speed = 2271560481;
    motor.position = 2271560481;
    motor.direction = 2271560481;

    char test[] = "1234567890";
    char test2[] = {'T', 'h', 'i', 's', ' ', 'i', 's'};
    ring.WriteStruct(&motor);
    ring.WriteStruct(&button);
    // WriteString(test, sizeof(test));
    // WriteChars(test2, sizeof(test2));

    // ring.PrintData();
    RingBuffer::DefaultBuffer buffer;
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
