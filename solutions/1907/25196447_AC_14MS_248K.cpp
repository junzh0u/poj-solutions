// POJ 1907 - Work Reduction
// Model: gpt-5.6-terra
// Approach: For each agency, enumerate how many times the current workload is
// halved before finishing with unit reductions.  Unit reductions can always be
// moved after all chosen halvings without increasing their number, so these
// logarithmically many candidates contain an optimum.  Halving rounds the
// remaining workload down, as specified.

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Agency {
    string name;
    int unitCost;
    int halfCost;
    int minimumCost;
};

bool cheaperAgency(const Agency &left, const Agency &right) {
    if (left.minimumCost != right.minimumCost) {
        return left.minimumCost < right.minimumCost;
    }
    return left.name < right.name;
}

int computeMinimumCost(int start, int target, int unitCost, int halfCost) {
    int workload = start;
    int halvings = 0;
    int best = (start - target) * unitCost;

    while (workload / 2 >= target) {
        workload /= 2;
        ++halvings;
        int cost = halvings * halfCost + (workload - target) * unitCost;
        if (cost < best) {
            best = cost;
        }
    }
    return best;
}

int main() {
    int caseCount;
    cin >> caseCount;
    for (int caseNumber = 1; caseNumber <= caseCount; ++caseNumber) {
        int start;
        int target;
        int agencyCount;
        cin >> start >> target >> agencyCount;

        vector<Agency> agencies(agencyCount);
        for (int index = 0; index < agencyCount; ++index) {
            string description;
            cin >> description;
            string::size_type colon = description.find(':');
            string::size_type comma = description.find(',', colon + 1);
            agencies[index].name = description.substr(0, colon);
            agencies[index].unitCost =
                atoi(description.substr(colon + 1, comma - colon - 1).c_str());
            agencies[index].halfCost =
                atoi(description.substr(comma + 1).c_str());
            agencies[index].minimumCost = computeMinimumCost(
                start, target, agencies[index].unitCost, agencies[index].halfCost);
        }

        sort(agencies.begin(), agencies.end(), cheaperAgency);
        cout << "Case " << caseNumber << '\n';
        for (int index = 0; index < agencyCount; ++index) {
            cout << agencies[index].name << ' '
                 << agencies[index].minimumCost << '\n';
        }
    }
    return 0;
}
