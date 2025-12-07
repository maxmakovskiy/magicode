def solve(rotations: list[str]) -> int:
    dial = 50
    dial_limit = 100
    zero_counter = 0

    for r in rotations:
        if len(r) < 2:
            continue

        is_left = r[0] == "L"
        rot_value = int(r[1:])

        dial += (-1 if is_left else 1) * rot_value
        dial %= dial_limit

        if dial == 0:
            zero_counter += 1

    return zero_counter


def solve_2(rotations: list[str]) -> int:
    dial = 50
    dial_limit = 100
    zero_counter = 0

    for r in rotations:
        if len(r) < 2:
            continue

        is_left = r[0] == "L"
        rot_value = int(r[1:])

        for _ in range(rot_value):
            dial += (-1 if is_left else 1)
            dial %= dial_limit

            if dial == 0:
                zero_counter += 1

    return zero_counter


def main():
    # rotations = [
    #     "L68", "L30", "R48", "L5",
    #     "R60", "L55", "L1", "L99",
    #     "R14", "L82"
    # ]
    # print(solve_2(rotations))

    with open("day_1_part1.txt", "r") as f:
        # zero_counter = solve(f.read().split("\n"))
        zero_counter = solve_2(f.read().split("\n"))
        print(f"Dial has pointed at zero {zero_counter} times")


if __name__ == "__main__":
    main()