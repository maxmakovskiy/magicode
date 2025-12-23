from itertools import permutations
from functools import reduce



class IndicatorLights:
    def __init__(self, length: int):
        self.lights: list[int] = [0 for _ in range(length-2)]

    def toggle(self, key: int):
        if self.lights[key] == 0:
            self.lights[key] = 1
        else:
            self.lights[key] = 0

    def __str__(self):
        return "[" + "".join(map(lambda item: "#" if item == 1 else ".", self.lights)) + "]"


def brut_force(ref_lights: str, button_schematics: list[tuple[int, ...]]) -> int:
    for i in range(1, len(ref_lights) + 1):
        configs: list[list[tuple[int, ...]]] = list(permutations(button_schematics, i))

        for c in configs:
            work_lights = IndicatorLights(len(ref_lights))
            for schema in c:
                for btn in schema:
                    work_lights.toggle(btn)

            if str(work_lights) == ref_lights:
                return len(c)

    return 0


def compute_score(r: list[int], all_ranks_sum: int, max_schema_len: int) -> float:
    return sum(r) / all_ranks_sum + len(r) / max_schema_len


def rank_counters(counters: list[int]) -> list[int]:
    ranks = {v:i for i,v in enumerate(sorted(set(counters)))}
    return [ranks[v] for v in counters]


def analyze_counters(counters: list[int], button_schematics: list[tuple[int, ...]]) -> int:
    """
    Choose best button schematics to apply
    """

    pos_ranks = rank_counters(counters)

    all_ranks_sum = reduce(lambda x, y: x+y, range(len(counters)))
    max_schema_len = len(button_schematics[0])
    scores = []

    for i, schema in enumerate(button_schematics):
        curr_ranks = []
        for c in schema:
            curr_ranks.append(pos_ranks[c])
        scores.append((i, compute_score(curr_ranks, all_ranks_sum, max_schema_len)))

    scores.sort(key=lambda item: item[1], reverse=True)

    for sc in scores:
        print(f"    {button_schematics[sc[0]]} : {sc[1]}")

    return scores[0][0]


def solve_part2(counters: list[int], button_schematics: list[tuple[int, ...]]) -> int:
    button_schematics.sort(key=lambda item: len(item), reverse=True)

    i = 1
    while sum(counters) != 0 and not any(map(lambda x: x < 0, counters)):
        idx_schema = analyze_counters(counters, button_schematics)
        print(f"step {i}: schema: {button_schematics[idx_schema]}")
        for s in button_schematics[idx_schema]:
            counters[s] -= 1
        print(f"step {i}: counters: {counters}")
        print()
        i += 1

    return i


def main():
    sample = (
        "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
        "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
        "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}"
    )

    indicator_lights: list[str] = []
    button_schematics: list[list[tuple[int,...]]] = []
    counters: list[list[int]] = []

    # f = open("day10.txt", "r")
    # sample = f.read()
    # f.close()

    for line in sample.splitlines():
        parts = line.split(" ")
        indicator_lights.append(parts[0])
        button_schematics.append([tuple([int(x) for x in p if x.isdigit()]) for p in parts[1:-1]])
        counters.append([int(x) for x in parts[-1][1:-1].split(",")])

    part1 = sum([
        brut_force(indicator_lights[i], button_schematics[i])
        for i in range(len(indicator_lights))
    ])
    print(f"Part 1 : {part1}")

    part2 = sum([
        solve_part2(counters[i], button_schematics[i])
        for i in range(len(counters))
    ])
    print(f"Part 2 : {part2}")


if __name__ == "__main__":
    main()

