//
// Created by Timothy Herchen on 1/3/22.
//
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <sstream>

enum {
	NOP = 0,
	FLIP = 1,
	SET_ZERO = 2,
	SET_ONE = 3
};

using Operation = int;
using PlayerList = std::vector<int>; // int are the indices

// 4 -> ABCD
const int PLAYER_COUNT = 4;

// B -> 0, C -> 1, D -> 2 (min is 0, max is PLAYER_COUNT - 2)
using PlayerAssignments = std::array<Operation, PLAYER_COUNT - 1>;

const char* op_to_string(Operation o) {
	switch (o) {
		case NOP: return "NOP";
		case FLIP: return "FLIP";
		case SET_ZERO: return "SET_ZERO";
		case SET_ONE: return "SET_ONE";
		default:
			throw std::runtime_error("WTF");
	}
}


PlayerList player_list_from_string(const char* s) {
	std::vector<int> out;

	for (const char* c = s; *c != '\0'; ++c) {
		out.push_back(static_cast<int>(*c - 'B'));
	}

	for (int i = 0; i < out.size(); ++i) {
		int player_i = out[i];
		if (player_i < 0 || player_i >= PLAYER_COUNT - 1)
			throw std::runtime_error("Invalid player " + std::to_string(static_cast<char>(player_i + 'B')) + " at index " + std::to_string(i));
	}

	return out;
}

std::string string_from_player_list(const PlayerList& l) {
	std::string out;

	for (int p : l) {
		out += static_cast<char>(p);
	}

	return out;
}

// Evaluate a player chain with given player assignments on an input
bool evaluate_chain(const PlayerList& players, const PlayerAssignments& assignments, bool in) {
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

// Generate permutations THIS CODE SUCKS SORRY

template <typename Lambda>
void generate_assignments(Lambda callback) {
	PlayerAssignments assignments{};

	int i = 0;
	while (i >= 0) {
		if (assignments[i] > 3) {
			assignments[i] = 0;
			--i;
			assignments[i]++;

			if (i == 0 && assignments[i] == 4)
				return;

			continue;
		}

		callback(assignments);

		if (i < PLAYER_COUNT - 2)
			++i;
		else
			assignments[i]++;
	}
}

struct KnownResult {
	PlayerList players;
	bool in;
	bool out;
};

// Whether a given assignment works for a result
bool assignment_satisfies_result(const PlayerAssignments& ass, KnownResult res) {
	return res.out == evaluate_chain(res.players, ass, res.in);
}

std::string pp_string_assignment(const PlayerAssignments& ass) {
	// Looks something like B: NOP, C: ...
	std::stringstream ss;

	for (int i = 0; i < PLAYER_COUNT - 1; ++i) {
		char player = i + 'B'; // technically implementation defined....

		ss << player << ": " << op_to_string(ass[i]);
		if (i != PLAYER_COUNT - 2) ss << ", ";
	}

	return ss.str();
}

// Attempt the strategy on a given assignment
int attempt_assignment(const PlayerAssignments& ass) {
	// ones that have yet to be disproven, and that we will try to whittle down
	std::vector<PlayerAssignments> candidate_assignments;

	// Each round we have to select two things: the permutation BACD, and our test value in the previous round
}

int main() {
	std::vector<KnownResult> results = {
					// Input of true into BC is known to be true
					{ .players = player_list_from_string("BC"), .in = true, .out = true },
					{ .players = player_list_from_string("B"), .in = true, .out = false },
					{ .players = player_list_from_string("DC"), .in = true, .out = true },
					{ .players = player_list_from_string("DC"), .in = false, .out = false },
	};

	generate_assignments([&] (const PlayerAssignments& ass) {
		if (std::all_of(results.begin(), results.end(), [&](const KnownResult& r) { return assignment_satisfies_result(ass, r); })) {
			std::cout << pp_string_assignment(ass) << '\n';
		}
	});
}
