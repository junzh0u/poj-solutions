// POJ 1896 - Code Formatting
// Model: claude-sonnet-5
// Approach: tokenize the TRIVIAL-language source (identifiers and the six
// punctuation characters), then recursive-descent parse the grammar
//   Program ::= Block
//   Block   ::= '{' Statements '}'
//   Statements ::= Statement | Statement Statements
//   Statement  ::= Expression ';'
//   Expression ::= identifier ['(' Arguments ')'] [Block]
//   Arguments  ::= Expression | Expression ',' Arguments
// while directly emitting formatted output lines. Each output line has an
// associated indentation level (4*level spaces); a new line is started for
// every statement and for the closing '}' of every block, and everything
// else (including a block's closing '}' followed by ')'/','/';' from an
// enclosing construct) is appended to whatever line is currently open. This
// mirrors the sample output exactly: a nested block's closing brace shares
// its line with any trailing ')' / ';' from the expression or statement it
// is part of.
// Ambiguity: the discuss board (message 7596/7598) confirms the input is a
// single test case, read to EOF -- not a stream of multiple programs.
#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
using namespace std;

struct Token { char type; string text; };
static vector<Token> toks;
static size_t pos_;

static vector<int> lineLevel;
static vector<string> lineText;

static void newLine(int lvl) { lineLevel.push_back(lvl); lineText.push_back(""); }
static void app(const string &s) { lineText.back() += s; }

static void parseExpression(int lvl);

static void parseArguments(int lvl) {
    parseExpression(lvl);
    while (pos_ < toks.size() && toks[pos_].type == ',') {
        app(", ");
        pos_++;
        parseExpression(lvl);
    }
}

// Assumes the opening '{' has already been consumed; emits the statements
// at indentation lvl and the closing '}' at indentation lvl-1 (appended to
// whatever line is open at that point).
static void parseBlockBody(int lvl) {
    while (pos_ < toks.size() && toks[pos_].type != '}') {
        newLine(lvl);
        parseExpression(lvl);
        app(";");
        pos_++; // consume ';'
    }
    newLine(lvl - 1);
    app("}");
    pos_++; // consume '}'
}

static void parseExpression(int lvl) {
    app(toks[pos_].text); // identifier
    pos_++;
    if (pos_ < toks.size() && toks[pos_].type == '(') {
        app("(");
        pos_++;
        parseArguments(lvl);
        app(")");
        pos_++; // consume ')'
    }
    if (pos_ < toks.size() && toks[pos_].type == '{') {
        app(" {");
        pos_++;
        parseBlockBody(lvl + 1);
    }
}

int main() {
    string input;
    {
        char buf[65536];
        size_t n;
        while ((n = fread(buf, 1, sizeof(buf), stdin)) > 0) input.append(buf, n);
    }
    size_t i = 0, N = input.size();
    while (i < N) {
        unsigned char c = (unsigned char)input[i];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { i++; continue; }
        if (isalnum(c)) {
            size_t j = i;
            while (j < N && isalnum((unsigned char)input[j])) j++;
            Token t; t.type = 'I'; t.text = input.substr(i, j - i);
            toks.push_back(t);
            i = j;
        } else {
            Token t; t.type = (char)c; t.text = string(1, (char)c);
            toks.push_back(t);
            i++;
        }
    }
    pos_ = 0;
    newLine(0);
    app("{");
    if (pos_ < toks.size() && toks[pos_].type == '{') {
        pos_++;
        parseBlockBody(1);
    }

    string out;
    for (size_t k = 0; k < lineText.size(); k++) {
        out.append((size_t)lineLevel[k] * 4, ' ');
        out += lineText[k];
        out += "\n";
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
