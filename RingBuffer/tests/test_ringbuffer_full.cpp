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
        address = 88;
        len_msb = sizeof(StepperMotorTelegram) >> 8;
        len_lsb = sizeof(StepperMotorTelegram) & 0xFF;
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
        address = 76;
        len_msb = sizeof(EncoderTelegram) >> 8;
        len_lsb = sizeof(EncoderTelegram) & 0xFF;
    }
};

// ===========================================================
// Test: Write several Stepper + Encoder messages & read them
// ===========================================================
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

    // Buffer should be empty
    EXPECT_EQ(buffer.MessageAvailible(), 0);
}
// ===========================================================
// Test: Buffer should fill and stop writing
// ===========================================================
#if 1
TEST(RingBufferTests, BufferFillsCorrectly)
{
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
        if (result == STRUCT_LARGER_THAN_BUFFER)
            break;
    }

    EXPECT_TRUE(buffer.BytesRemaining() < sizeof(StepperMotorTelegram));
    EXPECT_GT(buffer.MessageAvailible(), sizeof(buffer) / sizeof(StepperMotorTelegram));
}
#endif
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
