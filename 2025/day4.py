import itertools


def walk(net: list[list[str]]) -> int:
    coordinates = []

    delta = [-1, 0, 1]

    for row_idx in range(1, len(net) - 1):
        row = net[row_idx]
        for col_idx in range(1, len(row) - 1):
            if row[col_idx] == "@":
                counter = 0
                for dx in delta:
                    for dy in delta:
                        if dx == 0 and dy == 0:
                            continue

                        if net[row_idx + dx][col_idx + dy] == "@":
                            counter += 1

                if counter < 4:
                    coordinates.append((row_idx, col_idx))

    return len(coordinates)


def walk_2(net: list[list[str]]) -> int:
    coordinates = []
    r = 0

    delta = [-1, 0, 1]

    for row_idx in range(1, len(net) - 1):
        row = net[row_idx]
        for col_idx in range(1, len(row) - 1):
            if row[col_idx] == "@":
                counter = 0
                for dx in delta:
                    for dy in delta:
                        if dx == 0 and dy == 0:
                            continue

                        if net[row_idx + dx][col_idx + dy] == "@":
                            counter += 1

                if counter < 4:
                    coordinates.append((row_idx, col_idx))

    if len(coordinates) > 0:
        for x, y in coordinates:
            net[x][y] = "."

        r += len(coordinates) + walk_2(net)

    return r


def enrich(net: list[list[str]]) -> list[list[str]]:
    row_len = len(net[0]) + 2
    artificial = list(itertools.repeat(".", times=row_len))

    for row in net:
        row.insert(0, ".")
        row.append(".")

    net.insert(0, artificial)
    net.append(artificial)

    return net


def main():
    sample = (
        "..@@.@@@@.\n"
        "@@@.@.@.@@\n"
        "@@@@@.@.@@\n"
        "@.@@@@..@.\n"
        "@@.@@@@.@@\n"
        ".@@@@@@@.@\n"
        ".@.@.@.@@@\n"
        "@.@@@.@@@@\n"
        ".@@@@@@@@.\n"
        "@.@.@@@.@."
    )

    # net = [[ch for ch in line] for line in sample.strip().split("\n")]
    # net = enrich(net)
    # r1 = walk(net)
    # r2 = walk_2(net)
    # print(f"Part 1: {r1}")
    # print(f"Part 2: {r2}")

    with open("day4.txt", "r") as f:
        net = [[ch for ch in line] for line in f.read().strip().split("\n")]
        net = enrich(net)
        r1 = walk(net)
        r2 = walk_2(net)
        print(f"Part 1: {r1}")
        print(f"Part 2: {r2}")



if __name__ == "__main__":
    main()
