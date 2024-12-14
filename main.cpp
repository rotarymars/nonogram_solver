#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
std::vector<std::vector<int>> genlist(const std::vector<int> &info,
                                      const std::vector<int> &now) {
  if (info.size() == 0) {
    return std::vector<std::vector<int>>(1, std::vector<int>(now.size(), 2));
  }
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
  std::vector<std::vector<int>> ret;
  while (true) {
    std::vector<int> push_back_element(now.size(), 2);
    for (int i = 0; i < (int)info.size(); ++i) {
      for (int j = nowindex[i]; j < nowindex[i] + info[i]; ++j) {
        push_back_element[j] = 1;
      }
    }
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
  ret.erase(std::remove_if(ret.begin(), ret.end(),
                           [&](const std::vector<int> &v) -> bool {
                             for (int i = 0; i < (int)now.size(); ++i) {
                               if (now[i] == 0)
                                 continue;
                               if (now[i] == v[i])
                                 continue;
                               return true;
                             }
                             return false;
                           }),
            ret.end());
  return ret;
}
int main() {
  int h, w;
  std::string line;
  {
    getline(std::cin, line);
    std::istringstream stream(line);
    stream >> h >> w;
  }
  // free: 0, iscolored: 1, isnotcolored: 2
  std::vector<std::vector<int>> board(h, std::vector<int>(w, 0));
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
  std::cout << "solving" << std::endl;
  bool ismodified = false;
  while (true) {
    ismodified = false;
    for (int i = 0; i < h; ++i) {
      auto avaliblelist = std::move(genlist(hinfo[i], board[i]));
      std::vector<int> next(w, 0);
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
          if (board[i][j] == next[j])
            continue;
          else {
            std::cout << "conflict\n";
            return 0;
          }
        }
      }
    }
    for (int i = 0; i < w; ++i) {
      std::vector<int> verticalboard(h);
      for (int j = 0; j < h; ++j) {
        verticalboard[j] = board[j][i];
      }
      auto availiblelist = std::move(genlist(winfo[i], verticalboard));
      std::vector<int> next(h, 0);
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
          if (board[j][i] == next[j]) {
            continue;
          } else {
            std::cout << "conflict\n";
            return 0;
          }
        }
      }
    }
    if (!ismodified)
      break;
  }
  bool uncertain_places = false;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      std::cout << (board[i][j] == 1 ? '#' : board[i][j] == 2 ? '.' : '?');
      if (board[i][j] == 0)
        uncertain_places = true;
    }
    std::cout << '\n';
  }
}