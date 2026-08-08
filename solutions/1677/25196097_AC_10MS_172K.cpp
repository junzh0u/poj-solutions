// POJ 1677 - Girls' Day
// Model: claude-fable-5
// Approach: pure string processing. For each wish line, lowercase everything
// and split into words (maximal letter runs; '!' and spaces are separators).
// A wish talks to the girls whose names appear as a whole word (names in
// name-list order, each printed once); if none appear it talks to All.
// Response: <=9 words -> "oh"; >=10 words -> "xixi" if any word is exactly
// "beautiful"/"pretty"/"lovely", else "hehe".
// Statement ambiguities settled from the sample and discuss board:
// - Output separator is ": " (sample), not the "semicolon" the statement claims.
// - Matching is whole-word, not substring ("beautifulhh" is neither a name hit
//   nor a xixi trigger unless the name is literally that word).
// - A girl may be named "beautiful"/"pretty"/"lovely"; the name check and the
//   xixi check are independent of each other.
// - Sentence-initial words are capitalized and may be names -> lowercase first.
#include <stdio.h>
#include <string.h>

int main() {
    int g, w;
    if (scanf("%d %d", &g, &w) != 2) return 0;
    static char names[5][256];
    int i;
    for (i = 0; i < g; i++) scanf("%s", names[i]);
    int c;
    while ((c = getchar()) != EOF && c != '\n') {}
    static char line[1024];
    int t;
    for (t = 0; t < w; t++) {
        if (!fgets(line, sizeof(line), stdin)) line[0] = '\0';
        int mentioned[5];
        for (i = 0; i < g; i++) mentioned[i] = 0;
        int words = 0, beauty = 0;
        int n = (int)strlen(line);
        int p = 0;
        while (p < n) {
            if (!((line[p] >= 'a' && line[p] <= 'z') ||
                  (line[p] >= 'A' && line[p] <= 'Z'))) { p++; continue; }
            char word[256];
            int len = 0;
            while (p < n && ((line[p] >= 'a' && line[p] <= 'z') ||
                             (line[p] >= 'A' && line[p] <= 'Z'))) {
                char ch = line[p];
                if (ch >= 'A' && ch <= 'Z') ch = (char)(ch - 'A' + 'a');
                if (len < 250) word[len++] = ch;
                p++;
            }
            word[len] = '\0';
            words++;
            if (!strcmp(word, "beautiful") || !strcmp(word, "pretty") ||
                !strcmp(word, "lovely")) beauty = 1;
            for (i = 0; i < g; i++)
                if (!strcmp(word, names[i])) mentioned[i] = 1;
        }
        int any = 0;
        for (i = 0; i < g; i++) if (mentioned[i]) any = 1;
        if (!any) {
            printf("All");
        } else {
            int first = 1;
            for (i = 0; i < g; i++) {
                if (mentioned[i]) {
                    if (!first) printf(" ");
                    printf("%s", names[i]);
                    first = 0;
                }
            }
        }
        const char *resp;
        if (words <= 9) resp = "oh";
        else if (beauty) resp = "xixi";
        else resp = "hehe";
        printf(": %s\n", resp);
    }
    return 0;
}
