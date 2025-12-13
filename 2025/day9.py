import itertools as iter

PointType = tuple[int, int]
VectorType = tuple[PointType, PointType]


def compute_rectangle(v: VectorType) -> int:
    p1, p2 = v
    max_x, min_x = max(p1[0], p2[0]), min(p1[0], p2[0])
    max_y, min_y = max(p1[1], p2[1]), min(p1[1], p2[1])

    return (max_x - min_x + 1) * (max_y - min_y + 1)


def filter_straight(v: VectorType) -> bool:
    p1, p2 = v
    return p1[0] == p2[0] or p1[1] == p2[1]


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

    with open("day9.txt", "r") as f:
        lines = [tuple(line.split(",")) for line in sample.splitlines()]
        # lines = [tuple(line.split(",")) for line in f.read().splitlines()]
        vertices: list[PointType] = [(int(l[0]), int(l[1])) for l in lines]

        pairs: list[VectorType] = list(iter.combinations(vertices, 2))

        areas = [(i, area) for i, area in enumerate(map(compute_rectangle, pairs))]

        areas.sort(key=lambda x: x[1], reverse=True)

        # print(f"Coordinate: {pairs[areas[0][0]]}")
        print(f"Part 1: {areas[0][1]}")


if __name__ == "__main__":
    main()