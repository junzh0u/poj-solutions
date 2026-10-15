// POJ 2448 - A New Operating System
// Model: gpt-6.1-sol
// Maintain one best message per process in an ordered set, and a second
// set ordered by memory. Remove old keys before every state change.
// Memory and priority products require 64 bits. Zero-memory creation
// closes immediately; for zero outer priority, use highest inner first.
#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <queue>
#include <utility>
using namespace std;
typedef long long Int;
typedef pair<Int,int> Key;
struct Process {
    Int memory;
    int priority;
    priority_queue<int> messages;
    Process() : memory(0), priority(0) {}
};
map<int,Process> processes;
set<Key> memories, ready;
void detach(int id, Process &p) {
    memories.erase(Key(p.memory,-id));
    if (!p.messages.empty()) ready.erase(Key(Int(p.priority)*p.messages.top(),-id));
}
void attach(int id, Process &p) {
    memories.insert(Key(p.memory,-id));
    if (!p.messages.empty()) ready.insert(Key(Int(p.priority)*p.messages.top(),-id));
}
int main() {
    int n;
    if (scanf("%d",&n)!=1) return 0;
    char line[256];
    fgets(line,sizeof(line),stdin);
    for (int i=0;i<n;++i) {
        if (!fgets(line,sizeof(line),stdin)) break;
        int a[3]={0,0,0}, count=0;
        for (int j=0;line[j];++j) if (line[j]>='0' && line[j]<='9') {
            int v=0;
            do { v=v*10+line[j]-'0'; ++j; } while(line[j]>='0' && line[j]<='9');
            if(count<3) a[count++]=v;
            if(!line[j]) break;
        }
        if(strncmp(line,"CreateProcess",13)==0) {
            if(processes.find(a[0])!=processes.end()) puts("Error");
            else if(a[1]>0) {
                Process &p=processes[a[0]];
                p.memory=a[1]; p.priority=a[2]; attach(a[0],p);
            }
            continue;
        }
        bool run = strncmp(line,"RunProcess",10)!=0 && strncmp(line,"Run",3)==0;
        bool closeMax = strncmp(line,"CloseMaxMemory",14)==0;
        int id=a[0];
        if(run || closeMax) {
            if((run && ready.empty()) || (closeMax && memories.empty())) {
                puts("Empty"); continue;
            }
            id=-(run ? ready.rbegin()->second : memories.rbegin()->second);
        }
        map<int,Process>::iterator it=processes.find(id);
        if(it==processes.end()) { puts("Error"); continue; }
        Process &p=it->second;
        detach(id,p);
        bool close=closeMax || strncmp(line,"CloseProcess",12)==0;
        if(run) {
            printf("Run: %lld\n",Int(p.priority)*p.messages.top()); p.messages.pop();
        } else if(strncmp(line,"RunProcess",10)==0) {
            if(p.messages.empty()) puts("Empty");
            else { printf("Run Process: %d\n",p.messages.top()); p.messages.pop(); }
        } else if(strncmp(line,"AddMessage",10)==0) p.messages.push(a[1]);
        else if(strncmp(line,"ChangePriority",14)==0) p.priority=a[1];
        else if(strncmp(line,"GetMemory",9)==0) p.memory+=a[1];
        else if(strncmp(line,"FreeMemory",10)==0) p.memory-=a[1];
        if(close || p.memory<=0) processes.erase(it);
        else attach(id,p);
    }
    return 0;
}
