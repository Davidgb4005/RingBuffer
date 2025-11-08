
#include "RingBuffer.hpp"

int main()
{
    
    RingBuffer ring(120);
    RingBuffer ring2(24);
    int len = 0;
    char msg[255];
    // Messages to write
    // Full messages
    char message1[] = {5, 'H', 'E', 'L', 'L', 'O'};
    char message2[] = {4, 'T', 'H', 'I', 'S'};
    char message3[] = {2, 'I', 'S'};
    char message4[] = {5, 'W', 'O', 'R', 'D', 'S'};
    char message5[] = {12, 'L', 'O', 'N', 'G', 'M', 'E', 'S', 'S', 'A', 'G', 'E', '1'}; // longer message
    char message6[] = {7, 'N', 'E', 'W', ' ', 'M', 'S', 'G'};
    char message7[] = {3, 'B', 'Y', 'E'};

    // Fragmented messages
    char message8_part1[] = {10, 'F', 'R', 'A', 'G', 'M', 'E'}; // first part with length = 10
    char message8_part2[] = {'N', 'T', 'E', 'D'};               // continuation, no length byte

    char message9_part1[] = {16, 'V', 'E', 'R', 'Y', 'L', 'O', 'N', 'G', 'M', 'E', 'S'}; // first part
    char message9_part2[] = {'S', 'A', 'G', 'E', '2'};                                   // second part

    char message10[] = {4, 'T', 'E', 'S', 'T'};

    // Write all messages and fragments to ring buffer
    ring.WriteData(message1, sizeof(message1));
    ring.WriteData(message2, sizeof(message2));
    ring.WriteData(message3, sizeof(message3));
    ring.WriteData(message4, sizeof(message4));
    ring.WriteData(message5, sizeof(message5));
    ring.WriteData(message6, sizeof(message6));
    ring.WriteData(message7, sizeof(message7));

    ring.WriteData(message8_part1, sizeof(message8_part1));
    ring.WriteData(message8_part2, sizeof(message8_part2));
    ring.WriteData(message9_part1, sizeof(message9_part1));
    ring.WriteData(message9_part2, sizeof(message9_part2));

    ring.WriteData(message10, sizeof(message10));
    ring.PrintData();
    len = 1;
    while(len>0){
    len = ring.ReadData(msg);
    RingBuffer::PrintMsg(msg,len);}
    return 0;
}
