#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using CellState = char;

std::vector<std::vector<CellState>> generateValidConfigurations(const std::vector<int> &constraints,
                                                                const std::vector<CellState> &current) {
  if (constraints.empty()) {
    return {std::vector<CellState>(current.size(), 2)};
  }

  if (std::none_of(current.begin(), current.end(), [](CellState c) { return c == 0; })) {
    return {current};
  }

  const int line_length = current.size();
  const int num_constraints = constraints.size();

  std::vector<int> min_positions(num_constraints);
  std::vector<int> max_positions(num_constraints);

  min_positions[0] = 0;
  for (int i = 1; i < num_constraints; ++i) {
    min_positions[i] = min_positions[i - 1] + constraints[i - 1] + 1;
  }

  max_positions[num_constraints - 1] = line_length - constraints[num_constraints - 1];
  for (int i = num_constraints - 2; i >= 0; --i) {
    max_positions[i] = max_positions[i + 1] - constraints[i] - 1;
  }

  std::vector<int> positions = min_positions;
  std::vector<std::vector<CellState>> valid_configs;

  while (true) {
    std::vector<CellState> candidate(line_length, 2);
    for (int i = 0; i < num_constraints; ++i) {
      for (int j = 0; j < constraints[i]; ++j) {
        candidate[positions[i] + j] = 1;
      }
    }

    bool is_valid = true;
    for (int i = 0; i < line_length && is_valid; ++i) {
      if (current[i] != 0 && current[i] != candidate[i]) {
        is_valid = false;
      }
    }

    if (is_valid) {
      valid_configs.push_back(std::move(candidate));
    }

    int increment_index = -1;
    for (int i = num_constraints - 1; i >= 0; --i) {
      if (positions[i] < max_positions[i]) {
        increment_index = i;
        break;
      }
    }

    if (increment_index == -1) break;

    ++positions[increment_index];
    for (int i = increment_index + 1; i < num_constraints; ++i) {
      positions[i] = positions[i - 1] + constraints[i - 1] + 1;
    }
  }

  return valid_configs;
}

template<typename LineExtractor>
bool processLine(std::vector<std::vector<CellState>>& board, int index, int line_length,
                 const std::vector<int>& constraints, LineExtractor extractor) {
  std::vector<CellState> current_line(line_length);
  extractor(board, index, current_line);

  auto valid_configs = generateValidConfigurations(constraints, current_line);
  if (valid_configs.empty()) return false;

  std::vector<CellState> deduced(line_length, 0);
  for (int pos = 0; pos < line_length; ++pos) {
    bool all_filled = true, all_empty = true;
    for (const auto& config : valid_configs) {
      if (config[pos] == 1) all_empty = false;
      if (config[pos] == 2) all_filled = false;
    }
    if (all_filled) deduced[pos] = 1;
    else if (all_empty) deduced[pos] = 2;
  }

  bool modified = false;
  for (int pos = 0; pos < line_length; ++pos) {
    CellState& cell = (extractor == [](auto& b, int i, auto& line) {
      for (int j = 0; j < line.size(); ++j) line[j] = b[i][j];
    }) ? board[index][pos] : board[pos][index];

    if (cell == 0 && deduced[pos] != 0) {
      cell = deduced[pos];
      modified = true;
    } else if (cell != 0 && deduced[pos] != 0 && cell != deduced[pos]) {
      return false;
    }
  }
  return !modified;
}

std::vector<std::vector<CellState>> solveNonogram(int h, int w,
    const std::vector<std::vector<int>>& row_constraints,
    const std::vector<std::vector<int>>& col_constraints) {

  std::vector<std::vector<CellState>> board(h, std::vector<CellState>(w, 0));

  auto extract_row = [](std::vector<std::vector<CellState>>& b, int i, std::vector<CellState>& line) {
    for (int j = 0; j < line.size(); ++j) line[j] = b[i][j];
  };

  auto extract_col = [](std::vector<std::vector<CellState>>& b, int i, std::vector<CellState>& line) {
    for (int j = 0; j < line.size(); ++j) line[j] = b[j][i];
  };

  bool converged = false;
  while (!converged) {
    converged = true;

    for (int i = 0; i < h; ++i) {
      std::vector<CellState> row(w);
      for (int j = 0; j < w; ++j) row[j] = board[i][j];

      auto valid_configs = generateValidConfigurations(row_constraints[i], row);
      if (valid_configs.empty()) return board;

      for (int j = 0; j < w; ++j) {
        bool all_filled = true, all_empty = true;
        for (const auto& config : valid_configs) {
          if (config[j] == 1) all_empty = false;
          if (config[j] == 2) all_filled = false;
        }

        CellState deduced = all_filled ? 1 : (all_empty ? 2 : 0);
        if (board[i][j] == 0 && deduced != 0) {
          board[i][j] = deduced;
          converged = false;
        } else if (board[i][j] != 0 && deduced != 0 && board[i][j] != deduced) {
          return board;
        }
      }
    }

    for (int i = 0; i < w; ++i) {
      std::vector<CellState> col(h);
      for (int j = 0; j < h; ++j) col[j] = board[j][i];

      auto valid_configs = generateValidConfigurations(col_constraints[i], col);
      if (valid_configs.empty()) return board;

      for (int j = 0; j < h; ++j) {
        bool all_filled = true, all_empty = true;
        for (const auto& config : valid_configs) {
          if (config[j] == 1) all_empty = false;
          if (config[j] == 2) all_filled = false;
        }

        CellState deduced = all_filled ? 1 : (all_empty ? 2 : 0);
        if (board[j][i] == 0 && deduced != 0) {
          board[j][i] = deduced;
          converged = false;
        } else if (board[j][i] != 0 && deduced != 0 && board[j][i] != deduced) {
          return board;
        }
      }
    }
  }
  return board;
}

#ifndef NO_MAIN
int main() {
  int height, width;
  std::string line;

  std::getline(std::cin, line);
  std::istringstream dimensions_stream(line);
  dimensions_stream >> height >> width;

  std::vector<std::vector<int>> row_constraints(height), col_constraints(width);

  for (int i = 0; i < height; ++i) {
    std::getline(std::cin, line);
    std::istringstream stream(line);
    int constraint;
    while (stream >> constraint) {
      row_constraints[i].push_back(constraint);
    }
  }

  for (int i = 0; i < width; ++i) {
    std::getline(std::cin, line);
    std::istringstream stream(line);
    int constraint;
    while (stream >> constraint) {
      col_constraints[i].push_back(constraint);
    }
  }

  auto solution = solveNonogram(height, width, row_constraints, col_constraints);

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (j > 0) std::cout << ' ';
      std::cout << (solution[i][j] == 1 ? '#' :
                    solution[i][j] == 2 ? '.' : '?');
    }
    std::cout << '\n';
  }

  return 0;
}
#endif
