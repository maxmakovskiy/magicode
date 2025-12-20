def explore(vertex, adjacency_list, visited, counters):
    visited[vertex] = True

    for v in adjacency_list[vertex]:
        if v == "out":
            return 1
        elif not visited[v]:
            counters[v] += explore(v, adjacency_list, visited, counters)

    counters[vertex] = sum([counters[v] for v in adjacency_list[vertex]])

    return counters[vertex]


def dfs(adjacency_list, label):
    visited: dict[str, bool] = dict()
    counters: dict[str, int] = dict()

    for vertex in adjacency_list.keys():
        visited[vertex] = False
        counters[vertex] = 0

    s = 0
    for v in adjacency_list[label]:
        s += explore(v, adjacency_list, visited, counters)

    return s


def main():
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

        print(dfs(adjacency_list, "you"))



if __name__ == "__main__":
    main()

