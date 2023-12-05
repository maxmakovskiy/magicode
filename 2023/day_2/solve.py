from typing import NamedTuple
import re
import functools


RESERVED_ID = -1

class Round(NamedTuple):
    red: int = 0
    green: int = 0
    blue: int = 0
 

class Game(NamedTuple):
    id: int
    rounds: list[Round]


def new_game(raw):
    parts = list(map(
        lambda item: item.strip(),
        re.split(r"[:;]", raw)
    ))
    
    game_id = int(parts[0][5:])
    rounds = []
    for raw_round in parts[1:]:
        p_red = "([0-9]+ red)"
        p_blue = "([0-9]+ blue)"
        p_green = "([0-9]+ green)"
        g_red = re.search(p_red, raw_round)
        g_blue = re.search(p_blue, raw_round)
        g_green = re.search(p_green, raw_round)

        red = 0
        if g_red:
            red = int(str(g_red.group(0))[:-4])
        blue = 0
        if g_blue:
            blue = int(str(g_blue.group(0))[:-5])
        green = 0
        if g_green:
            green = int(str(g_green.group(0))[:-6])

        rounds.append(Round(red=red, blue=blue, green=green))
        # check for every match
        # if found somethin, so parse
        # after all rounds return new Game obj

    return Game(id=game_id, rounds=rounds)


def game_fits(config: Round, fitable: Game) -> bool:
    for round in fitable.rounds:
        if (config.blue  < round.blue or
            config.red   < round.red  or
            config.green < round.green
        ):
            return False

    return True


def solve(line):
    ## CONFIGURATION
    # 12 red cubes, 13 green cubes, and 14 blue cubes
    config = Round(
         red=12, green=13, blue=14
    )
    game = new_game(line)
    is_possible = game_fits(config, game)
    return game.id if is_possible else 0


def find_min_round(game: Game) -> Round:
    red = 0
    green = 0
    blue = 0
    for round in game.rounds:
        red =  round.red if red < round.red else red
        green =  round.green if green < round.green else green
        blue =  round.blue if blue < round.blue else blue
    return Round(red=red, green=green, blue=blue)


def solve2(lines):
    res = 0
    for line in lines:
        game = new_game(line)
        min = find_min_round(game)
        res += (min.red * min.blue * min.green)
    return res


def compare_games(game1, game2):
    if (game1.id != game2.id or
        len(game1.rounds) != len(game2.rounds)
    ):
        return False

    for i in range(len(game1.rounds)):
        left = game1.rounds[i]
        right = game2.rounds[i]
        if (
            left.red != right.red or
            left.blue != right.blue or
            left.green != right.green
        ):
            return False
    return True


def new_game_test():
    sample = "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"
    result = new_game(sample)
    expected = Game(
        id=1,
        rounds=[
            Round(red=4, blue=3, green=0),
            Round(red=1, blue=6, green=2),
            Round(red=0, blue=0, green=2),
        ]
    )
    # pdb.set_trace()
    assert compare_games(result, expected)


def solve_test():
    samples = [
        "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green",
        "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue",
        "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red",
        "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red",
        "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green",
    ]
    result = functools.reduce(
        lambda acc, item: acc+item,
        [solve(x) for x in samples]
    )
    assert result == 8


def find_min_round_test1():
    sample = "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"
    result = find_min_round(new_game(sample))
    expected = Round(red=4, green=2, blue=6)
    assert (
        result.red == expected.red and
        result.green == expected.green and
        result.blue == expected.blue
    )

def find_min_round_test2():
    sample = "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue"
    result = find_min_round(new_game(sample))
    expected = Round(red=1, green=3, blue=4)
    assert (
        result.red == expected.red and
        result.green == expected.green and
        result.blue == expected.blue
    )

    
def find_min_round_test3():
    sample = "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red"
    result = find_min_round(new_game(sample))
    expected = Round(red=20, green=13, blue=6)
    assert (
        result.red == expected.red and
        result.green == expected.green and
        result.blue == expected.blue
    )

    
def find_min_round_test4():
    sample = "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red"
    result = find_min_round(new_game(sample))
    expected = Round(red=14, green=3, blue=15)
    assert (
        result.red == expected.red and
        result.green == expected.green and
        result.blue == expected.blue
    )


def find_min_round_test5():
    sample = "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"
    result = find_min_round(new_game(sample))
    expected = Round(red=6, green=3, blue=2)
    assert (
        result.red == expected.red and
        result.green == expected.green and
        result.blue == expected.blue
    )


def solve2_test():
    sample = [
        "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green",
        "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue",
        "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red",
        "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red",
        "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green",
    ]
    result = solve2(sample)
    assert result == 2286


def main():
    new_game_test()
    solve_test()
    find_min_round_test1()
    find_min_round_test2()
    find_min_round_test3()
    find_min_round_test4()
    find_min_round_test5()
    solve2_test()
    
    result = 0
    result2 = 0
    with open("input.txt", "r") as file:
        lines = file.readlines()
        result2 = solve2(lines)
        for line in file:
            result += solve(line)

    print(f"result for 1st part {result}")
    print(f"result for 2nd part {result2}")


if __name__ == "__main__":
    main()
 
