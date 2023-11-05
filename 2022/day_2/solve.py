import pdb
import re

# // A & X -> 0b0001'0001
# // B & Y -> 0b0010'0010
# // C & Z -> 0x0011'0011
# // win:
# // A & Y -> 0b0010'0001
# // B & Z -> 0b0011'0010
# // C & X -> 0x0001'0011


def compute_pattern(pattern):
    # pdb.set_trace()
    chs = list(filter(
        lambda i: i != " " or i != "\n",
        pattern.split()
    ))
    if chs[1] == "X":
        return 1
    elif chs[1] == "Y":
        return 2
    else:
        return 3



def compute_score(line):
    result = 0
    patterns_draft = [
        r"A X", r"B Y", r"C Z"
    ]
    patterns_wins = [
        r"A Y", r"B Z", r"C X"
    ]
    # pdb.set_trace()
    for p in patterns_draft + patterns_wins:
        if re.match(p, line):
            if p in patterns_draft:
                result += 3
            else:
                result += 6

    # pdb.set_trace()
    result += compute_pattern(line) 

    return result


def main():
    total = 0
    with open("input.txt", "rt") as f:
        for line in f:
            if line != "\n":
                t = compute_score(line)
                print(t)
                total += t

    print(f"in total {total}")

if __name__ == "__main__":
    main()