def explore(vertex, target, adjacency_list, visited, counters):
    visited[vertex] = True

    for v in adjacency_list[vertex]:
        if v == target:
            return 1
        elif not visited[v]:
            counters[v] += explore(v, target, adjacency_list, visited, counters)

    counters[vertex] = sum([counters[v] for v in adjacency_list[vertex]])

    return counters[vertex]


def dfs(adjacency_list, *, label, target):
    visited: dict[str, bool] = dict()
    counters: dict[str, int] = dict()

    for vertex in adjacency_list.keys():
        visited[vertex] = False
        counters[vertex] = 0

    s = 0
    for v in adjacency_list[label]:
        s += explore(v, target, adjacency_list, visited, counters)

    return s


def part_1():
    sample = (
        "aaa: you hhh\n"
        "you: bbb ccc\n"
        "bbb: ddd eee\n"
        "ccc: ddd eee fff\n"
        "ddd: ggg\n"
        "eee: out\n"
        "fff: out\n"
        "ggg: out\n"
        "hhh: ccc fff iii\n"
        "iii: out"
    )
    with open("day11.txt", "r") as f:
        adjacency_list: dict[str, list[str]] = dict()

        # for line in sample.splitlines():
        for line in f.read().splitlines():
            parts = line.split(":")
            adjacency_list[parts[0].strip()] = list(filter(lambda x: len(x) > 0, [s.strip() for s in parts[1].split(" ")]))

        adjacency_list["out"] = []

        return dfs(adjacency_list, label="you", target="out")


def part_2():
    sample = (
        "svr: aaa bbb\n"
        "aaa: fft\n"
        "fft: ccc\n"
        "bbb: tty\n"
        "tty: ccc\n"
        "ccc: ddd eee\n"
        "ddd: hub\n"
        "hub: fff\n"
        "eee: dac\n"
        "dac: fff\n"
        "fff: ggg hhh\n"
        "ggg: out\n"
        "hhh: out\n"
    )

    f = open("day11.txt", "r")
    sample = f.read()
    f.close()

    adjacency_list: dict[str, list[str]] = dict()

    for line in sample.splitlines():
        parts = line.split(":")
        adjacency_list[parts[0].strip()] = list(filter(lambda x: len(x) > 0, [s.strip() for s in parts[1].split(" ")]))

    adjacency_list["out"] = []

    return (
              dfs(adjacency_list, label="svr", target="fft")
            * dfs(adjacency_list, label="fft", target="dac")
            * dfs(adjacency_list, label="dac", target="out")
    )


def main():
    print(f"Part 1: {part_1()}")
    print(f"Part 2: {part_2()}")


if __name__ == "__main__":
    main()

