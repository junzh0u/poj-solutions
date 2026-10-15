// POJ 1526 - Call Forwarding
// Model: gpt-6.1-sol
// Build the active forwarding graph for each call, then follow its unique path.
// Any repeated extension means a cycle and rings 9999, even after a tail.
// A request is active on the inclusive interval [start, start + duration].
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
struct Rule { int source, start, end, target; };
int nextExtension[10000];
bool visited[10000];
int main() {
    int systems;
    if (scanf("%d", &systems) != 1) return 0;
    puts("CALL FORWARDING OUTPUT");
    for (int system = 1; system <= systems; ++system) {
        printf("SYSTEM %d\n", system);
        vector<Rule> rules;
        int source;
        while (scanf("%d", &source) == 1 && source != 0) {
            Rule r;
            int duration;
            r.source = source;
            scanf("%d%d%d", &r.start, &duration, &r.target);
            r.end = r.start + duration;
            rules.push_back(r);
        }
        int time, extension;
        while (scanf("%d", &time) == 1 && time != 9000) {
            scanf("%d", &extension);
            memset(nextExtension, 0, sizeof(nextExtension));
            memset(visited, 0, sizeof(visited));
            for (unsigned int i = 0; i < rules.size(); ++i)
                if (rules[i].start <= time && time <= rules[i].end)
                    nextExtension[rules[i].source] = rules[i].target;
            int ring = extension;
            while (nextExtension[ring] != 0 && !visited[ring]) {
                visited[ring] = true;
                ring = nextExtension[ring];
            }
            if (visited[ring]) ring = 9999;
            printf("AT %04d CALL TO %04d RINGS %04d\n", time, extension, ring);
        }
    }
    puts("END OF OUTPUT");
    return 0;
}
