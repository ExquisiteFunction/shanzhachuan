#include <atomic>
#include <chrono>
#include <cstdio>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include <utility>
#include <vector>
#include <iostream>
using namespace std;
using namespace std::chrono;

int score = 0;
typedef vector<vector<char>> State;
typedef vector<char> Array;
State state;
Array dropping;
typedef pair<char, char> Act;
vector<Act> Actions;
typedef vector<int> Value;
typedef pair<State, vector<Act>> RET;
#define INV -1
#define DEAD -2

void reset(State &state) {
  score = 0;
  state.resize(6);
  for (int i = 0; i < 6; i++)
    state[i] = vector<char>({1, 1});
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 6; j++)
      if (i != j)
        Actions.push_back(Act(i, j));
};

int sum(Array &t) {
  int s = 0;
  for (auto &i : t)
    s += i;
  return s;
}

int max(Array &t) {
  int s = 0;
  for (auto &i : t)
    if (i > s)
      s = i;
  return s;
}

int value_less(Value &va, Value &vb) {
  for (int i = 0; i < va.size(); i++)
    if (va[i] > vb[i])
      return 0;
  return 1;
};

void print_state(State &state) {
  State tmp(7, vector<char>(6, 0));
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < state[i].size(); j++)
      tmp[j][i] = state[i][j];

  printf("Score: %d\n", score);
  for (int i = 6; i >= 0; i--) {
    if (sum(tmp[i]) > 0) {
      for (auto &j : tmp[i])
        printf("%d ", j);
      printf("\n");
    };
  }
};

State copy(State &state) {
  State ret;
  for (auto &t : state) {
    ret.push_back(Array(t));
  }
  return ret;
};

int move(State &state, Act &act) {
  int f = act.first, t = act.second;
  int n = state[act.first].size();
  if (n == 0)
    return INV;
  else if (n == 1) {
    state[t].push_back(state[f][n - 1]);
    state[f].pop_back();
  } else {
    if (state[f][n - 1] == state[f][n - 2]) {
      state[t].push_back(state[f][n - 1]);
      state[t].push_back(state[f][n - 1]);
      state[f].pop_back();
      state[f].pop_back();
    } else {
      state[t].push_back(state[f][n - 1]);
      state[f].pop_back();
    }
  };
  return 0;
};

void comb(Array &t) {
  int n = t.size();
  if (n >= 4) {
    if ((t[n - 1] == t[n - 2]) and (t[n - 1] == t[n - 3]) and
        (t[n - 1] == t[n - 4])) {
      t.pop_back();
      t.pop_back();
      t.pop_back();
      t.back() += 1;
    };
  };
  n = t.size();
  if (n >= 3)
    if ((t[n - 1] == t[n - 2]) and (t[n - 1] == t[n - 3])) {
      t.pop_back();
      t.pop_back();
      t.back() += 1;
      n = t.size();
      if ((t[n - 1] == t[n - 2]) and (t[n - 1] == t[n - 3])) {
        t.pop_back();
        t.pop_back();
        t.back() += 1;
        n = t.size();
        if ((t[n - 1] == t[n - 2]) and (t[n - 1] == t[n - 3])) {
          t.pop_back();
          t.pop_back();
          t.back() += 1;
        };
      };
    };
  if (t.back() == 9)
    t.pop_back();
};

int step(State &state, Act &act) {
  int ret = move(state, act);
  if (ret == INV)
    return INV;

  comb(state[act.second]);

  if (state[act.second].size() > 7)
    return DEAD;

  return 0;
};

void set_state(State &state, State &n_state) { state = n_state; }

int drop(State &state) {
  for (int i = 0; i < 6; i++) {
    state[i].push_back(dropping[i]);
    comb(state[i]);
    if (state[i].size() >= 7)
      return DEAD;
  }
  return 0;
}

Value value(State &state) {
  Array len;
  for (auto &t : state)
    len.push_back(t.size());
  Array empty;
  for (auto &t : state)
    len.push_back(t.size() != 0);
  Value v;
  v.push_back(sum(len));
  v.push_back(max(len));
  v.push_back(sum(empty));
  return v;
};

RET solveK1(State &state) {
  State best;
  vector<Act> best_acts;
  Value best_v = Value({0, 0, 0, 0});
  for (auto &a1 : Actions) {
    State t1 = State(state);
    int r1 = step(t1, a1);
    printf("action : %d %d\n", a1.first, a1.second);
    print_state(t1);
    if ((r1 == INV) or (r1 == DEAD)) {
      ;
    } else {
      Value v1 = value(t1);
      if (value_less(best_v, v1)) {
        best = t1;
        best_v = v1;
        best_acts = vector<Act>({a1});
      }
    }
  };
  return RET(best, best_acts);
};

