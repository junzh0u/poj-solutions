// POJ 1760 - Disk Tree
// Model: gpt-5.6-terra
// Approach: Split every absolute path into directory components and sort the
// component sequences lexicographically.  For each sequence, print only the
// suffix after its longest common prefix with the preceding sequence; sorted
// order makes every already printed directory part of that common prefix.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::string> Path;

Path split_path(const std::string &text) {
    Path result;
    std::string::size_type begin = 0;
    while (begin < text.size()) {
        const std::string::size_type end = text.find('\\', begin);
        if (end == std::string::npos) {
            result.push_back(text.substr(begin));
            break;
        }
        result.push_back(text.substr(begin, end - begin));
        begin = end + 1;
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);

    int count;
    if (!(std::cin >> count)) {
        return 0;
    }

    std::vector<Path> paths;
    paths.reserve(count);
    for (int i = 0; i < count; ++i) {
        std::string text;
        std::cin >> text;
        paths.push_back(split_path(text));
    }
    std::sort(paths.begin(), paths.end());

    Path previous;
    for (std::vector<Path>::const_iterator path = paths.begin();
         path != paths.end(); ++path) {
        std::size_t common = 0;
        while (common < previous.size() && common < path->size()
               && previous[common] == (*path)[common]) {
            ++common;
        }
        for (std::size_t depth = common; depth < path->size(); ++depth) {
            std::cout << std::string(depth, ' ') << (*path)[depth] << '\n';
        }
        previous = *path;
    }
    return 0;
}
