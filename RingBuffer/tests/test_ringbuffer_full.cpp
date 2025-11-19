#include <gtest/gtest.h>
#include "../src/RingBuffer.hpp"

// Your enums
enum MessageType
{
    STEPPER_MOTOR = 45,
    ENCODER = 34,
    CLAMP = 78
};

// Your Telegram Classes
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
        address = 88;
        len_msb = sizeof(StepperMotorTelegram) >> 8;
        len_lsb = sizeof(StepperMotorTelegram) & 0xFF;
    }
};

struct  EncoderTelegram : Telegram
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
        address = 76;
        len_msb = sizeof(EncoderTelegram) >> 8;
        len_lsb = sizeof(EncoderTelegram) & 0xFF;
    }
};

// ===========================================================
// Test: Write several Stepper + Encoder messages & read them
// ===========================================================
#if 0
TEST(RingBufferTests, WriteAndReadMultipleMessages)
{
    RingBuffer buffer(256);

    StepperMotorTelegram m1;
    m1.speed = 1231;
    m1.direction = 423;
    m1.active = false;
    m1.time = 432;
    m1.mode = 'c';

    EncoderTelegram e1;
    e1.speed = 2;
    e1.direction = 1;
    e1.steps = 5;
    e1.time = 3;
    e1.active = false;
    e1.mode = '4';

    char String[] = "This is a Test string";
    char array[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
    StringTelegram str;
    CharArrayTelegram chr;
    chr.Set((uint8_t *)array, 11, 77);
    str.Set(String, sizeof(String), 89);
    EXPECT_EQ(buffer.Write(&m1), 0);
    EXPECT_EQ(buffer.Write(&e1), 0);
    EXPECT_EQ(buffer.Write(&str), 0);
    EXPECT_EQ(buffer.Write(&chr), 0);
    EXPECT_EQ(buffer.MessageAvailible(), 4);

    // -------- Read first: StepperMotor --------
    EXPECT_EQ(buffer.GetType(), STEPPER_MOTOR);
    StepperMotorTelegram rm1;
    EXPECT_EQ(buffer.Read(&rm1), 18);
    EXPECT_EQ(rm1.type, STEPPER_MOTOR);
    EXPECT_EQ(rm1.address, 88);
    EXPECT_EQ(rm1.mode, 'c');
    EXPECT_EQ(rm1.speed, 1231);
    EXPECT_EQ(rm1.time, 432);
    EXPECT_EQ(rm1.direction, 423);
    EXPECT_EQ(buffer.MessageAvailible(), 3);

    // -------- Read second: Encoder --------
    EXPECT_EQ(buffer.GetType(), ENCODER);
    EncoderTelegram re1;
    EXPECT_EQ(buffer.Read(&re1), 22);
    EXPECT_EQ(re1.type, ENCODER);
    EXPECT_EQ(re1.address, 76);
    EXPECT_EQ(re1.mode, '4');
    EXPECT_EQ(re1.speed, 2);
    EXPECT_EQ(re1.time, 3);
    EXPECT_EQ(re1.steps, 5);
    EXPECT_EQ(re1.direction, 1);
    EXPECT_EQ(buffer.MessageAvailible(), 2);

    // -------- Read Third String --------
    EXPECT_EQ(buffer.GetType(), 1);
    StringTelegram buffer1;
    char *temp_buffer;
    int len;
    EXPECT_GT(len = buffer.Read(&buffer1, temp_buffer), 0);
    for (int i = 0; i < len; i++)
    {
        EXPECT_EQ(buffer1.data[i], String[i]);
    }
    EXPECT_EQ(buffer.MessageAvailible(), 1);

    // -------- Read Fourth Char Array --------
    EXPECT_EQ(buffer.GetType(), 2);
    CharArrayTelegram buffer2;
    EXPECT_GT(len = buffer.Read(&buffer2, temp_buffer), 0);
    for (int i = 0; i < len; i++)
    {
        EXPECT_EQ(buffer2.data[i], array[i]);
    }
    chr.Set((uint8_t *)array, 11, 77);
    str.Set(String, sizeof(String), 89);
    // Buffer should be empty
    EXPECT_EQ(buffer.MessageAvailible(), 0);
}
#endif
// ===========================================================
// Test: Buffer should fill and stop writing
// ===========================================================
TEST(RingBufferTests, BufferFillsCorrectly)
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
    while (result != STRUCT_LARGER_THAN_BUFFER)
    {
        if (flip_flop % 2 == 0)
        {
            result = buffer.Write(&stp_in);
            stp_in.speed += 10;
            stp_in.direction--;
            stp_in.active = !stp_in.active;
            stp_in.time++;
            stp_in.mode++;
        }
        else if (flip_flop % 3 == 0)
        {
            result = buffer.Write(&enc_in);
            enc_in.speed++;
            enc_in.direction--;
            enc_in.active = !enc_in.active ;
            enc_in.time--;
            enc_in.mode++;
            enc_in.steps -= 20;
        }
        else if (flip_flop % 5 == 0)
        {
            result = buffer.Write(&str_in);
        }
        else
        {
            result = buffer.Write(&chr_in);
        }
        flip_flop++;
    }

    StringTelegram str_out;
    CharArrayTelegram chr_out;
    EncoderTelegram enc_out;
    StepperMotorTelegram stp_out;
    char char_temp_buffer[256];
    char str_temp_buffer[256];
    flip_flop = 0;
    while (buffer.MessageAvailible() > 0)
    {
        int len = 0;
        switch (buffer.GetType())
        {
        case STEPPER_MOTOR:
            std::cout << "Motor" << std::endl;
            EXPECT_EQ(result = buffer.Read(&stp_out), sizeof(StepperMotorTelegram));

            break;
        case ENCODER:
            std::cout << "Enc" << std::endl;
            EXPECT_EQ(result = buffer.Read(&enc_out), sizeof(EncoderTelegram));
            break;
        case 1:
        {
            std::cout << "STR" << std::endl;
            EXPECT_GT(len = buffer.Read(&str_out, str_temp_buffer), 0);
            break;
        }
        case 2:
        {
            std::cout << "CHR" << std::endl;
            EXPECT_GT(len = buffer.Read(&chr_out, char_temp_buffer), 0);
            break;
        }
        }
        if (flip_flop % 2 == 0)
        {
            EXPECT_EQ(stp_out.active, stp_in_base.active);
            EXPECT_EQ(stp_out.address, stp_in_base.address);
            EXPECT_EQ(stp_out.direction, stp_in_base.direction);
            EXPECT_EQ(stp_out.len_lsb, stp_in_base.len_lsb);
            EXPECT_EQ(stp_out.len_msb, stp_in_base.len_msb);
            EXPECT_EQ(stp_out.mode, stp_in_base.mode);
            EXPECT_EQ(stp_out.speed, stp_in_base.speed);
            EXPECT_EQ(stp_out.time, stp_in_base.time);
            EXPECT_EQ(stp_out.type, stp_in_base.type);
            stp_in_base.speed += 10;
            stp_in_base.direction--;
            stp_in_base.active = !stp_in_base.active;
            stp_in_base.time++;
            stp_in_base.mode++;
        }
        else if (flip_flop % 3 == 0)
        {

            EXPECT_EQ(enc_out.active, enc_in_base.active);
            EXPECT_EQ(enc_out.address, enc_in_base.address);
            EXPECT_EQ(enc_out.direction, enc_in_base.direction);
            EXPECT_EQ(enc_out.len_lsb, enc_in_base.len_lsb);
            EXPECT_EQ(enc_out.len_msb, enc_in_base.len_msb);
            EXPECT_EQ(enc_out.mode, enc_in_base.mode);
            EXPECT_EQ(enc_out.speed, enc_in_base.speed);
            EXPECT_EQ(enc_out.time, enc_in_base.time);
            EXPECT_EQ(enc_out.type, enc_in_base.type);
            enc_in_base.speed++;
            enc_in_base.direction--;
            enc_in_base.active = !enc_in_base.active ;
            enc_in_base.time--;
            enc_in_base.mode++;
            enc_in_base.steps -= 20;
        }
        else if (flip_flop % 5 == 0)
        {
            EXPECT_EQ(str_out.address, str_in.address);
            EXPECT_EQ(str_out.type, str_in.type);
            EXPECT_EQ(str_out.len_lsb, str_in.len_lsb);
            EXPECT_EQ(str_out.len_msb, str_in.len_msb);
            for (int i = 0; i < len; i++)
            {
                EXPECT_EQ(str_out.data[i], String[i]);
            }
        }
        else
        {

            EXPECT_EQ(chr_out.address, chr_in.address);
            EXPECT_EQ(chr_out.type, chr_in.type);
            EXPECT_EQ(chr_out.len_lsb, chr_in.len_lsb);
            EXPECT_EQ(chr_out.len_msb, chr_in.len_msb);
            for (int i = 0; i < len; i++)
            {
                EXPECT_EQ(chr_out.data[i], array[i]);
            }
        }
        flip_flop++;
    }
    // buffer.PrintData();
    EXPECT_EQ(buffer.MessageAvailible(), 0);
}

// ===========================================================
// Test: Reading empty buffer gives NO_MESSAGES
// ===========================================================
#if 1
TEST(RingBufferTests, ReadEmpty)
{
    RingBuffer buffer(64);
    StepperMotorTelegram t;
    EXPECT_EQ((int16_t)buffer.Read(&t), NO_MESSAGES);
}
#endif
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
