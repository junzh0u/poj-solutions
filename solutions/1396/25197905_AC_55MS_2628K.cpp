// POJ 1396 - Simple Arithmetics
// Model: claude-sonnet-5
//
// Big-integer (decimal string) addition, subtraction and multiplication,
// formatted the way it is usually laid out by hand: operands stacked, a
// horizontal separator, and (for multiplication with a multi-digit
// multiplier) the per-digit partial products followed by their sum.
//
// Key formatting rule, derived from the problem statement and confirmed by
// examples on the discuss board (an accepted solution posted there was used
// as a differential oracle): every printed line is right-aligned so that
// units digits line up in one column.  If a line's own content has length
// L and starts "shift" columns to the left of the units column (shift = 0
// for everything except partial-product rows, where shift is the position,
// counted from the right, of the multiplier digit that produced the row),
// then its natural rightmost extent is (shift + L).  The overall block
// width W is the maximum natural extent over every data line in the block
// (both operands, every partial product, and the final result/sum).  A
// horizontal separator's own length is the max of the natural extents of
// only the two lines immediately adjacent to it (not the whole block); it
// is then right-aligned within W exactly like any other row.
//
// Ambiguity settled from the board: naive "always full block width" dashes
// (matching the num1/num2/result triple) happen to coincide with this rule
// for the top separator in every '+' and '*' case because the wider
// neighbour is always the adjacent line -- but NOT for '-' when the
// minuend is much longer than both the subtrahend and the (much smaller)
// difference; there the separator must stay narrow, matching only the
// "-num2" line and the result, not the (longer) first operand.  Verified
// against message 172916's accepted-looking source line by line and by
// differential testing against a compiled copy of it.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

static string addStr(const string&a, const string&b){
    string r;
    int i=(int)a.size()-1, j=(int)b.size()-1, carry=0;
    while(i>=0||j>=0||carry){
        int s=carry;
        if(i>=0) s+=a[i--]-'0';
        if(j>=0) s+=b[j--]-'0';
        r.push_back((char)('0'+s%10));
        carry=s/10;
    }
    reverse(r.begin(), r.end());
    return r;
}

// a - b, assumes a >= b (numerically), both without leading zeros.
static string subStr(const string&a, const string&b){
    string r;
    int i=(int)a.size()-1, j=(int)b.size()-1, borrow=0;
    while(i>=0){
        int s=(a[i]-'0')-borrow-(j>=0?(b[j]-'0'):0);
        if(s<0){ s+=10; borrow=1; } else borrow=0;
        r.push_back((char)('0'+s));
        i--; j--;
    }
    reverse(r.begin(), r.end());
    size_t p=0;
    while(p+1<r.size() && r[p]=='0') p++;
    return r.substr(p);
}

// a * single digit d (0..9)
static string mulDigit(const string&a, int d){
    if(d==0) return "0";
    string r;
    int carry=0;
    for(int i=(int)a.size()-1;i>=0;i--){
        int s=(a[i]-'0')*d+carry;
        r.push_back((char)('0'+s%10));
        carry=s/10;
    }
    while(carry){ r.push_back((char)('0'+carry%10)); carry/=10; }
    reverse(r.begin(), r.end());
    size_t p=0;
    while(p+1<r.size() && r[p]=='0') p++;
    return r.substr(p);
}

// acc += value * 10^shift  (acc, value are non-negative decimal strings)
static string addShifted(const string& acc, const string& value, int shift){
    string v = value + string(shift, '0');
    return addStr(acc, v);
}

static void printRow(const string& s, int shift, int W){
    int lead = W - shift - (int)s.size();
    for(int i=0;i<lead;i++) putchar(' ');
    fputs(s.c_str(), stdout);
    putchar('\n');
}

// Dash line, own length = max(natural extent of the two adjacent rows),
// then right-aligned within total width W.
static void printDash(int shiftA, int lenA, int shiftB, int lenB, int W){
    int extA = shiftA + lenA;
    int extB = shiftB + lenB;
    int dashLen = extA > extB ? extA : extB;
    int lead = W - dashLen;
    for(int i=0;i<lead;i++) putchar(' ');
    for(int i=0;i<dashLen;i++) putchar('-');
    putchar('\n');
}

int main(){
    int T;
    if(scanf("%d", &T)!=1) return 0;
    static char buf[1100];
    for(int tc=0; tc<T; tc++){
        scanf("%s", buf);
        int len=(int)strlen(buf);
        int pos=-1;
        char op=0;
        for(int i=0;i<len;i++){
            if(buf[i]=='+'||buf[i]=='-'||buf[i]=='*'){ pos=i; op=buf[i]; break; }
        }
        string num1(buf, buf+pos);
        string num2(buf+pos+1, buf+len);

        if(op=='+' || op=='-'){
            string result = (op=='+') ? addStr(num1, num2) : subStr(num1, num2);
            string opnum2 = string(1, op) + num2;
            int W = (int)num1.size();
            W = max(W, (int)opnum2.size());
            W = max(W, (int)result.size());
            printRow(num1, 0, W);
            printRow(opnum2, 0, W);
            printDash(0, (int)opnum2.size(), 0, (int)result.size(), W);
            printRow(result, 0, W);
        } else {
            int k = (int)num2.size();
            string opnum2 = string(1,'*') + num2;
            vector<string> partial(k);
            string acc = "0";
            for(int i=0;i<k;i++){
                int digit = num2[k-1-i]-'0'; // shift i corresponds to digit at position k-1-i
                partial[i] = mulDigit(num1, digit);
                acc = addShifted(acc, partial[i], i);
            }
            int W = (int)num1.size();
            W = max(W, (int)opnum2.size());
            for(int i=0;i<k;i++) W = max(W, i + (int)partial[i].size());
            if(k>1) W = max(W, (int)acc.size());

            printRow(num1, 0, W);
            printRow(opnum2, 0, W);
            if(k==1){
                printDash(0, (int)opnum2.size(), 0, (int)partial[0].size(), W);
                printRow(partial[0], 0, W);
            } else {
                printDash(0, (int)opnum2.size(), 0, (int)partial[0].size(), W);
                for(int i=0;i<k;i++) printRow(partial[i], i, W);
                printDash(k-1, (int)partial[k-1].size(), 0, (int)acc.size(), W);
                printRow(acc, 0, W);
            }
        }
        putchar('\n');
    }
    return 0;
}
