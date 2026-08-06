// POJ 3648 - Wedding
// Model: gpt-5.6-terra
// A 2-SAT literal records whether a person sits on the bride's side.  Each
// married pair is forced onto opposite sides, the bride is fixed on her side,
// and an adulterous pair contributes an OR saying that at least one member is
// on the bride's side (so she cannot see both).  Tarjan SCC detects conflicts.
// Pair endpoints are read as integer/character pairs because POJ permits them
// to be adjacent without whitespace.
#include <cstdio>
#include <vector>

using namespace std;

class TwoSat {
 public:
  explicit TwoSat(int variables)
      : variables_(variables), graph_(2 * variables), reverse_(2 * variables),
        seen_(2 * variables, 0), component_(2 * variables, -1) {}

  void AddImplication(int from, int to) {
    graph_[from].push_back(to);
    reverse_[to].push_back(from);
  }

  void AddOr(int first, int second) {
    AddImplication(first ^ 1, second);
    AddImplication(second ^ 1, first);
  }

  bool Solve(vector<int> *value) {
    int i;
    order_.clear();
    for (i = 0; i < 2 * variables_; ++i) {
      if (!seen_[i]) {
        DfsForward(i);
      }
    }
    int component_count = 0;
    for (i = (int)order_.size() - 1; i >= 0; --i) {
      int vertex = order_[i];
      if (component_[vertex] == -1) {
        DfsReverse(vertex, component_count++);
      }
    }
    value->assign(variables_, 0);
    for (i = 0; i < variables_; ++i) {
      if (component_[2 * i] == component_[2 * i + 1]) {
        return false;
      }
      (*value)[i] = component_[2 * i + 1] > component_[2 * i];
    }
    return true;
  }

 private:
  void DfsForward(int vertex) {
    int i;
    seen_[vertex] = 1;
    for (i = 0; i < (int)graph_[vertex].size(); ++i) {
      if (!seen_[graph_[vertex][i]]) {
        DfsForward(graph_[vertex][i]);
      }
    }
    order_.push_back(vertex);
  }

  void DfsReverse(int vertex, int component) {
    int i;
    component_[vertex] = component;
    for (i = 0; i < (int)reverse_[vertex].size(); ++i) {
      if (component_[reverse_[vertex][i]] == -1) {
        DfsReverse(reverse_[vertex][i], component);
      }
    }
  }

  int variables_;
  vector<vector<int> > graph_;
  vector<vector<int> > reverse_;
  vector<int> seen_;
  vector<int> component_;
  vector<int> order_;
};

static int Person(int couple, char spouse) {
  return 2 * couple + (spouse == 'w');
}

static int Literal(int person, bool bride_side) {
  return 2 * person + bride_side;
}

int main() {
  int couples, affairs;
  while (scanf("%d%d", &couples, &affairs) == 2 && (couples || affairs)) {
    int people = 2 * couples;
    TwoSat sat(people);
    int couple;
    for (couple = 0; couple < couples; ++couple) {
      int husband = Person(couple, 'h');
      int wife = Person(couple, 'w');
      sat.AddOr(Literal(husband, false), Literal(wife, false));
      sat.AddOr(Literal(husband, true), Literal(wife, true));
    }
    sat.AddOr(Literal(Person(0, 'w'), true), Literal(Person(0, 'w'), true));
    while (affairs--) {
      int first_couple, second_couple;
      char first_spouse, second_spouse;
      scanf("%d %c%d %c", &first_couple, &first_spouse, &second_couple,
            &second_spouse);
      int first = Person(first_couple, first_spouse);
      int second = Person(second_couple, second_spouse);
      sat.AddOr(Literal(first, true), Literal(second, true));
    }
    vector<int> bride_side;
    if (!sat.Solve(&bride_side)) {
      printf("bad luck\n");
      continue;
    }
    bool first_output = true;
    int person;
    for (person = 2; person < people; ++person) {
      if (bride_side[person]) {
        if (!first_output) {
          printf(" ");
        }
        printf("%d%c", person / 2, person % 2 ? 'w' : 'h');
        first_output = false;
      }
    }
    printf("\n");
  }
  return 0;
}
