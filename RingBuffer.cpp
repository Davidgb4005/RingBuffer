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
uint16_t RingBuffer::Read(void *data)
{
    if (messages_availible > 0)
    {
        size_t i = 0;
        uint8_t *data_ptr = reinterpret_cast<uint8_t *>(data);
        *data_ptr = *read_ptr;
        uint16_t length = *data_ptr << 8;
        AdvanceReadPtr();
        *(data_ptr + 1) = *read_ptr;
        length += (*read_ptr) & 0xff;
        AdvanceReadPtr();
        for (i = 2; i < length; i++)
        {
            *(data_ptr + i) = *read_ptr;
            AdvanceReadPtr();
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
        buffer_full = true;
        return BUFFER_FULL;
    }
    bytes_remaining = size;
    uint8_t *data_ptr = reinterpret_cast<uint8_t *>(data);
    size_t i;
    for (i = 0; i < size; i++)
    {
        *write_ptr = *(data_ptr + i);
        AdvanceWritePtr();
    }
    messages_availible++;
    return i - size;
}
uint16_t RingBuffer::Write(StringTelegram *data)
{
    int16_t size = (data->len_msb << 8) + (data->len_lsb & 0xff);
    if (size > (buffer_size - bytes_availible))
    {
        std::cout << "BUFFER FULL" << std::endl;
        buffer_full = true;
        return BUFFER_FULL;
    }
    bytes_remaining = size;
    uint8_t offset = reinterpret_cast<uint8_t *>(&data->data) - reinterpret_cast<uint8_t *>(data);
    uint8_t *data_ptr = reinterpret_cast<uint8_t *>(data);
    size_t i;
    for (i = 0; i < size; i++)
    {
        if (i < offset)
        {
            *write_ptr = *(data_ptr + i);
            AdvanceWritePtr();
        }
        else
        {
            *write_ptr = data->data[i - offset];
            AdvanceWritePtr();
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
        std::cout << "BUFFER FULL" << std::endl;
        buffer_full = true;
        return BUFFER_FULL;
    }
    bytes_remaining = size;
    uint8_t offset = reinterpret_cast<uint8_t *>(&data->data) - reinterpret_cast<uint8_t *>(data);
    uint8_t *data_ptr = reinterpret_cast<uint8_t *>(data);
    size_t i;
    for (i = 0; i < size; i++)
    {
        if (i < offset)
        {
            *write_ptr = *(data_ptr + i);
            AdvanceWritePtr();
        }
        else
        {
            *write_ptr = data->data[i - offset];
            AdvanceWritePtr();
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
        AdvanceWritePtr();
        if (bytes_remaining == 0)
        {
            messages_availible++;
            message_complete = true;
        }
    }
    return i - len;
}
void RingBuffer::AdvanceWritePtr()
{
    if (write_ptr + 1 == read_ptr || (write_ptr == end_ptr && read_ptr == start_ptr))
    {
        std::cout << "BUFFER FULL" << std::endl;
        buffer_full = true;
    }
    else if (write_ptr == end_ptr)
    {
        buffer_empty = false;
        write_ptr = start_ptr;
        bytes_availible++;
        bytes_remaining--;
    }
    else
    {
        buffer_empty = false;
        write_ptr++;
        bytes_availible++;
        bytes_remaining--;
    }
}
void RingBuffer::AdvanceReadPtr()
{
    if (read_ptr == write_ptr)
    {
        buffer_empty = true;
    }
    else if (read_ptr == end_ptr)
    {
        buffer_full = false;
        read_ptr = start_ptr;
        bytes_availible--;
    }
    else
    {
        buffer_full = false;
        read_ptr++;
        bytes_availible--;
    }
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