RET solveK4_v1(State &state) {
  State best;
  vector<Act> best_acts;
  Value best_v = Value({100, 100, 100});
  for (auto &a1 : Actions) {
    State t1 = State(state);
    int r1 = step(t1, a1);
    if ((r1 == INV) or (r1 == DEAD)) {
      ;
    } else {
      for (auto &a2 : Actions) {
        State t2 = State(t1);
        drop(t2);
        int r2 = step(t2, a2);
        if ((r2 == INV) or (r2 == DEAD)) {
          ;
        } else {
          for (auto &a3 : Actions) {
            State t3 = State(t2);
            int r3 = step(t3, a3);
            if ((r3 == INV) or (r3 == DEAD)) {
              ;
            } else {
              for (auto &a4 : Actions) {
                State t4 = State(t3);
                int r4 = step(t4, a4);
                if ((r4 == INV) or (r4 == DEAD)) {
                  ;
                } else {
                  Value v4 = value(t4);
                  if (value_less(v4, best_v)) {
                    best = t4;
                    best_v = v4;
                    best_acts = vector<Act>({a1, a2, a3, a4});
                  }
                }
              };
            }
          };
        }
      };
    }
  };
  return RET(best, best_acts);
};

void solveK3_ths(int nth, State &state, RET &ret) {
  auto beg_t = system_clock::now();
  State best;
  vector<Act> best_acts;
  Value best_v = Value({100, 100, 100});
  for (auto &a1 : Actions) {
    State t1 = State(state);
    int r1 = step(t1, a1);
    if ((r1 == INV) or (r1 == DEAD)) {
      ;
    } else {
      for (auto &a2 : Actions) {
        State t2 = State(t1);
        int r2 = step(t2, a2);
        if ((r2 == INV) or (r2 == DEAD)) {
          ;
        } else {
          for (auto &a3 : Actions) {
            State t3 = State(t2);
            int r3 = step(t3, a3);
            if ((r3 == INV) or (r3 == DEAD)) {
              ;
            } else {
              Value v3 = value(t3);
              if (value_less(v3, best_v)) {
                best = t3;
                best_v = v3;
                best_acts = vector<Act>({a1, a2, a3});
              };
            };
          };
        };
      };
    };
  };
  ret.first = best;
  ret.second = best_acts;
  auto end_t = system_clock::now();
  duration<double> diff = end_t-beg_t;
  cout << "time = " << diff.count() << "s\n";
  return;
};

void solveK5_ths(int nth, State &state, RET &ret) {
  // printf("thread %d starts.\n", nth);
  // time_t begin_t = clock();
  auto beg_t = system_clock::now();
  State best;
  vector<Act> best_acts;
  Value best_v = Value({100, 100, 100});
  for (auto &a1 : Actions) {
    State t1 = State(state);
    int r1 = step(t1, a1);
    if ((r1 == INV) or (r1 == DEAD)) {
      ;
    } else {
      for (auto &a2 : Actions) {
        State t2 = State(t1);
        int r2 = step(t2, a2);
        if ((r2 == INV) or (r2 == DEAD)) {
          ;
        } else {
          for (auto &a3 : Actions) {
            State t3 = State(t2);
            int r3 = step(t3, a3);
            if ((r3 == INV) or (r3 == DEAD)) {
              ;
            } else {
              for (auto &a4 : Actions) {
                State t4 = State(t3);
                int r4 = step(t4, a4);
                if ((r4 == INV) or (r4 == DEAD)) {
                  ;
                } else {
                  for (auto &a5 : Actions) {
                    State t5 = State(t4);
                    int r5 = step(t5, a5);
                    if ((r5 == INV) or (r5 == DEAD)) {
                      ;
                    } else {
                      Value v5 = value(t5);
                      if (value_less(v5, best_v)) {
                        best = t5;
                        best_v = v5;
                        best_acts = vector<Act>({a1, a2, a3, a4, a5});
                      };
                    };
                  };
                };
              };
            };
          };
        };
      };
    };
  };
  ret.first = best;
  ret.second = best_acts;
  // time_t finish_t = clock();
  // printf("time: %fs\n", (double)(finish_t - begin_t) / CLOCKS_PER_SEC);
  auto end_t = system_clock::now();
  duration<double> diff = end_t-beg_t;
  cout << "time = " << diff.count() << "s\n";
  return;
};

