//
// Created by Timothy Herchen on 1/3/22.
//
#include <iostream>
#include <vector>
#include <array>

enum {
	NOP = 0,
	FLIP = 1,
	SET_ZERO = 2,
	SET_ONE = 3
};

using Operation = int;

// 4 -> ABCD
const int PLAYER_COUNT = 4;

// B -> 0, C -> 1
using PlayerAssignments = std::array<Operation, PLAYER_COUNT - 1>;

// [B]A[CDC]AB

class OpChain {
	std::vector<int> players;
	PlayerAssignments assignments;

	bool evaluate_chain(bool in) {
		for (int player : players) {
			switch (assignments[player]) {
				case FLIP:
					in = !in;
					break;
				case SET_ZERO:
					in = false;
					break;
				case SET_ONE:
					in = true;
					break;
				case NOP:
					break;
			}
		}

		return in;
	}
};

// Generate permutations

template <typename Lambda>
void generate_assignments(Lambda callback) {
	PlayerAssignments assignments;

	int i = 0;
	while (i >= 0) {
		if (assignments[i] == 3) {
			assignments[i] = 0;
			--i;
			continue;
		}

		callback(assignments);
		assignments[i]++;

		if (i < PLAYER_COUNT - 1) ++i;
	}
}

int main() {
	generate_assignments([&] (PlayerAssignments ass) {
		for (int i = 0; i < PLAYER_COUNT - 1; ++i) {
			std::cout << ass[i] << ' ';
		}

		std::cout << '\n';
	});
	std::cout << "Hi\n";
}
