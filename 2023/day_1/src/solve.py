import re
# import pdb


def preprocess(input):
    numbers = {
        "one": "1", "two": "2", "three": "3", "four": "4",
        "five": "5", "six": "6", "seven": "7", "eight": "8", "nine": "9",
        # "one": 1, "two": 2, "three": 3, "four": 4,
        # "five": 5, "six": 6, "seven": 7, "eight": 8, "nine": 9,
        # "1": 1, "2": 2, "3": 3, "4": 4,
        # "5": 5, "6": 6, "7": 7, "8": 8, "9": 9,

    }
    pattern = r"one|two|three|four|five|six|seven|eight|nine|[0-9]"

    res = ""
    for raw in re.findall(pattern, input):
        res += numbers[raw] if len(raw) > 1 else raw
    return res


def test_preprocess():
    sample = "eight5fourtwotwo"
    result = preprocess(sample)
    assert result == "85422"


def solve(input):
    ready = preprocess(input)
    return int(ready[0]) * 10 + int(ready[-1])


def test_solve():
    sample = "eight5fourtwotwo"
    result = solve(sample)
    assert result == 82


def main():
    test_preprocess()
    test_solve()
    
    res = 0
    with open("input.txt", "r") as file:
        for l in file:
            res += solve(l)
    print(res)


if __name__ == "__main__":
    main()

    
