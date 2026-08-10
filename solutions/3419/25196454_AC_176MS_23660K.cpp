// POJ 3419 - Difference Is Beautiful
// Model: gpt-5.6-terra
// Approach: A sliding window records the smallest distinct-suffix start for
// every right endpoint and, from those starts, the maximum distinct length for
// every left endpoint.  These lengths form a static range-maximum table.  For a
// query, the suffix-start at R is the exact pivot between lengths ending before
// R and lengths clipped by R, giving an O(1) answer after O(N log N) setup.

#include <algorithm>
#include <cstdio>

using namespace std;

const int MAX_N = 200000;
const int MAX_LOG = 18;
const int VALUE_OFFSET = 1000000;
const int VALUE_COUNT = 2000001;

int distinctSuffixStart[MAX_N];
int logarithmTable[MAX_N + 1];
int rangeMaximum[MAX_LOG][MAX_N];
int lastOccurrence[VALUE_COUNT];

int queryMaximum(int left, int right) {
    int length = right - left + 1;
    int power = logarithmTable[length];
    int span = 1 << power;
    return max(rangeMaximum[power][left],
               rangeMaximum[power][right - span + 1]);
}

int main() {
    int companyCount;
    int queryCount;
    if (scanf("%d%d", &companyCount, &queryCount) != 2) {
        return 0;
    }

    fill(lastOccurrence, lastOccurrence + VALUE_COUNT, -1);
    int windowLeft = 0;
    for (int right = 0; right < companyCount; ++right) {
        int turnover;
        scanf("%d", &turnover);
        int valueIndex = turnover + VALUE_OFFSET;
        if (lastOccurrence[valueIndex] >= windowLeft) {
            windowLeft = lastOccurrence[valueIndex] + 1;
        }
        distinctSuffixStart[right] = windowLeft;
        lastOccurrence[valueIndex] = right;
    }

    int startToFinish = 0;
    for (int right = 0; right < companyCount; ++right) {
        while (startToFinish < distinctSuffixStart[right]) {
            rangeMaximum[0][startToFinish] = right - startToFinish;
            ++startToFinish;
        }
    }
    while (startToFinish < companyCount) {
        rangeMaximum[0][startToFinish] = companyCount - startToFinish;
        ++startToFinish;
    }

    logarithmTable[1] = 0;
    for (int length = 2; length <= companyCount; ++length) {
        logarithmTable[length] = logarithmTable[length / 2] + 1;
    }
    for (int power = 1; power < MAX_LOG &&
                        (1 << power) <= companyCount; ++power) {
        int span = 1 << power;
        int half = span / 2;
        for (int left = 0; left + span <= companyCount; ++left) {
            rangeMaximum[power][left] =
                max(rangeMaximum[power - 1][left],
                    rangeMaximum[power - 1][left + half]);
        }
    }

    for (int query = 0; query < queryCount; ++query) {
        int left;
        int right;
        scanf("%d%d", &left, &right);
        int pivot = distinctSuffixStart[right];
        int answer;
        if (pivot <= left) {
            answer = right - left + 1;
        } else {
            answer = max(queryMaximum(left, pivot - 1),
                         right - pivot + 1);
        }
        printf("%d\n", answer);
    }
    return 0;
}
