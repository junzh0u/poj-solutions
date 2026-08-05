// POJ 1072 - Puzzle Out
//
// The cipher is a permutation of the 26 letters, blanks are kept as they are,
// so every encrypted word must decrypt to a dictionary word of the same
// *repetition pattern*: relabel a word by the order in which its letters first
// occur (SEE -> ABB) and only dictionary words with an identical pattern can be
// the plaintext of an encrypted word.  Bucketing the dictionary by that pattern
// therefore gives each encrypted word its candidate list right away.
//
// Constraint propagation shrinks those lists before any search: every cipher
// letter keeps a 26-bit domain of the plaintext letters it may still become, a
// candidate that disagrees with the domains is dropped, and each domain is then
// intersected with the union of the letters its candidates still allow.  A
// letter whose domain became a singleton also removes that plaintext letter from
// every other domain (the map is injective).  Iterating to a fixpoint usually
// leaves one candidate per word.
//
// What remains is solved by backtracking, always assigning the word with the
// fewest surviving candidates next and re-filtering every unassigned word after
// each assignment, so a word starved of candidates fails the branch at once
// instead of when the search finally reaches it.  Distinct leaves are necessarily
// distinct tables, because the table plus the encrypted word determines the
// plaintext word, so the leaves can simply be counted and the search cut off as
// soon as a second one shows up.

#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<map>
#include<utility>
#include<algorithm>
using namespace std;

const int LETTERS=26;
const int FULL=(1<<LETTERS)-1;

// Relabel a word by first occurrence of its letters: SEE -> ABB.  Two words can
// encrypt to each other if and only if their patterns are equal (the pattern
// also encodes the length).
string Pattern(const string &w){
    int id[LETTERS];
    memset(id,-1,sizeof(id));
    int next=0;
    string p(w.size(),' ');
    for(size_t i=0;i<w.size();i++){
        int c=w[i]-'A';
        if(id[c]<0) id[c]=next++;
        p[i]=(char)('A'+id[c]);
    }
    return p;
}

int PopCount(int mask){
    int n=0;
    while(mask){mask&=mask-1;n++;}
    return n;
}

int LetterMask(const string &w){
    int mask=0;
    for(size_t i=0;i<w.size();i++) mask|=1<<(w[i]-'A');
    return mask;
}

vector<string> dict;                        // de-duplicated dictionary
map<string,vector<int> > buckets;           // pattern -> dictionary indices

// State of one test case.
vector<string> words;                       // distinct encrypted words
vector<vector<int> > cand;                  // candidate dictionary words per word
vector<int> order;                          // order in which words are assigned
int dom[LETTERS];                           // cipher letter -> possible plain letters
int used;                                   // cipher letters occurring in the text
int cipherToPlain[LETTERS];                 // current partial map, -1 = free
int usedPlain;                              // plain letters already taken
int solutions;
int solution[LETTERS];                      // first complete map found

// Drop the candidates that contradict the current domains and tighten the
// domains with what the survivors allow.  Returns false when a word runs out of
// candidates or a domain becomes empty.
bool Propagate(){
    bool changed=true;
    while(changed){
        changed=false;
        for(size_t k=0;k<words.size();k++){
            const string &w=words[k];
            vector<int> &cs=cand[k];
            int allowed[LETTERS];
            memset(allowed,0,sizeof(allowed));
            size_t kept=0;
            for(size_t j=0;j<cs.size();j++){
                const string &p=dict[cs[j]];
                bool ok=true;
                for(size_t i=0;i<w.size();i++)
                    if(!(dom[w[i]-'A']>>(p[i]-'A')&1)){ok=false;break;}
                if(!ok) continue;
                for(size_t i=0;i<w.size();i++) allowed[w[i]-'A']|=1<<(p[i]-'A');
                cs[kept++]=cs[j];
            }
            if(kept==0) return false;
            if(kept<cs.size()){cs.resize(kept);changed=true;}
            int letters=LetterMask(w);
            for(int c=0;c<LETTERS;c++){
                if(!(letters>>c&1)) continue;
                int narrowed=dom[c]&allowed[c];
                if(narrowed!=dom[c]){
                    dom[c]=narrowed;
                    if(!narrowed) return false;
                    changed=true;
                }
            }
        }
        // The table is injective: a letter pinned to a single plaintext letter
        // takes it away from all the others.
        for(int c=0;c<LETTERS;c++){
            if(!(used>>c&1)||PopCount(dom[c])!=1) continue;
            for(int o=0;o<LETTERS;o++){
                if(o==c||!(used>>o&1)) continue;
                int narrowed=dom[o]&~dom[c];
                if(narrowed!=dom[o]){
                    dom[o]=narrowed;
                    if(!narrowed) return false;
                    changed=true;
                }
            }
        }
    }
    return true;
}

// Can `p` still be the plaintext of `w` under the current partial map?  Two
// distinct letters of `w` never need the same letter of `p`, that is exactly what
// the equal patterns guarantee, so only the map and the taken letters matter.
bool Fits(const string &w,const string &p){
    for(size_t i=0;i<w.size();i++){
        int c=w[i]-'A',t=p[i]-'A';
        if(cipherToPlain[c]<0){
            if((usedPlain>>t&1)||!(dom[c]>>t&1)) return false;
        }else if(cipherToPlain[c]!=t) return false;
    }
    return true;
}

// Forward check: after every assignment each word still to be assigned drops the
// candidates the freshly pinned letters ruled out, and a word left without any
// candidate fails the branch immediately.  The surviving candidates of word k are
// the prefix cand[k][0..alive[k]-1]; a rejected one is swapped behind that prefix,
// so undoing a level only has to put the old length back (the set is restored,
// its order is not, which does not matter).  Without this the search notices a
// contradiction only once it reaches the offending word, far too late for a text
// made of dozens of short words.
vector<int> alive;
vector<pair<int,int> > trail;                // (word, previous alive count)

