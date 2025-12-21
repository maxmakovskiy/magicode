from itertools import combinations, pairwise, repeat
from functools import partial, Placeholder
from math import comb

Point = tuple[int, int]


def compute_rectangle(pair: tuple[Point, Point]) -> int:
    p1, p2 = pair
    max_x, min_x = max(p1[0], p2[0]), min(p1[0], p2[0])
    max_y, min_y = max(p1[1], p2[1]), min(p1[1], p2[1])

    return (max_x - min_x + 1) * (max_y - min_y + 1)


def str_to_crds(source: str) -> list[Point]:
    lines = [tuple(line.split(",")) for line in source.splitlines()]
    return [(int(l[0]), int(l[1])) for l in lines]


def part_1(source: str) -> int:
    vertices: list[Point] = str_to_crds(source)
    pairs: list[tuple[Point, Point]] = list(combinations(vertices, 2))

    areas = [(i, area) for i, area in enumerate(map(compute_rectangle, pairs))]
    areas.sort(key=lambda x: x[1], reverse=True)
    return areas[0][1]


def compress_coordinates(crds: list[Point]):
    crds_x = list(set([x for x, _ in crds]))
    crds_y = list(set([y for _, y in crds]))

    crds_x.sort()
    crds_y.sort()

    ranks_x = {n:i for i, n in enumerate(crds_x)}
    ranks_y = {n:i for i, n in enumerate(crds_y)}

    return (
        [(ranks_x[x], ranks_y[y]) for x, y in crds],
        ranks_x,
        ranks_y
    )


def build_polygon(crds: list[Point]) -> list[tuple[Point, Point]]:
    return list(pairwise(crds)) + [(crds[-1], crds[0])]


def flood_fill(p: Point, edges: list[tuple[Point, Point]]) -> list[Point]:
    polygon: list[Point] = []

    q: list[Point] = list()
    q.append(p)

    while len(q) > 0:
        point = q[0]
        q = q[1:]
        if point_in_orthogonal_polygon(point, edges):
            polygon.append(point)

            right = (point[0] + 1, point[1])
            left = (point[0] - 1, point[1])
            top = (point[0], point[1] + 1)
            bottom = (point[0], point[1] - 1)

            if right not in polygon and right not in q:
                q.append(right)

            if left not in polygon and left not in q:
                q.append(left)

            if top not in polygon and top not in q:
                q.append(top)

            if bottom not in polygon and bottom not in q:
                q.append(bottom)

    return polygon


def is_enclosed(corners: tuple[Point, Point], polygon: list[Point]) -> bool:
    x1, x2 = min(corners[0][0], corners[1][0]), max(corners[0][0], corners[1][0])
    y1, y2 = min(corners[0][1], corners[1][1]), max(corners[0][1], corners[1][1])

    xs = list(range(x1, x2 + 1))
    ys = list(range(y1, y2 + 1))

    borders = [
        list(zip(xs, repeat(y2))),
        list(zip(xs, repeat(y1))),
        list(zip(repeat(x1), ys)),
        list(zip(repeat(x2), ys))
    ]

    for b in borders:
        for p in b:
            if p not in polygon:
                return False

    return True


def point_on_segment(px, py, x1, y1, x2, y2):
    # Check if point (px,py) lies on segment (x1,y1)-(x2,y2)
    if x1 == x2:  # vertical segment
        return px == x1 and min(y1, y2) <= py <= max(y1, y2)
    if y1 == y2:  # horizontal segment
        return py == y1 and min(x1, x2) <= px <= max(x1, x2)
    return False


def point_in_orthogonal_polygon(point: Point, edges: list[tuple[Point, Point]]) -> bool:
    px, py = point
    intersections = 0

    for edge in edges:
        x1, y1 = edge[0]
        x2, y2 = edge[1]

        # 1. Boundary check
        if point_on_segment(px, py, x1, y1, x2, y2):
            return True

        # 2. Ray casting: only vertical edges matter
        if x1 == x2:  # vertical edge
            # Check if ray crosses the y-range of the edge
            if min(y1, y2) <= py < max(y1, y2):
                # Check if edge is to the right of the point
                if x1 > px:
                    intersections += 1

    # 3. Even–odd rule
    return intersections % 2 == 1


def find_inside_point(edges: list[tuple[Point, Point]]) -> Point:
    x = 0
    y = 0
    while True:
        if point_in_orthogonal_polygon((x, y), edges):
            return x, y
        x += 1
        y += 1


def part_2(source: str) -> int:
    # 1. compress coordinates
    orig_vertices: list[Point] = str_to_crds(source)
    vertices, ranks_x, ranks_y = compress_coordinates(orig_vertices)

    print("compression done")

    # 2. rasterize the polygon | fill the edges
    edges: list[tuple[Point, Point]] = build_polygon(vertices)

    print("building polygon done")

    # 3. find a (any) point that is inside the polygon
    inside_point: Point = find_inside_point(edges)


    print("finding inside point done")

    # 4. fill flood the polygon using this point (actually starting from it)
    polygon_points: list[Point] = flood_fill(inside_point, edges)

    # 5. use part 1 strategy to generate rectangles but this time use (4)
    #    to check if all borders of a rectangle belong to the polygon

    print("filling polygon done")

    # pairs = list(filter(partial(is_enclosed, Placeholder, polygon_points),
    #                     combinations(vertices, 2)))

    pairs = []
    n_combs = comb(len(vertices), 2)
    print(f"{n_combs} pairs to process")

    i = 1
    for v1, v2 in combinations(vertices, 2):
        print(f"checking {i} / {n_combs}")
        if is_enclosed((v1, v2), polygon_points):
            pairs.append((v1, v2))
        i += 1

    print(f"filtering enclosed corners done. {len(pairs)} are correct")

    inv_ranks_x = {v:k for k, v in ranks_x.items()}
    inv_ranks_y = {v:k for k, v in ranks_y.items()}

    pairs = list(zip(
        map(lambda corners: (inv_ranks_x[corners[0][0]],  inv_ranks_y[corners[0][1]]), pairs),
        map(lambda corners: (inv_ranks_x[corners[1][0]],  inv_ranks_y[corners[1][1]]), pairs)
    ))

    print("uncompressing coordinates done")

    # not the right answer: 4591195600 - too high

    areas = list(map(compute_rectangle, pairs))
    areas.sort(reverse=True)
    return areas[0]


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

    file = open("day9.txt", "r")
    sample = file.read()
    file.close()

    print(f"Part 1: {part_1(sample)}")
    print(f"Part 2: {part_2(sample)}")


if __name__ == "__main__":
    main()