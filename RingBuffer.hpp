#pragma once

class RingBuffer
{
public:
    // Constructors
    RingBuffer(int len);
    ~RingBuffer();
    // Resets
    void ResetBuffer();// Resets All Errors And Resets (read_ptr) And (write_ptr) To (start_ptr)

    // Memeber Functions
    int ReadData(char *c);// Read 1 Complete Message From (This->buffer) and copy it into (c)
    int WriteData(char *c, int len);// Write (len) Bytes From (c) Into (This->buffer)



    // Debugging Memeber Functions MUST USE (#DEFINE DEBUG 1)
    void PrintData(); // Prints All Bytes In Buffer DOES NOT CONSUME!
    static void PrintMsg(char *c, int len); // Prints (len) Bytes From Pointer (c)
    void PrintDebug(const char* c);
    // Variables
    int data_availible; // Amount Of Valid Bytes In Buffer
    char *buffer;       // Ring Buffer
    // Enums
    enum Error
    {
        NO_DATA = 0,
        INCOMPLETE_DATA = -1,
        BUFFER_OVERFLOW = -2,
        INVALID_DATA = -3,
        BUFFER_OVERREAD = -4,
        UNEXPECTED_ERROR = -99
    };

private:
    bool debug;         // Debug enabled Flag
    char *read_ptr;
    char *write_ptr;
    char *end_ptr;
    char *start_ptr;
    int buffer_len;
    bool buffer_overflow;
};
