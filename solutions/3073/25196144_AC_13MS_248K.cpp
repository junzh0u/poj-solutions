// POJ 3073 - Spam
// Model: gpt-5.6-terra
// Encode the input, then count every segmentation of that character string
// into spam-alphabet tokens with a prefix dynamic program.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

int main() {
    const char *code[26] = {
        "4", "|3", "(", "|)", "3", "|=", "6", "#", "|", "_|", "|<", "|_",
        "|\\/|", "|\\|", "0", "|0", "(,)", "|?", "5", "7", "|_|", "\\/", "\\/\\/", "><", "-/", "2"
    };
    char input[105];
    while (std::scanf("%100s", input) == 1 && std::strcmp(input, "end") != 0) {
        std::string encoded;
        for (int i = 0; input[i]; ++i) encoded += code[input[i] - 'A'];
        std::vector<long long> ways(encoded.size() + 1, 0);
        ways[0] = 1;
        for (std::size_t i = 0; i < encoded.size(); ++i) {
            if (ways[i] == 0) continue;
            for (int letter = 0; letter < 26; ++letter) {
                std::size_t length = std::strlen(code[letter]);
                if (i + length <= encoded.size() && encoded.compare(i, length, code[letter]) == 0)
                    ways[i + length] += ways[i];
            }
        }
        std::printf("%lld\n", ways[encoded.size()]);
    }
    return 0;
}
