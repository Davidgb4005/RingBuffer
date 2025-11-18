#include "RingBuffer.hpp"
#include <iostream>
RingBuffer::RingBuffer(uint16_t buffer_size)
{
    this->buffer_size = buffer_size;

    start_ptr = new uint8_t[buffer_size];
    write_ptr = start_ptr;
    read_ptr = start_ptr;
    end_ptr = start_ptr + buffer_size - 1;
}
RingBuffer::~RingBuffer()
{
    delete[] start_ptr;
}
uint16_t RingBuffer::MessageAvailible()
{
    return messages_availible;
}
uint16_t RingBuffer::BytesAvailible()
{
    return bytes_availible;
}
bool RingBuffer::BufferFull()
{

    return buffer_full;
}

uint16_t RingBuffer::Read(void *data)
{
    if (messages_availible > 0)
    {
        size_t i = 0;
        int16_t err = 0;
        uint8_t *data_ptr = reinterpret_cast<uint8_t *>(data);
        *data_ptr = *read_ptr;
        uint16_t length = 0;

        length = *data_ptr << 8;
        err = AdvanceReadPtr();
        if (err < 0)
            return (err);
        *(data_ptr + 1) = *read_ptr;
        length += (*read_ptr) & 0xff;
        err = AdvanceReadPtr();
        if (err < 0)
            return (err);
        for (i = 2; i < length; i++)
        {
            *(data_ptr + i) = *read_ptr;
            err = AdvanceReadPtr();
            if (err < 0)
                return (err);
        }
        messages_availible--;
        return i;
    }
    else
        return NO_MESSAGES;
}
uint16_t RingBuffer::Write(Telegram *data)
{
    int16_t size = (data->len_msb << 8) + (data->len_lsb & 0xff);
    if (size > (buffer_size - bytes_availible))
    {
        return STRUCT_LARGER_THAN_BUFFER;
    }
    bytes_remaining = size;

    uint8_t *data_ptr = reinterpret_cast<uint8_t *>(data);
    size_t i = 0;

    for (i = 0; i < size; i++)
    {
        *write_ptr = *(data_ptr + i);
        int16_t err = AdvanceWritePtr();
        if (err < 0)
            return (err);
    }
    messages_availible++;
    return i - size;
}
uint16_t RingBuffer::Write(StringTelegram *data)
{
    int16_t size = (data->len_msb << 8) + (data->len_lsb & 0xff);
    if (size > (buffer_size - bytes_availible))
    {
        return STRING_LARGER_THAN_BUFFER;
    }
    bytes_remaining = size;

    uint8_t offset = reinterpret_cast<uint8_t *>(&data->data) - reinterpret_cast<uint8_t *>(data);
    uint8_t *data_ptr = reinterpret_cast<uint8_t *>(data);
    size_t i = 0;
    for (i = 0; i < size; i++)
    {
        if (i < offset)
        {
            *write_ptr = *(data_ptr + i);
            int16_t err = AdvanceWritePtr();
            if (err < 0)
                return (err);
        }
        else
        {
            *write_ptr = data->data[i - offset];
            int16_t err = AdvanceWritePtr();
            if (err < 0)
                return (err);
        }
    }
    messages_availible++;
    return i - size;
}
uint16_t RingBuffer::Write(CharArrayTelegram *data)

{
    int16_t size = (data->len_msb << 8) + (data->len_lsb & 0xff);
    if (size > (buffer_size - bytes_availible))
    {
        return CHAR_ARRAY_LARGER_THAN_BUFFER;
    }

    bytes_remaining = size;
    uint8_t offset = reinterpret_cast<uint8_t *>(&data->data) - reinterpret_cast<uint8_t *>(data);
    uint8_t *data_ptr = reinterpret_cast<uint8_t *>(data);
    size_t i = 0;
    for (i = 0; i < size; i++)
    {
        if (i < offset)
        {
            *write_ptr = *(data_ptr + i);
            int16_t err = AdvanceWritePtr();
            if (err < 0)
                return (err);
        }
        else
        {
            *write_ptr = data->data[i - offset];
            int16_t err = AdvanceWritePtr();
            if (err < 0)
                return (err);
        }
    }
    messages_availible++;
    return i - size;
}
uint16_t RingBuffer::WriteRaw(uint8_t *data, uint16_t len, uint16_t offset)