void solveK4_ths(int nth, State &state, RET &ret) {
  // printf("thread %d starts.\n", nth);
  // time_t begin_t = clock();
  auto beg_t = system_clock::now();
  State best;
  vector<Act> best_acts;
  Value best_v = Value({100, 100, 100});
  for (auto &a1 : Actions) {
    State t1 = State(state);
    int r1 = step(t1, a1);
    if ((r1 == INV) or (r1 == DEAD)) {
      ;
    } else {
      for (auto &a2 : Actions) {
        State t2 = State(t1);
        int r2 = step(t2, a2);
        if ((r2 == INV) or (r2 == DEAD)) {
          ;
        } else {
          for (auto &a3 : Actions) {
            State t3 = State(t2);
            int r3 = step(t3, a3);
            if ((r3 == INV) or (r3 == DEAD)) {
              ;
            } else {
                  for (auto &a4 : Actions) {
                    State t4 = State(t3);
                    int r4 = step(t4, a4);
                    if ((r4 == INV) or (r4 == DEAD)) {
                      ;
                    } else {
                      Value v4 = value(t4);
                      if (value_less(v4, best_v)) {
                        best = t4;
                        best_v = v4;
                        best_acts = vector<Act>({a1, a2, a3, a4});
                      };
                };
              };
            };
          };
        };
      };
    };
  };
  ret.first = best;
  ret.second = best_acts;
  // time_t finish_t = clock();
  // printf("time: %fs\n", (double)(finish_t - begin_t) / CLOCKS_PER_SEC);
  auto end_t = system_clock::now();
  // printf("time: %fs\n", (end_t-beg_t));
  duration<double> diff = end_t-beg_t;
  cout << "time :" << diff.count() << "\n";
  return;
};

RET solveK4_v2(State &state) {
  State best;
  vector<Act> best_acts;
  Value best_v = Value({100, 100, 100});
  thread ths[30];
  RET rets[30];
  int has_th[30] = {0};
  State states[30];
  for (int i = 0; i < 30; i++) {
    states[i] = State(state);
    int r1 = step(states[i], Actions[i]);
    if ((r1 == INV) or (r1 == DEAD))
      continue;
    r1 = drop(states[i]);
    if (r1 == -2)
      continue;
    has_th[i] = 1;
    State ret_best;
    vector<Act> ret_acts;
    rets[i] = RET(ret_best, ret_acts);
    ths[i] = thread(solveK5_ths, i, ref(states[i]), ref(rets[i]));
  };

  for (int i = 0; i < 30; i++) {
    if (has_th[i]) {
      ths[i].join();
      if (rets[i].first.size() > 0) {
        Value v = value(rets[i].first);
        if (value_less(v, best_v)) {
          best = rets[i].first;
          best_v = v;
          best_acts = vector<Act>({Actions[i]});
          for (auto &act : rets[i].second)
            best_acts.push_back(act);
        };
      };
    };
  }
  return RET(best, best_acts);
};

RET solveK(State &state, int k) {
  // time_t begin_t = clock();
  auto beg_t = system_clock::now();
  RET ret;
  if (k == 4)
    ret = solveK4_v2(state);
  if (k == 1)
    ret = solveK1(state);
  // time_t finish_t = clock();
  // printf("\ntotal time: %fs\n", (double)(finish_t - begin_t) / CLOCKS_PER_SEC);
  auto end_t = system_clock::now();
  duration<double> diff = end_t-beg_t;
  cout << "total time : " << diff.count() << "\n";
  return ret;
};

void test1(State &state) {
  reset(state);
  print_state(state);
  Act act = Act(1, 2);
  step(state, act);
  print_state(state);
};

void test2(State &state) {
  reset(state);
  // print_state(state);
  state = State({{}, {3}, {}, {}, {}, {}});
  // print_state(state);
  dropping = Array({4, 2, 3, 3, 2, 2});
  // drop(state);
  print_state(state);
  RET ret = solveK(state, 4);
  print_state(ret.first);

  dropping = Array({4, 4, 5, 5, 3, 4});
  state = State(ret.first);
  print_state(state);
  ret = solveK(state, 4);
  print_state(ret.first);
};

void test3(State &state) {
  reset(state);
  state = State({{5, 2, 5},
                 {5, 3, 7},
                 {4, 4, 6, 6},
                 {2, 7},
                 {6, 3, 2, 5, 5},
                 {4, 2, 2, 6, 2}});
  dropping = Array({5, 6, 7, 5, 3, 5});
  print_state(state);
  RET ret = solveK(state, 4);

  // print_state(ret.first);
  // print_state(state);
  // Act a = ret.second[0];
  // printf("\n%d->%d\n", a.first, a.second);
  // step(state, a);
  // print_state(state);
  // drop(state);
  // printf("\ndropping...\n");
  // print_state(state);
  // a = ret.second[1];
  // printf("\n%d->%d\n", a.first, a.second);
  // step(state, a);
  // print_state(state);
  // a = ret.second[2];
  // printf("\n%d->%d\n", a.first, a.second);
  // step(state, a);
  // print_state(state);
  // a = ret.second[3];
  // printf("\n%d->%d\n", a.first, a.second);
  // step(state, a);
  // print_state(state);

  print_state(ret.first);
};

int main() {
  State state;
  test3(state);
};