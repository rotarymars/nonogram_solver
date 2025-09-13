#define TESTING
#include <gtest/gtest.h>
#include <vector>
#include <string>

using CellState = char;

std::vector<std::vector<CellState>> genlist(const std::vector<int> &info,
                                      const std::vector<CellState> &now);

std::vector<std::vector<CellState>> solveNonogram(int h, int w,
    const std::vector<std::vector<int>>& hinfo,
    const std::vector<std::vector<int>>& winfo);

std::vector<std::string> toStringList(const std::vector<std::vector<CellState>> &list) {
    std::vector<std::string> result;
    for (const auto &row : list) {
        std::string s;
        bool first = true;
        for (const auto &cell : row) {
            if(first) {
                first = false;
            }
            else {
                s += ' ';
            }
            if (cell == 1) {
                s += '#';
            } else if (cell == 2) {
                s += '.';
            } else {
                s += '?';
            }
        }
        result.push_back(s);
    }
    return result;
}

TEST(NonogramTest, SolveFace) {
    std::vector<std::vector<int>> hinfo = {{2, 2}, {2, 2}, {3}, {3}, {3}};
    std::vector<std::vector<int>> winfo = {{2}, {5}, {3}, {5}, {2}};

    auto result = solveNonogram(5, 5, hinfo, winfo);

    EXPECT_EQ(toStringList(result), std::vector<std::string>({
        "# # . # #",
        "# # . # #",
        ". # # # .",
        ". # # # .",
        ". # # # ."
    }));
}

TEST(NonogramTest, SolveCu) {
    std::vector<std::vector<int>> hinfo = {{4}, {7}, {3, 3}, {2, 2}, {2}, {2}, {2}, {2, 1, 1}, {2, 1, 1}, {2, 1, 1}, {2, 3, 1}, {3, 4, 1}, {7, 1, 2}, {4, 2, 1}};
    std::vector<std::vector<int>> winfo = {{10}, {12}, {3, 3}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {3, 3}, {2, 2}, {6}, {1}, {1}, {1}, {7}};

    auto result = solveNonogram(14, 14, hinfo, winfo);

    EXPECT_EQ(toStringList(result), std::vector<std::string>({
        ". . # # # # . . . . . . . .",
        ". # # # # # # # . . . . . .",
        "# # # . . . # # # . . . . .",
        "# # . . . . . # # . . . . .",
        "# # . . . . . . . . . . . .",
        "# # . . . . . . . . . . . .",
        "# # . . . . . . . . . . . .",
        "# # . . . . . . . ? . ? . #",
        "# # . . . . . . . # . . . #",
        "# # . . . . . . . # . . . #",
        "# # . . . . . # # # . . . #",
        "# # # . . . # # # # . . . #",
        ". # # # # # # # . # . . # #",
        ". . # # # # . . . ? # ? . #"
    }));
}

TEST(NonogramTest, SolveSeal) {
    std::vector<std::vector<int>> hinfo = {{10}, {7, 3}, {16}, {12, 4}, {9, 2}, {11, 2}, {18}, {8, 4}, {6, 4}, {5, 5}, {4, 2, 2}, {4, 3, 1}, {3, 2, 1}, {3, 2}, {3, 2}, {2, 1, 4}, {2, 1, 4}, {2, 1, 4}, {3, 1, 4}, {5, 4}, {9}, {10}, {5}, {6}, {6}};
    std::vector<std::vector<int>> winfo = {{2}, {5}, {2, 8}, {3, 3, 4}, {2, 7, 3}, {1, 4, 7}, {1, 4, 6}, {1, 3, 7}, {1, 4, 6}, {2, 3, 6}, {2, 3, 6}, {3, 3, 6}, {4, 3, 5}, {5, 4, 4}, {6, 4, 3}, {7, 4, 2}, {8, 4, 1}, {9, 4, 1}, {10, 4}, {11, 4}, {7}, {3}, {3}, {2}, {1}};

    auto result = solveNonogram(25, 25, hinfo, winfo);

    EXPECT_EQ(toStringList(result), std::vector<std::string>({
        ". . . . . . . . . . . . # # # # # # # # . . . . .",
        ". . . . . . . . . . # # # # # # # . # # # . . . .",
        ". . . . . . . . # # # # # # # # # # # # # # # # .",
        ". . . . . . . # # # # # # # # # # # . . . # # # #",
        ". . . # # # # # # # # # # # # # . . . # # . . . .",
        ". # # # # # # # # # # # # # # . . # # . . . . . .",
        "# # # # # # # # # # # # # # # # # # . . . . . . .",
        ". . . # # # # # # # # . # # # # . . . . . . . . .",
        ". . . . # # # # # # . . . # # # # . . . . . . . .",
        ". . . . # # # # # . . . # # # # # . . . . . . . .",
        ". . . . # # # # . . . . # # . # # . . . . . . . .",
        ". . . . # # # # . . . # # # . . # . . . . . . . .",
        ". . . . # # # . . . # # . # . . . . . . . . . . .",
        ". . . . # # # . . # # . . . . . . . . . . . . . .",
        ". . . . # # # . . # # . . . . . . . . . . . . . .",
        ". . . . . # # . . # . . . . . . . . # # # # . . .",
        ". . . . . # # . . # . . . . . . . # # # # . . . .",
        ". . . . . . # # . # . . . . . . # # # # . . . . .",
        ". . . . . . # # # . # . . . . . # # # # . . . . .",
        ". . . . . . . # # # # # . . . # # # # . . . . . .",
        ". . . . . . . . # # # # # # # # # # # . . . . . .",
        ". . . . . . . . . . # # # # # # # # # # . . . . .",
        ". . . . . . . . . . . . . . . . # # # # # . . . .",
        ". . . . . . . . . . . . . . . . . # # # # # . . .",
        ". . . . . . . . . . . . . . . . . . # # # # # # ."
    }));
}

