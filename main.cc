#include <array>
#include <string_view>
#include <tuple>
#include <ranges>
#include <iostream>
#include <print>

#ifndef TURNS
#define TURNS ""
#endif

#ifndef COUNT_TURNS
#define COUNT_TURNS 0
#endif

consteval int parse_int(auto&& range)
{
    int result = 0;

    for (char c : range)
        result = result * 10 + (c - '0');

    return result;
}

template<size_t SIZE>
consteval auto parse_moves(std::string_view str) {
	std::array<std::tuple<int,int>, SIZE> moves;

    for (auto [i, move] : str | std::views::split('.') | std::views::enumerate) {
        if (move.empty()) {
            continue;
	}

        int values[2]{};

        for (auto [j, value] : move | std::views::split(',') | std::views::enumerate) {
            values[j] = parse_int(value);
        }

        moves[i] = {values[0], values[1]};
    }

	return moves;
}

consteval auto build_field_from_moves(auto moves) {
	std::array<char, 9> field;
	field.fill(' ');

	for (const auto [index, value] : std::views::enumerate(moves)) {
		const auto [x, y] = value;
		const auto i = x + (y * 3);
		if (index % 2 == 0) {
			field[i] = 'O';
		} else {
			field[i] = 'X';
		}
	} 

	return field;
}

consteval auto build_empty_field() {
	std::array<char, 9> field;
	field.fill(' ');
	return field;
}

void print_field(auto field) {
	std::println("{}|{}|{}", field[0], field[1], field[2]);
	std::println("-----");
	std::println("{}|{}|{}", field[3], field[4], field[5]);
	std::println("-----");
	std::println("{}|{}|{}", field[6], field[7], field[8]);
}

consteval std::tuple<bool, std::string_view> check_win(auto field) {
    constexpr std::array<std::array<int, 3>, 8> wins = {{
        {{0, 1, 2}},
        {{3, 4, 5}},
        {{6, 7, 8}},

        {{0, 3, 6}},
        {{1, 4, 7}},
        {{2, 5, 8}},

        {{0, 4, 8}},
        {{2, 4, 6}}
    }};

    for (const auto& line : wins) {
        char a = field[line[0]];
        char b = field[line[1]];
        char c = field[line[2]];

        if (a != ' ' && a == b && b == c) {
            if (a == 'X') {
				return {true, "X"};
			}

            if (a == 'O') {
				return {true, "O"};
			}
        }
    }

    return {false, ""};
}

void print_turn(int count_turns) {
	char turn = 'O';
	if (count_turns % 2 != 0) {
		turn = 'X';
	}
	
	std::println("Turn {} - {}", count_turns, turn);
}

std::tuple<int, int> get_user_input(const auto& field) {
	int x, y;
	
	while (true) {
		std::cin >> x >> y;
		if (x > 2 || x < 0 || y > 2 || y < 0) {
			std::println("X and Y must be [0;2]");
			continue;
		}

		if (field[x + (y * 3)] != ' ') {
			std::println("This cell is not empty");
			continue;
		}

		break;
	}

	return {x, y};
}

template <int count_turns>
consteval auto get_field(auto all_moves) {
	if constexpr (count_turns == 0) {
		auto field = build_empty_field();
		return field;
	}

	auto moves = parse_moves<count_turns>(all_moves);
	auto field = build_field_from_moves(moves);
	return field;
}

int main() {
	constexpr std::string_view all_moves = TURNS;
	constexpr int count_turns = COUNT_TURNS;
	constexpr auto field = get_field<count_turns>(all_moves);
	print_field(field);
	
	if constexpr (count_turns != 0) {
		const auto [is_over, who_win] = check_win(field);
		if (is_over) {
			std::println("Win {}", who_win);
			return 0;
		}
	}
		
	print_turn(count_turns);
	const auto [x, y] = get_user_input();

}
