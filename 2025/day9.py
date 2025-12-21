from itertools import combinations

PointType = tuple[int, int]


def compute_rectangle(pair: tuple[PointType, PointType]) -> int:
    p1, p2 = pair
    max_x, min_x = max(p1[0], p2[0]), min(p1[0], p2[0])
    max_y, min_y = max(p1[1], p2[1]), min(p1[1], p2[1])

    return (max_x - min_x + 1) * (max_y - min_y + 1)


def part_1(source: str) -> int:
    lines = [tuple(line.split(",")) for line in source.splitlines()]
    vertices: list[PointType] = [(int(l[0]), int(l[1])) for l in lines]

    pairs: list[tuple[PointType, PointType]] = list(combinations(vertices, 2))

    areas = [(i, area) for i, area in enumerate(map(compute_rectangle, pairs))]

    areas.sort(key=lambda x: x[1], reverse=True)

    return areas[0][1]


def main():
    sample = (
        "7,1\n"
        "11,1\n"
        "11,7\n"
        "9,7\n"
        "9,5\n"
        "2,5\n"
        "2,3\n"
        "7,3"
    )

    # file = open("day9.txt", "r")
    # sample = file.read()
    # file.close()

    print(f"Part 1: {part_1(sample)}")


if __name__ == "__main__":
    main()