{
    if (len < 2)
    {
        return DATA_TO_SHORT;
    }

    uint8_t *data_ptr = data;
    size_t i = offset;
    for (i; i < len; i++)
    {
        if (buffer_full)
        {
            return i - len;
        }
        if (bytes_remaining == 0 && message_complete)
        {
            bytes_remaining_msb = data[i] << 8;
            message_complete = false;
        }
        else if (bytes_remaining == -1)
        {
            bytes_remaining_lsb = data[i] & 0xff;
            bytes_remaining += bytes_remaining_lsb + bytes_remaining_msb;
            bytes_remaining_lsb = 0;
            bytes_remaining_msb = 0;
        }
        *write_ptr = *(data_ptr + i);
        int16_t err = AdvanceWritePtr();
        if (err < 0)
        {
            // If You Enable The Return Line It Will Return The ErrorCode For Debugging Not The Remaining Bytes
            // It Will Break The Ability For the Function To Process Partial Messages BEWARE
            // return (err);
        }
        if (bytes_remaining == 0)
        {
            messages_availible++;
            message_complete = true;
        }
    }
    return i - len;
}
int16_t RingBuffer::GetAdr()
{
    if (messages_availible < 1)
    {
        return NO_MESSAGES;
    }
    else
    {
        uint8_t *temp_ptr = read_ptr;
        for (int i = 0; i < 3; i++)
        {
            int16_t err = AdvanceTempReadPtr(temp_ptr);
            if (err < 0)
                return (err);
        }
        return *temp_ptr;
    }
}
int16_t RingBuffer::GetType()
{
    if (messages_availible < 1)
    {
        return NO_MESSAGES;
    }
    else
    {
        uint8_t *temp_ptr = read_ptr;
        for (int i = 0; i < 2; i++)
        {
            int16_t err = AdvanceTempReadPtr(temp_ptr);
            if (err < 0)
                return (err);
        }
        return *temp_ptr;
    }
    return UNEXPECTED_ERROR_GetType;
}
int16_t RingBuffer::AdvanceWritePtr()
{
    if (write_ptr + 1 == read_ptr || (write_ptr == end_ptr && read_ptr == start_ptr))
    {
        buffer_full = true;
        return BUFFER_FULL;
    }
    else if (write_ptr == end_ptr)
    {
        buffer_empty = false;
        write_ptr = start_ptr;
        bytes_availible++;
        bytes_remaining--;
        return 0;
    }
    else
    {
        buffer_empty = false;
        write_ptr++;
        bytes_availible++;
        bytes_remaining--;
        return 0;
    }
    return UNEXPECTED_ERROR_AdvanceWritePtr;
}

int16_t RingBuffer::AdvanceReadPtr()
{
    if (read_ptr == write_ptr)
    {
        buffer_empty = true;
        return BUFFER_EMPTY;
    }
    else if (read_ptr == end_ptr)
    {
        buffer_full = false;
        read_ptr = start_ptr;
        bytes_availible--;
        return 0;
    }
    else
    {
        buffer_full = false;
        read_ptr++;
        bytes_availible--;
        return 0;
    }
    return UNEXPECTED_ERROR_AdvanceReadPtr;
}
int16_t RingBuffer::AdvanceTempReadPtr(uint8_t *&temp_ptr)
{

    if (temp_ptr == end_ptr)
    {
        temp_ptr = start_ptr;
        return 0;
    }
    else
    {
        temp_ptr++;
        return 0;
    }
    return UNEXPECTED_ERROR_AdvanceTempReadPtr;
}
void RingBuffer::PrintData()
{
#if 1 // Enables Console Output For Debugging
    uint8_t *temp_read_ptr = read_ptr;
    if (bytes_availible < 1)
    {
        std::cout << "No Data" << std::endl;
        return;
    }
    uint8_t c;
    uint16_t temp_bytes_availibe = bytes_availible;
    while (temp_bytes_availibe > 0)
    {
        int16_t len = (*temp_read_ptr << 8) + ((*(temp_read_ptr + 1)) & 0xff);

        if (len > temp_bytes_availibe)
        {
            std::cout << "Data Fragment(" << (temp_bytes_availibe) << "/" << static_cast<int>(len) << ") : ";
        }
        else
        {
            std::cout << "Data Length(" << len << ") : ";
        }
        std::cout << "Struct Type(" << static_cast<int>(*(temp_read_ptr + 2)) << ") : ";
        std::cout << "Priority(" << static_cast<int>(*(temp_read_ptr + 3)) << ") : ";
        for (int k = 0; k < len; ++k)
        {
            c = *temp_read_ptr;
            if (false) // Debugging Causes output to be in Int instead of char
                std::cout << static_cast<int>(c);
            else
                std::cout << c;
            if (temp_read_ptr == end_ptr)
            {
                temp_read_ptr = start_ptr;
                temp_bytes_availibe--;
            }
            else
            {
                temp_read_ptr++;
                temp_bytes_availibe--;
            }
            if (temp_read_ptr > end_ptr)
            {
                temp_read_ptr = start_ptr;
            }
        }
        std::cout << std::endl;
    }
#endif
}