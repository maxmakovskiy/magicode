import re
import functools


def solve_2(lines: list[str]):
    net = [[ch for ch in line] for line in lines]

    ops = re.split(r"\s+", lines[4].strip())
    ops_idx = 0

    acc = 0

    operands = []
    for i in range(len(net[0])):
        num = ""
        for idx in range(4):
            num += net[idx][i]

        if len(num.strip()) == 0:
            if ops[ops_idx] == "+":
                acc += sum([int(o) for o in operands])
            else:
                acc += functools.reduce(
                    lambda x,y: x * y,
                    [int(o) for o in operands], 1)
            ops_idx += 1
            operands = []
        else:
            operands.append(num)

    if len(operands) > 0:
        if ops[ops_idx] == "+":
            acc += sum([int(o) for o in operands])
        else:
            acc += functools.reduce(
                lambda x,y: x * y,
                [int(o) for o in operands], 1)

    return acc


def main():
    # sample = (
    #     "123 328  51 64 \n"
    #     " 45 64  387 23 \n"
    #     "  6 98  215 314\n"
    #     "*   +   *   +  "
    # )

    with open("day6.txt", "r") as f:
        sample = f.read()

    lines = sample.splitlines()

    print(f"Part 2 : {solve_2(lines)}")

    pattern = r"\s+"
    regexp = re.compile(pattern)
    operands1 = regexp.split(lines[0].strip())
    operands2 = regexp.split(lines[1].strip())
    operands3 = regexp.split(lines[2].strip())
    operands4 = regexp.split(lines[3].strip())
    ops = regexp.split(lines[4].strip())

    assert(len(operands1) == len(operands2) == len(operands3) == len(operands4) == len(ops))

    acc = 0
    for i, op in enumerate(ops):
        if op == "+":
            acc += int(operands1[i]) + int(operands2[i]) + int(operands3[i]) + int(operands4[i])
        else:
            acc += int(operands1[i]) * int(operands2[i]) * int(operands3[i]) * int(operands4[i])

    print(f"Part 1: {acc}")


if __name__ == "__main__":
    main()
