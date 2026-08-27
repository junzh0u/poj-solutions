// POJ 2144 - Leaky Cryptography
// Model: gpt-5.6-terra
// Recover key bits from low to high.  At each bit, the carry into the sum of
// the eight decoded chunks is fixed by lower bits, so exactly one key bit
// makes that sum agree with the decoded checksum bit.
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

int main() {
    int sets;
    if (!(cin >> sets)) return 0;
    while (sets--) {
        unsigned int c[9];
        int i;
        for (i = 0; i < 9; ++i) {
            string token;
            stringstream parser;
            cin >> token;
            parser << hex << token;
            parser >> c[i];
        }

        unsigned int key = 0;
        unsigned int carry = 0;
        int bit;
        for (bit = 0; bit < 32; ++bit) {
            unsigned int encryptedOnes = 0;
            for (i = 0; i < 8; ++i)
                encryptedOnes += (c[i] >> bit) & 1U;
            unsigned int checksumBit = (c[8] >> bit) & 1U;
            unsigned int keyBit;
            for (keyBit = 0; keyBit < 2; ++keyBit) {
                unsigned int decodedOnes = carry +
                    (keyBit ? 8U - encryptedOnes : encryptedOnes);
                if ((decodedOnes & 1U) == (checksumBit ^ keyBit)) {
                    if (keyBit) key |= (1U << bit);
                    carry = decodedOnes >> 1;
                    break;
                }
            }
        }
        cout << hex << nouppercase << key << '\n';
    }
    return 0;
}
