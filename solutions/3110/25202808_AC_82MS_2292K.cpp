// POJ 3110 - Jenny's First Exam
// Model: gpt-5.6-terra
// Sort exams by their preparation deadline.  For a proposed first day, assign
// each exam the earliest still-free non-exam day in its interval; this is the
// interval scheduling greedy criterion.  A disjoint-set successor structure
// supplies that day in almost constant time.  Binary search the latest
// feasible first day.
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

struct Exam {
    int left;
    int right;
};

static bool byRight(const Exam &a, const Exam &b) {
    return a.right < b.right;
}

static int daysBeforeYear(int year) {
    int y = year - 1;
    return 365 * y + y / 4 - y / 100 + y / 400;
}

static bool leap(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

static int serial(int day, int month, int year) {
    static const int beforeMonth[] = {
        0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };
    int result = daysBeforeYear(year) + beforeMonth[month] + day - 1;
    if (month > 2 && leap(year)) ++result;
    return result;
}

static void printDate(int value) {
    int lo = 1, hi = 3000;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (daysBeforeYear(mid) <= value) lo = mid;
        else hi = mid - 1;
    }
    int year = lo;
    int dayOfYear = value - daysBeforeYear(year);
    static const int monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int month = 1;
    while (true) {
        int count = monthDays[month] + (month == 2 && leap(year));
        if (dayOfYear < count) break;
        dayOfYear -= count;
        ++month;
    }
    printf("%02d.%02d.%04d\n", dayOfYear + 1, month, year);
}

static vector<int> parentBase;
static vector<int> parent;
static vector<Exam> exams;
static int lowDay;
static int highDay;

static int findSet(int x) {
    int root = x;
    while (parent[root] != root) root = parent[root];
    while (parent[x] != x) {
        int next = parent[x];
        parent[x] = root;
        x = next;
    }
    return root;
}

static bool feasible(int start) {
    parent = parentBase;
    int last = highDay - lowDay + 1;
    for (size_t i = 0; i < exams.size(); ++i) {
        int want = exams[i].left > start ? exams[i].left : start;
        int pos = findSet(want - lowDay);
        if (pos == last || pos + lowDay > exams[i].right) return false;
        parent[pos] = findSet(pos + 1);
    }
    return true;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<int> examDays;
    exams.reserve(n);
    lowDay = 2147483647;
    highDay = -1;
    for (int i = 0; i < n; ++i) {
        char subject[16], date[16];
        int t, day, month, year;
        scanf("%15s%15s%d", subject, date, &t);
        sscanf(date, "%d.%d.%d", &day, &month, &year);
        int examDay = serial(day, month, year);
        Exam e;
        e.left = examDay - t;
        e.right = examDay - 1;
        exams.push_back(e);
        examDays.push_back(examDay);
        if (e.left < lowDay) lowDay = e.left;
        if (e.right > highDay) highDay = e.right;
    }
    sort(exams.begin(), exams.end(), byRight);
    int length = highDay - lowDay + 1;
    vector<char> blocked(length, 0);
    for (size_t i = 0; i < examDays.size(); ++i) {
        if (examDays[i] >= lowDay && examDays[i] <= highDay)
            blocked[examDays[i] - lowDay] = 1;
    }
    parentBase.resize(length + 1);
    parentBase[length] = length;
    for (int i = length - 1; i >= 0; --i)
        parentBase[i] = blocked[i] ? parentBase[i + 1] : i;
    if (!feasible(lowDay)) {
        puts("Impossible");
        return 0;
    }
    int lo = lowDay, hi = highDay;
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;
        if (feasible(mid)) lo = mid;
        else hi = mid - 1;
    }
    printDate(lo);
    return 0;
}
