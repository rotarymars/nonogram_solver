#include <algorithm>
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
  std::vector<int> minindex(info.size()), maxindex(info.size());
  minindex[0] = 0;
  for (int i = 1; i < info.size(); ++i) {
    minindex[i] = minindex[i - 1] + info[i - 1] + 1;
  }
  maxindex.back() = now.size() - info.back();
  for (int i = info.size() - 2; i >= 0; --i) {
    maxindex[i] = maxindex[i + 1] - info[i]-1;
  }
  std::vector nowindex = minindex;
  std::vector<std::vector<int>> ret;
  while (true) {
    std::vector<int> push_back_element(now.size(), 2);
    for (int i = 0; i < info.size(); ++i) {
      for (int j = nowindex[i]; j < nowindex[i] + info[i]; ++j) {
        push_back_element[j] = 1;
      }
    }
    ret.push_back(push_back_element);
    int turn_back_point = -1;
    for (int i = info.size() - 1; i >= 0; --i) {
      if(nowindex[i]>=maxindex[i])continue;
      turn_back_point = i;
      break;
    }
    if (turn_back_point == -1) {
      break;
    }
    for (int i = turn_back_point; i < info.size(); ++i) {
      if (i == turn_back_point) [[unlikely]] {
        ++nowindex[i];
      } else {
        nowindex[i] = nowindex[i - 1] + info[i-1] + 1;
      }
    }
  }
  ret.erase(std::remove_if(
    ret.begin(),
    ret.end(),
    [&](const std::vector<int>&v)->bool{
      for(int i=0;i<now.size();++i){
        if(now[i]==0)continue;
        if(now[i]==v[i])continue;
        return true;
      }
      return false;
    }
  ),ret.end());
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
  bool ismodified = false;
  auto temp=genlist(hinfo[0],board[0]);
  for(int i=0;i<temp.size();++i){
    for(int j=0;j<temp[i].size();++j){
      std::cout<<temp[i][j]<<' ';
    }
    std::cout<<'\n';
  }
  // while (true) {
  //   for (int i = 0; i < h; ++i) {
  //     auto avaliblelist = std::move(genlist(hinfo[i], board[i]));
  //   }
  //   if (!ismodified)
  //     break;
  // }
}