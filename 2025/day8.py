import functools
import itertools
import math

Box = tuple[int, int, int]

class ClassicDisjointSetsUnion:
    def __init__(self):
        self.circuits: dict[Box,Box] = dict()
        self.rank: dict[Box, int] = dict()

    def make_new(self, b: Box):
        self.circuits[b] = b
        self.rank[b] = 0

    def find(self, b: Box) -> Box:
        """
        :param b: box in circuit
        :return: returns the representative (also called leader) of the circuit
        (set) that contains the box (element) b.
        """
        if b == self.circuits[b]:
            return b

        return self.find(self.circuits[b])

    def union_sets(self, v1: Box, v2: Box):
        b1 = self.find(v1)
        b2 = self.find(v2)

        if b1 == b2:
            return

        if self.rank[b1] > self.rank[b2]:
            self.circuits[b2] = b1
        else:
            self.circuits[b1] = b2
            if self.rank[b1] == self.rank[b2]:
                self.rank[b1] += 1


class DisjointSetsUnion:
    def __init__(self):
        self.circuits: dict[Box,set[Box]] = dict()

    def make_new(self, b: Box):
        self.circuits[b] = set()
        self.circuits[b].add(b)

    def find(self, b: Box) -> Box | None:
        """
        :param b: box in circuit
        :return: returns the representative (also called leader) of the circuit
        (set) that contains the box (element) b.
        """
        if b in self.circuits.keys():
            return b

        for key, cs in self.circuits.items():
            if b in cs:
                return key
        return None

    def union_sets(self, pair: tuple[Box, Box]):
        b1 = self.find(pair[0])
        b2 = self.find(pair[1])

        if b1 != b2:
            self.circuits[b2].add(b1)
            for b in self.circuits[b1]:
                self.circuits[b2].add(b)
            del self.circuits[b1]

    def __str__(self):
        return "\n".join([f"len({len(v)}) {k}: ({v})" for k, v in self.circuits.items()])

    def top_k_len(self, k: int):
        tmp = sorted(self.circuits.items(), key=lambda item: len(item[1]), reverse=True)
        return [len(v) for _, v in tmp][:k]


def unpack_values(line) -> Box:
    vals = line.strip().split(",")
    return int(vals[0]), int(vals[1]), int(vals[2])


def euclidian(point1: Box, point2: Box) -> float:
    return math.sqrt(sum([math.pow(point1[i] - point2[i], 2) for i in range(3)]))


def compute_distances(pairs: list[tuple[Box, Box]]) -> list[tuple[int, float]]:
    return [(idx, euclidian(p[0], p[1])) for idx, p in enumerate(pairs)]


def connect(pairs: list[tuple[Box, Box]]) -> DisjointSetsUnion:
    c = DisjointSetsUnion()

    for p1, p2 in pairs:
        c.make_new(p1)
        c.make_new(p2)

    for p in pairs[:-1]:
        c.union_sets(p)

    c.union_sets(pairs[-1])

    return c

def kruskal(vertices: list[Box], edges: list[tuple[Box, Box]]) -> list[tuple[Box, Box]]:
    c = ClassicDisjointSetsUnion()

    # init all the vertices
    for b in vertices:
        c.make_new(b)

    x: list[tuple[Box, Box]] = list()

    for edge in edges:
        # we do find here first to understand should we or not
        # add edge to the list of Min Spanning Tree edges
        if c.find(edge[0]) != c.find(edge[1]):
            x.append(edge)
            # another find happens inside the union_sets
            # to find out should we or not union two sets (are they disjoint?)
            c.union_sets(edge[0], edge[1])

    return x



def main():
    sample = (
        "162,817,812\n"
        "57,618,57\n"
        "906,360,560\n"
        "592,479,940\n"
        "352,342,300\n"
        "466,668,158\n"
        "542,29,236\n"
        "431,825,988\n"
        "739,650,466\n"
        "52,470,668\n"
        "216,146,977\n"
        "819,987,18\n"
        "117,168,530\n"
        "805,96,715\n"
        "346,949,466\n"
        "970,615,88\n"
        "941,993,340\n"
        "862,61,35\n"
        "984,92,344\n"
        "425,690,689\n"
    )

    with open("day8.txt", "r") as f:
        boxes: list[Box] = list(map(
            unpack_values,
            # sample.splitlines()
            f.read().splitlines()
        ))

        # k = 10
        k = 1000

        pairs = list(itertools.combinations(boxes, 2))
        distances = compute_distances(pairs)
        distances.sort(key=lambda x: x[1])

        pairs = [pairs[i] for i, _ in distances]

        # for idx, (internal_idx, d) in enumerate(distances):
        #     print(f"# {idx} Pair: {pairs[internal_idx]} got distance={d}")

        dsu = connect(pairs[:k])
        min_span_tree = kruskal(boxes, pairs)

        ans_part1 = functools.reduce(lambda x, y: x * y, dsu.top_k_len(3), 1)
        print(f"Part 1: {ans_part1}")
        print(f"Part 2: {min_span_tree[-1][0][0] * min_span_tree[-1][1][0]}")


if __name__ == "__main__":
    main()
