from itertools import permutations


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


def main():
    sample = (
        "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
        "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
        "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}"
    )

    indicator_lights: list[str] = []
    button_schematics: list[list[tuple[int,...]]] = []

    with open("day10.txt", "r") as f:
        # for line in sample.splitlines():
        for line in f.read().splitlines():
            parts = line.split(" ")
            indicator_lights.append(parts[0])
            button_schematics.append([tuple([int(x) for x in p if x.isdigit()]) for p in parts[1:-1]])

        print(sum([
            brut_force(indicator_lights[i], button_schematics[i])
            for i in range(len(indicator_lights))
        ]))



if __name__ == "__main__":
    main()

