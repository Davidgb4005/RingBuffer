#include <memory>
enum ErrorCodes
{
    DATA_TO_SHORT = -1,
    NO_MESSAGES = -2,
    BUFFER_FULL = -3
};
struct Telegram
{
    uint8_t len_msb = 0;
    uint8_t len_lsb = 0;
    uint8_t type = 0;
    uint8_t priority = 0;

};

struct __attribute__((packed)) StringTelegram : Telegram
{
    uint8_t *data = nullptr;
    void Set(const char *buffer, int length)
    {
        type = 1;
        priority = 111;
        length--;                                                               // Remove Null Termination Char
        len_msb = static_cast<uint8_t>((length + sizeof(StringTelegram) - 8) >> 8);   // Add Length for Base Telegram info
        len_lsb = static_cast<uint8_t>((length + sizeof(StringTelegram) - 8) & 0xff); // Add Length for Base Telegram info
        data = const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(buffer));
    }
};
struct __attribute__((packed)) CharArrayTelegram : Telegram
{
    uint8_t *data = nullptr;
    void Set(uint8_t *buffer, int length)
    {
        type = 2;
        priority = 31;                                                          // Remove Null Termination Char
        len_msb = static_cast<uint8_t>((length + sizeof(CharArrayTelegram) - sizeof(uintptr_t)) >> 8);   // Add Length for Base Telegram info
        len_lsb = static_cast<uint8_t>((length + sizeof(CharArrayTelegram) - sizeof(uintptr_t)) & 0xff); // Add Length for Base Telegram info
        data = buffer;
    }
};

class RingBuffer
{

private:
    bool message_complete = true;
    int16_t bytes_remaining = 0;
    int16_t bytes_remaining_msb = 0;
    int16_t bytes_remaining_lsb = 0;
    uint16_t bytes_availible = 0;
    uint16_t messages_availible = 0;
    uint16_t buffer_size = 0;
    bool buffer_full = false;
    bool buffer_empty = false;

public:
    RingBuffer(uint16_t buffer_size);
    ~RingBuffer();

    uint16_t Write(Telegram *data);
    uint16_t Write(StringTelegram *data);
    uint16_t Write(CharArrayTelegram *data);

    uint16_t WriteRaw(uint8_t *data, uint16_t len, uint16_t offset);
    uint16_t Read(void *data);
    void AdvanceWritePtr();
    void AdvanceReadPtr();
    void PrintData();

    uint8_t *write_ptr = nullptr;
    uint8_t *read_ptr = nullptr;
    uint8_t *start_ptr = nullptr;
    uint8_t *end_ptr = nullptr;
};
