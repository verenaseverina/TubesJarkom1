#ifndef _RECEIVERFRAME_
#define _RECEIVERFRAME_

#include <stdio.h>
#include <string.h>

class ReceiverFrame {
private:
    char ack;
    char nextSequenceNumber[4];
    char advWindowSize;
    char checksum;
    bool error;

public:
    ReceiverFrame(int nextSequenceNumber) {
        this->nextSequenceNumber = nextSequenceNumber;
        this->error = false;
    }

    ReceiverFrame(char * frame) {
        this->error = false;
        
        this->setAck(frame[0]);

        ////SETTING FRAME NUMBER
        char * temp = (int)(frame[1] + frame[2] + frame[3] + frame[4]);
        this->setNextSequenceNumber(temp);

        if (!this->error) {
            char checksum[6];
            for (int i = 1 + 1 /* after ETX */; i < 1 + 1 + 4; i++) {
                checksum[i - 2] = frame[i];
            }
            char * framex = this->toBytes();
            char trueChecksum[5];
            for (int i = 1 + 1 /* after ETX */; i < 1 + 1 + 4; i++) {
                trueChecksum[i - 2] = framex[i];
            }
            //check equality    
            for (int i = 0; i < 6 && !this->error; i++) {
                if (checksum[i] != trueChecksum[i]) {
                    this->error = true;
                }
            }
        }
    }

    char getAck() { return this->ack; }
    void setAck(char newAck) { this->ack = newAck; }

    int getNextSequenceNumber() { return this->nextSequenceNumber; }
    void setNextSequenceNumber(int newNumber) { this->nextSequenceNumber = newNumber; }

    char getAdvWindowSize() { return this->advWindowSize; }
    void setAdvWindowSize(char newWindSize) { this->advWindowSize = newWindSize; }
    
    char getChecksum() { return this->checksum; }
    void setChecksum(char newChecksum) { this->checksum = newChecksum; }

    char * toBytes() {
        char * o = new char[1 + 4 + 1 + 1];
        o[0] = this->ack;
        int temp = (int)(frame[1] + frame[2] + frame[3] + frame[4]);
        o[1] = this->nextSequenceNumber;

        unsigned short c = calc_crc16(o, strlen(o));
        char * a = new char[2];
        a[0] = c & 0xff;
        a[1] = (c >> 8) & 0xff;
        sprintf(o, "%s%s", o, a);
        return o;
    }

    int getBytesLength() { return 1 + 4 + 1 + 1; }

    void printBytes() {
        char * buffer = this->toBytes();
        for(int j = 0; buffer[j] != 0; j++)
            printf("%02hhX ", buffer[j]);
    }

    bool isError() { return this->error; }
};

#endif