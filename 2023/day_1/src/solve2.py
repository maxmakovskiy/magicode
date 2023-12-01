import re


def check(string):
    pattern = r"one|two|three|four|five|six|seven|eight|nine|[0-9]"
    res = re.search(pattern, string)
    if res:
        return (True, res[0])
    return (False, "")


def search_first(string):
    res = ""
    for i in range(0, len(string)):
        (is_match, match) = check(string[:i])
        if is_match:
            return match
    return res


def search_last(string):
    res = ""
    length = len(string)
    for i in range(0, length):
        (is_match, match) = check(string[(length-(i+1))-length:])
        if is_match:
            return match
    return res


def to_num(string):
    if len(string) == 1:
        return int(string)

    match string:
        case "one":
            return 1
        case "two":
            return 2
        case "three":
            return 3
        case "four":
            return 4
        case "five":
            return 5
        case "six":
            return 6
        case "seven":
            return 7
        case "eight":
            return 8
        case "nine":
            return 9


def solve(input):
    first = search_first(input)
    last = search_last(input)
    return to_num(first) * 10 + to_num(last)


def test_solve():
    sample = "eight5fourtwotwo"
    result = solve(sample)
    assert result == 82


def main():
    test_solve()
    
    res = 0
    with open("input.txt", "r") as file:
        for l in file:
            res += solve(l)
    print(res)


if __name__ == "__main__":
    main()

    
