class Range:
    def __init__(self, left: int, right: int):
        self.left = left
        self.right = right

    def __contains__(self, val) -> bool:
        return self.left <= val <= self.right

    def __str__(self):
        return f"({self.left}-{self.right})"

    def __repr__(self):
        return self.__str__()


def folding(ranges: list[Range]) -> list[Range]:
    if len(ranges) == 2:
        if ranges[1].left in ranges[0] and ranges[1].right in ranges[0]:
            return [ranges[0]]
        elif ranges[1].left in ranges[0]:
            return [Range(ranges[0].left, ranges[1].right)]
        else:
            return ranges

    tmp = folding(ranges[1:])

    if tmp[0].left in ranges[0] and tmp[0].right in ranges[0]:
        return [ranges[0], *tmp[1:]]
    elif tmp[0].left in ranges[0]:
        return [Range(ranges[0].left, tmp[0].right), *tmp[1:]]
    else:
        return [ranges[0], *tmp]


def main():
    fresh_ranges = (
        "3-5\n"
        "10-14\n"
        "12-18\n"
        "16-20\n"
    )
    fruits = (
        "1\n"
        "5\n"
        "8\n"
        "11\n"
        "17\n"
        "32"
    )

    with open("day5_ranges.txt", "r") as f:
        fresh_ranges = f.read()

    with open("day5_fruits.txt", "r") as f:
        fruits = f.read()

    raw_ranges = []
    for line in fresh_ranges.splitlines():
        raw = line.split("-")
        raw_ranges.append(Range(int(raw[0]), int(raw[1])))

    raw_ranges.sort(key=lambda x: x.left)

    ranges = folding(raw_ranges)
    ids = [int(f) for f in fruits.splitlines()]

    counter = 0
    for id in ids:
        for r in ranges:
            if int(id) in r:
                counter += 1

    print(f"Part 1 : {counter}")

    acc = 0
    for r in ranges:
        acc += r.right - r.left + 1

    print(f"Part 2 : {acc}")


if __name__ == "__main__":
    main()
