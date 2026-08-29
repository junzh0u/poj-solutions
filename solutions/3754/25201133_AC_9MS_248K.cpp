// POJ 3754 - 模拟Modbus协议
// Model: claude-sonnet-5
// Simulate the Modbus RTU protocol: build a request frame from four decimal
// fields plus a CRC16 (Modbus polynomial 0xA001, standard bit-shift
// algorithm) written low-byte-then-high-byte as the checksum; and parse a
// response frame (addr, function, byte-count, IEEE-754 big-endian floats,
// checksum), verifying the same CRC before decoding the floats.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

static unsigned short crc16(const unsigned char *buf, int len) {
    unsigned short crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= buf[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) crc = (unsigned short)((crc >> 1) ^ 0xA001);
            else crc = (unsigned short)(crc >> 1);
        }
    }
    return crc;
}

static int hexval(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

int main() {
    int a, f, sa, nb;
    char line[200];
    while (scanf("%d,%d,%d,%d", &a, &f, &sa, &nb) == 4) {
        scanf("%s", line);
        // ---- Part 1: build send frame ----
        unsigned char bytes[6];
        bytes[0] = (unsigned char)(a & 0xFF);
        bytes[1] = (unsigned char)(f & 0xFF);
        bytes[2] = (unsigned char)((sa >> 8) & 0xFF);
        bytes[3] = (unsigned char)(sa & 0xFF);
        bytes[4] = (unsigned char)((nb >> 8) & 0xFF);
        bytes[5] = (unsigned char)(nb & 0xFF);
        unsigned short crc = crc16(bytes, 6);
        unsigned char cs1 = (unsigned char)(crc & 0xFF);
        unsigned char cs2 = (unsigned char)((crc >> 8) & 0xFF);
        printf("%02X%02X%04X%04X%02X%02X\n",
               bytes[0], bytes[1], sa & 0xFFFF, nb & 0xFFFF, cs1, cs2);

        // ---- Part 2: parse receive frame ----
        int len = (int)strlen(line);
        int nbytes = len / 2;
        unsigned char rb[100];
        for (int i = 0; i < nbytes; i++) {
            rb[i] = (unsigned char)((hexval(line[2*i]) << 4) | hexval(line[2*i+1]));
        }
        int rnb = rb[2]; // NumberOfBytes field in receive frame: 1 byte
        int dataLen = 3 + rnb; // addr,function,numberofbytes + data
        unsigned short rcrc = crc16(rb, dataLen);
        unsigned char rcs1 = (unsigned char)(rcrc & 0xFF);
        unsigned char rcs2 = (unsigned char)((rcrc >> 8) & 0xFF);
        if (rb[dataLen] != rcs1 || rb[dataLen+1] != rcs2) {
            printf("CRC_ERROR\n");
        } else {
            int nfloats = rnb / 4;
            for (int i = 0; i < nfloats; i++) {
                if (i) printf(",");
                uint32_t bits = ((uint32_t)rb[3+4*i] << 24) | ((uint32_t)rb[3+4*i+1] << 16) |
                                 ((uint32_t)rb[3+4*i+2] << 8) | (uint32_t)rb[3+4*i+3];
                float val;
                memcpy(&val, &bits, sizeof(val));
                printf("%.1f", val);
            }
            printf("\n");
        }
    }
    return 0;
}
