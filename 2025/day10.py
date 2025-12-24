# /// script
# requires-python = ">=3.14"
# dependencies = [
#   "z3-solver>=4.15.4.0",
# ]
# ///


from itertools import permutations
import z3


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


def brut_force(ref_lights: str, button_schematics: list[list[int]]) -> int:
    for i in range(1, len(ref_lights) + 1):
        configs = list(permutations(button_schematics, i))

        for c in configs:
            work_lights = IndicatorLights(len(ref_lights))
            for schema in c:
                for btn in schema:
                    work_lights.toggle(btn)

            if str(work_lights) == ref_lights:
                return len(c)

    return 0


def solve_part2(joltage, button_schematics) -> int:
    # For every row in the input
    # This puzzle is linear programming problem
    # and z3 is the solver (docs: https://ericpony.github.io/z3py-tutorial/guide-examples.htm)

    opt = z3.Optimize()

    presses = [z3.Int(f"c_{i}") for i in range(len(button_schematics))]

    # button could be pressed or not
    for count in presses:
        opt.add(count >= 0)

    # Find out which buttons affect which joltage
    # In other words:
    # Let say joltage is {3,5,4,7}
    # Buttons are:
    # a = (3), b = (1,3), c = (2), d = (2,3), e = (0,2), f = (0,1)
    # for example e and f are the only ones which affect joltage with position (index) 0
    # so e + f = 3
    # using same logic for other positions within joltage we get:
    # b + f = 5
    # c + d + e = 4
    # a + b + d = 7

    for pos, jol in enumerate(joltage):
        affects = [presses[idx] for idx, btn in enumerate(button_schematics) if pos in btn]
        opt.add(z3.Sum(affects) == jol)

    # minimize total presses
    opt.minimize(z3.Sum(presses))

    if opt.check() != z3.sat:
        raise ValueError("No solution")

    model = opt.model()
    return sum(model[c].as_long() for c in presses)


def main():
    sample = (
        "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
        "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
        "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}"
    )

    indicator_lights: list[str] = []
    button_schematics: list[list[list[int]]] = []
    counters: list[list[int]] = []

    # f = open("day10.txt", "r")
    # sample = f.read()
    # f.close()

    for line in sample.splitlines():
        parts = line.split(" ")
        indicator_lights.append(parts[0])
        button_schematics.append([[int(x) for x in p if x.isdigit()] for p in parts[1:-1]])
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

