visited: dict[str, bool] = dict()
out_touched_counter = 0


def explore(vertex, adjacency_list):
    global out_touched_counter

    # visited[vertex] = True

    for v in adjacency_list[vertex]:
        if v == "out":
            out_touched_counter += 1

        # if not visited[v]:
        explore(v, adjacency_list)


def dfs(adjacency_list):
    global visited

    # for vertex in adjacency_list.keys():
    #     visited[vertex] = False

    label = "you"

    for v in adjacency_list[label]:
        explore(v, adjacency_list)


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

        dfs(adjacency_list)
        print(out_touched_counter)



if __name__ == "__main__":
    main()

