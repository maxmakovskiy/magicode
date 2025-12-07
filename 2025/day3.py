import itertools

def gen_pairs(source: str) -> list[str]:
    pairs = []

    for j in range(len(source)):
        curr = source[j]

        for i in range(j + 1, len(source)):
            pairs.append(curr + source[i])

    return pairs


def find_biggest_joltage(battery: str) -> int:
    pairs = [int(p) for p in gen_pairs(battery)]

    for i in range(len(pairs)):
        min_pos = i
        for j in range(i + 1, len(pairs)):
            if pairs[j] < pairs[min_pos]:
                min_pos = j
        tmp = pairs[min_pos]
        pairs[min_pos] = pairs[i]
        pairs[i] = tmp

    return pairs[-1]


def find_biggest_joltage_2(battery: str) -> int:
    """
    To naive to be true
    """
    combs = list(itertools.combinations(battery, 12))
    numbers = [int("".join(cmb)) for cmb in combs]
    return sorted(numbers, reverse=True)[0]


def find_biggest_joltage_3(battery: str, rest: int) -> str:
    if rest > 0:
        i_biggest = max(enumerate(battery[:-rest]), key=lambda x: x[1])[0]
        return battery[i_biggest] + find_biggest_joltage_3(
            battery[i_biggest + 1:], rest-1)
    else:
        return max(battery)

def main():
    sample = ("987654321111111\n"
              "811111111111119\n"
              "234234234234278\n"
              "818181911112111\n")

    with open("day_3_part1.txt", "r") as f:
        r1 = 0
        r2 = 0
        for battery in f.read().strip().split("\n"):
        # for battery in sample.strip().split("\n"):
            r1 += find_biggest_joltage(battery) # real: 17445
            r2 += int(find_biggest_joltage_3(battery, 11)) # sample: 3121910778619

        print(f"Part 1: {r1}")
        print(f"Part 2: {r2}")


if __name__ == "__main__":
    main()
