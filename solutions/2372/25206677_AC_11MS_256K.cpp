// POJ 2372 - D++ Again
// Model: claude-sonnet-5
//
// Single left-to-right scan with one integer "depth" tracking whether we
// are inside an arithmetic expression (depth = nesting level of its
// brackets) or in "plain text" (depth == 0).
//
// A comment "(* ... *)" is recognized first, in ANY state (plain text or
// inside an expression, at any nesting level), by matching a literal "(*"
// and then finding the leftmost subsequent "*)" (comments do not nest;
// confirmed on the discuss board: "(*(**)" -> YES, "(**)*)" -> NO). If no
// closing "*)" exists, the program is incorrect.
//
// Outside a comment:
//   depth == 0 (plain text): any symbol is legal except '(' and ')'.
//     '(' (not starting a comment) opens a new arithmetic expression
//     (depth becomes 1); a bare ')' here is illegal (unmatched).
//   depth > 0 (inside an expression): legal symbols are digits, the
//     operators = + - * /, '(' / ')' (nested brackets, adjusting depth),
//     and the two end-of-line bytes '\r' and '\n' (spaces are explicitly
//     forbidden per the statement; the discuss board confirms '\r' does
//     appear in the judge data and must be accepted). Anything else is
//     illegal.
// At EOF, depth must be back to 0 (every expression closed).
//
// Ambiguity resolved via the discuss board (message 45236): a comment is
// always the leftmost "(*" ... "*)" match; a "(" immediately followed by
// '*' can never instead be read as an expression-open followed by a
// multiplication.
#include <cstdio>
#include <string>
using namespace std;

int main(){
    string s;
    int c;
    while((c=getchar())!=EOF) s.push_back((char)c);
    int n=(int)s.size();
    int i=0, depth=0;
    bool ok=true;
    while(i<n){
        if(s[i]=='(' && i+1<n && s[i+1]=='*'){
            string::size_type pos = s.find("*)", i+2);
            if(pos==string::npos){ ok=false; break; }
            i = (int)pos+2;
            continue;
        }
        char ch=s[i];
        if(depth==0){
            if(ch=='('){ depth=1; i++; }
            else if(ch==')'){ ok=false; break; }
            else i++;
        } else {
            if(ch=='('){ depth++; i++; }
            else if(ch==')'){ depth--; i++; }
            else if((ch>='0'&&ch<='9')||ch=='='||ch=='+'||ch=='-'||ch=='*'||ch=='/'){ i++; }
            else if(ch=='\n'||ch=='\r'){ i++; }
            else { ok=false; break; }
        }
    }
    if(depth!=0) ok=false;
    printf(ok? "YES\n" : "NO\n");
    return 0;
}