bool Prune(size_t pos){
    for(size_t q=pos;q<order.size();q++){
        int k=order[q];
        const string &w=words[k];
        vector<int> &cs=cand[k];
        int n=alive[k],j=0,e=n;
        while(j<e){
            if(Fits(w,dict[cs[j]])) j++;
            else{swap(cs[j],cs[e-1]);e--;}
        }
        if(e<n){
            trail.push_back(make_pair(k,n));
            alive[k]=e;
        }
        if(e==0) return false;
    }
    return true;
}

void Undo(size_t mark){
    while(trail.size()>mark){
        alive[trail.back().first]=trail.back().second;
        trail.pop_back();
    }
}

// Assign the words one by one, always taking the one with the fewest surviving
// candidates next, counting complete tables and stopping at two.
void Search(size_t pos){
    if(solutions>=2) return;
    if(pos==order.size()){
        if(++solutions==1) memcpy(solution,cipherToPlain,sizeof(cipherToPlain));
        return;
    }
    size_t pick=pos;
    for(size_t q=pos+1;q<order.size();q++)
        if(alive[order[q]]<alive[order[pick]]) pick=q;
    swap(order[pos],order[pick]);

    int k=order[pos];
    const string &w=words[k];
    // Deeper levels never touch word k's own list, so this bound stays valid.
    int n=alive[k];
    for(int j=0;j<n;j++){
        const string &p=dict[cand[k][j]];
        if(!Fits(w,p)) continue;
        int touched[LETTERS];
        int ntouched=0;
        for(size_t i=0;i<w.size();i++){
            int c=w[i]-'A',t=p[i]-'A';
            if(cipherToPlain[c]<0){
                cipherToPlain[c]=t;
                usedPlain|=1<<t;
                touched[ntouched++]=c;
            }
        }
        size_t mark=trail.size();
        if(Prune(pos+1)) Search(pos+1);
        Undo(mark);
        while(ntouched--){
            int c=touched[ntouched];
            usedPlain&=~(1<<cipherToPlain[c]);
            cipherToPlain[c]=-1;
        }
        if(solutions>=2) break;
    }
    swap(order[pos],order[pick]);
}

void Solve(const vector<string> &text){
    words=text;
    sort(words.begin(),words.end());
    words.erase(unique(words.begin(),words.end()),words.end());

    used=0;
    for(int c=0;c<LETTERS;c++) dom[c]=FULL;
    for(size_t k=0;k<words.size();k++) used|=LetterMask(words[k]);

    cand.assign(words.size(),vector<int>());
    for(size_t k=0;k<words.size();k++){
        map<string,vector<int> >::const_iterator it=buckets.find(Pattern(words[k]));
        if(it==buckets.end()){puts("#No solution#");return;}
        cand[k]=it->second;
    }
    if(!Propagate()){puts("#No solution#");return;}

    // The search picks the next word itself, so any starting order will do.
    order.resize(words.size());
    for(size_t k=0;k<words.size();k++) order[k]=(int)k;

    alive.resize(words.size());
    for(size_t k=0;k<words.size();k++) alive[k]=(int)cand[k].size();
    trail.clear();
    for(int c=0;c<LETTERS;c++) cipherToPlain[c]=-1;
    usedPlain=0;
    solutions=0;
    Search(0);

    if(solutions==0){puts("#No solution#");return;}
    if(solutions>1){puts("#More than one solution#");return;}

    // out[plain]=cipher, '*' for the plain letters the text never uses.
    char out[LETTERS+1];
    for(int i=0;i<LETTERS;i++) out[i]='*';
    out[LETTERS]='\0';
    for(int c=0;c<LETTERS;c++)
        if(solution[c]>=0) out[solution[c]]=(char)('A'+c);
    puts(out);
}

int main(){
    int d;
    if(scanf("%d",&d)!=1) return 0;
    dict.reserve(d);
    char buf[128];
    for(int i=0;i<d;i++){
        if(scanf("%127s",buf)!=1) break;
        dict.push_back(string(buf));
    }
    // A duplicated dictionary entry would be counted as a second solution.
    sort(dict.begin(),dict.end());
    dict.erase(unique(dict.begin(),dict.end()),dict.end());
    for(size_t i=0;i<dict.size();i++) buckets[Pattern(dict[i])].push_back((int)i);

    int t;
    if(scanf("%d",&t)!=1) return 0;
    int ch;
    while((ch=getchar())!=EOF&&ch!='\n');

    // Test cases are separated by blank lines; runs of blank lines collapse so a
    // stray empty line at the end of a case cannot shift the cases apart.
    vector<vector<string> > cases;
    char line[256];
    bool fresh=true;
    while(fgets(line,sizeof(line),stdin)){
        vector<string> tokens;
        for(char *p=line;*p;){
            while(*p&&(*p<'A'||*p>'Z')) p++;
            char *s=p;
            while(*p>='A'&&*p<='Z') p++;
            if(p>s) tokens.push_back(string(s,p-s));
        }
        if(tokens.empty()){
            if(!cases.empty()) fresh=true;
            continue;
        }
        if(fresh){cases.push_back(vector<string>());fresh=false;}
        for(size_t i=0;i<tokens.size();i++) cases.back().push_back(tokens[i]);
    }

    for(int i=0;i<t;i++)
        Solve(i<(int)cases.size()?cases[i]:vector<string>());
    return 0;
}
