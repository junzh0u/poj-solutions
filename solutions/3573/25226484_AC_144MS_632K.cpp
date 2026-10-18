// POJ 3573 - I18n
// Model: gpt-6-astra
// Scan tokens and preserve separators. For each lowercase abbreviation key,
// retain its unique full word, or an empty string once two words collide.
// Repeated spellings in different cases do not create ambiguity. Expansion
// uses the abbreviation's case and only words already encountered.
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <cctype>
using namespace std;
map<string, string> words;
bool letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
string convert(const string &s) {
    string low = s;
    for (size_t i = 0; i < low.size(); ++i)
        low[i] = char(tolower((unsigned char)low[i]));
    if (s.size() >= 3 && isdigit((unsigned char)s[1])) {
        map<string,string>::iterator it = words.find(low);
        if (it == words.end() || it->second.empty()) return s;
        string result = it->second;
        if (isupper((unsigned char)s[s.size()-1])) {
            for (size_t i = 0; i < result.size(); ++i)
                result[i] = char(toupper((unsigned char)result[i]));
        } else if (isupper((unsigned char)s[0])) {
            result[0] = char(toupper((unsigned char)result[0]));
        }
        return result;
    }
    if (s.size() >= 4) {
        ostringstream key;
        key << low[0] << s.size()-2 << low[low.size()-1];
        map<string,string>::iterator it = words.find(key.str());
        if (it == words.end()) words[key.str()] = low;
        else if (it->second != low) it->second.clear();
    }
    return s;
}
int main() {
    string token;
    char c;
    while (cin.get(c)) {
        if (letter(c) || (c >= '0' && c <= '9')) token += c;
        else {
            if (!token.empty()) { cout << convert(token); token.clear(); }
            cout.put(c);
        }
    }
    if (!token.empty()) cout << convert(token);
    return 0;
}
