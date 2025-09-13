#include <cassert>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using CellState = char;

// Animation removed for simplicity
std::vector<std::vector<CellState>> genlist(const std::vector<int> &info,
                                      const std::vector<CellState> &now) {
  if (info.size() == 0) {
    return std::vector<std::vector<CellState>>(1, std::vector<CellState>(now.size(), 2));
  }
  bool nothing = true;
  for (auto &i : now) {
    if (i == 0)
      nothing = false;
  }
  if (nothing)
    return std::vector<std::vector<CellState>>({now});
  std::vector<int> minindex(info.size()), maxindex(info.size());
  minindex[0] = 0;
  for (int i = 1; i < (int)info.size(); ++i) {
    minindex[i] = minindex[i - 1] + info[i - 1] + 1;
  }
  maxindex.back() = now.size() - info.back();
  for (int i = ((int)info.size()) - 2; i >= 0; --i) {
    maxindex[i] = maxindex[i + 1] - info[i] - 1;
  }
  std::vector nowindex = minindex;
  std::vector<std::vector<CellState>> ret;
  while (true) {
    std::vector<CellState> push_back_element(now.size(), 2);
    for (int i = 0; i < (int)info.size(); ++i) {
      for (int j = nowindex[i]; j < nowindex[i] + info[i]; ++j) {
        push_back_element[j] = 1;
      }
    }
    bool flag = true;
    for (int i = 0; i < (int)now.size(); ++i) {
      if (now[i] == 0)
        continue;
      if (now[i] == push_back_element[i])
        continue;
      flag = false;
      break;
    }
    if (flag)
      ret.push_back(push_back_element);
    int turn_back_point = -1;
    for (int i = info.size() - 1; i >= 0; --i) {
      if (nowindex[i] >= maxindex[i])
        continue;
      turn_back_point = i;
      break;
    }
    if (turn_back_point == -1) {
      break;
    }
    for (int i = turn_back_point; i < (int)info.size(); ++i) {
      if (i == turn_back_point) [[unlikely]] {
        ++nowindex[i];
      } else {
        nowindex[i] = nowindex[i - 1] + info[i - 1] + 1;
      }
    }
  }
  return ret;
}

std::vector<std::vector<CellState>> solveNonogram(int h, int w,
    const std::vector<std::vector<int>>& hinfo,
    const std::vector<std::vector<int>>& winfo) {

  std::vector<std::vector<CellState>> board(h, std::vector<CellState>(w, 0));
  bool ismodified = false;

  while (true) {
    ismodified = false;
    for (int i = 0; i < h; ++i) {
      auto avaliblelist = genlist(hinfo[i], board[i]);
      std::vector<CellState> next(w, 0);
      for (int j = 0; j < w; ++j) {
        bool colored = false, notcolored = false;
        for (int k = 0; k < (int)avaliblelist.size(); ++k) {
          if (avaliblelist[k][j] == 1) {
            colored = true;
          }
          if (avaliblelist[k][j] == 2) {
            notcolored = true;
          }
        }
        if (colored && !notcolored) {
          next[j] = 1;
        } else if (!colored && notcolored) {
          next[j] = 2;
        }
      }
      for (int j = 0; j < w; ++j) {
        if (board[i][j] == 0) {
          if (next[j] == 0)
            continue;
          board[i][j] = next[j];
          ismodified = true;
        } else {
          if (board[i][j] != next[j] && next[j] != 0) {
            return board; // conflict
          }
        }
      }
    }
    for (int i = 0; i < w; ++i) {
      std::vector<CellState> verticalboard(h);
      for (int j = 0; j < h; ++j) {
        verticalboard[j] = board[j][i];
      }
      auto availiblelist = genlist(winfo[i], verticalboard);
      std::vector<CellState> next(h, 0);
      for (int j = 0; j < h; ++j) {
        bool colored = false, notcolored = false;
        for (int k = 0; k < (int)availiblelist.size(); ++k) {
          if (availiblelist[k][j] == 1) {
            colored = true;
          }
          if (availiblelist[k][j] == 2) {
            notcolored = true;
          }
        }
        if (colored && !notcolored) {
          next[j] = 1;
        } else if (!colored && notcolored) {
          next[j] = 2;
        }
      }
      for (int j = 0; j < h; ++j) {
        if (board[j][i] == 0) {
          if (next[j] == 0) {
            continue;
          }
          board[j][i] = next[j];
          ismodified = true;
        } else {
          if (board[j][i] != next[j] && next[j] != 0) {
            return board; // conflict
          }
        }
      }
    }
    if (!ismodified)
      break;
  }
  return board;
}

#ifndef NO_MAIN
int main() {
  int h, w;
  std::string line;
  {
    getline(std::cin, line);
    std::istringstream stream(line);
    stream >> h >> w;
  }
  std::vector<std::vector<int>> hinfo(h), winfo(w);
  for (int i = 0; i < h; ++i) {
    getline(std::cin, line);
    std::istringstream stream(line);
    int temp;
    while (stream >> temp) {
      hinfo[i].push_back(temp);
    }
  }
  for (int i = 0; i < w; ++i) {
    getline(std::cin, line);
    std::istringstream stream(line);
    int temp;
    while (stream >> temp) {
      winfo[i].push_back((temp));
    }
  }
  auto board = solveNonogram(h, w, hinfo, winfo);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      std::cout << (j == 0 ? "" : " ")
                << (board[i][j] == 1   ? '#'
                    : board[i][j] == 2 ? '.'
                                       : '?');
    }
    std::cout << '\n';
  }
}
#endif
