#include <gtest/gtest.h>
#include "RingBuffer.hpp"

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

    // Write several alternating messages
    EXPECT_EQ(buffer.Write(&m1), 0);
    EXPECT_EQ(buffer.Write(&e1), 0);

    EXPECT_EQ(buffer.MessageAvailible(), 2);

    // -------- Read first: StepperMotor --------
    EXPECT_EQ(buffer.GetType(), STEPPER_MOTOR);
    StepperMotorTelegram rm1;
    EXPECT_GT(buffer.Read(&rm1), 0);

    EXPECT_EQ(rm1.type, STEPPER_MOTOR);
    EXPECT_EQ(rm1.address, 88);
    EXPECT_EQ(rm1.mode, 'c');
    EXPECT_EQ(rm1.speed, 1231);
    EXPECT_EQ(rm1.time, 432);
    EXPECT_EQ(rm1.direction, 423);

    // -------- Read second: Encoder --------
    EXPECT_EQ(buffer.GetType(), ENCODER);
    EncoderTelegram re1;
    EXPECT_GT(buffer.Read(&re1), 0);

    EXPECT_EQ(re1.type, ENCODER);
    EXPECT_EQ(re1.address, 76);
    EXPECT_EQ(re1.mode, '4');
    EXPECT_EQ(re1.speed, 2);
    EXPECT_EQ(re1.time, 3);
    EXPECT_EQ(re1.steps, 5);
    EXPECT_EQ(re1.direction, 1);
 
    // Only 2 messages left
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

    EXPECT_TRUE(buffer.BytesRemaining()< sizeof(StepperMotorTelegram));
    EXPECT_GT(buffer.MessageAvailible(), sizeof(buffer)/sizeof(StepperMotorTelegram));
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
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
