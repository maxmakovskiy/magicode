import copy


class SparseMatrix:
    def __init__(self, n_rows, n_cols):
        self.n_rows = n_rows
        self.n_cols = n_cols
        self.storage: dict[dict[int:int]] = dict()

    def get(self, row_idx: int, col_idx: int):
        if row_idx not in self.storage.keys():
            return 0

        if col_idx not in self.storage[row_idx].keys():
            return 0

        return self.storage[row_idx][col_idx]

    def set(self, row_idx: int, col_idx: int, val: int):
        if row_idx not in self.storage.keys():
            self.storage[row_idx] = dict()

        self.storage[row_idx][col_idx] = val

    def shape(self):
        return self.n_rows, self.n_cols


coord = tuple[int, int]


def find_beam(line: str) -> coord:
    for i, ch in enumerate(line):
        if ch == "S":
            return 0, i
    return 0, 0


def find_splitters(net: list[str], net_dimensions: tuple[int, int]) -> SparseMatrix:
    matrix = SparseMatrix(net_dimensions[0], net_dimensions[1])

    for row_idx, line in enumerate(net):
        for col_idx, ch in enumerate(line):
            if ch == "^":
                matrix.set(row_idx, col_idx, 1)

    return matrix


def move_beam(beam_pos: coord, splitters: SparseMatrix, net_dimensions: tuple[int, int]) -> int:
    beam_row, beam_col = beam_pos
    n_rows, n_cols = net_dimensions

    for _ in range(beam_row, n_rows):
        beam_row += 1

        val = splitters.get(beam_row, beam_col)

        if val == 1:
            splitters.set(beam_row, beam_col, val + 1)
            return (1
                    + move_beam((beam_row, beam_col + 1), splitters, net_dimensions)
                    + move_beam((beam_row, beam_col - 1), splitters, net_dimensions))

        if val > 1:
            return 0

    return 0


# gridy solution
def calc_path(beam_pos: coord, splitters: SparseMatrix, net_dimensions: tuple[int, int]) -> int:
    beam_row, beam_col = beam_pos
    n_rows, n_cols = net_dimensions

    for _ in range(beam_row, n_rows):
        beam_row += 1
        if splitters.get(beam_row, beam_col) == 1:
            return (calc_path((beam_row, beam_col + 1), splitters, net_dimensions)
                    + calc_path((beam_row, beam_col - 1), splitters, net_dimensions))
    return 1


# optimized solution by avoiding unnecessary recursive calls
def calc_path3(beam_pos: coord, splitters: SparseMatrix, net_dimensions: tuple[int, int]) -> int:
    beam_row, beam_col = beam_pos
    n_rows, n_cols = net_dimensions

    for _ in range(beam_row, n_rows):
        beam_row += 1

        val = splitters.get(beam_row, beam_col)

        # beam touching a splitter for the 1st time
        # how much of paths subsequent (underlying) splitters bring
        if val == 1:
            v1 = calc_path3((beam_row, beam_col + 1), splitters, net_dimensions)
            v2 = calc_path3((beam_row, beam_col - 1), splitters, net_dimensions)
            # cache number of paths of underlying splitters in the current splitter
            splitters.set(beam_row, beam_col, v1 + v2)
            # return it
            return v1 + v2

        elif val > 1:
            # if current splitter has been touched before
            # return cached value from it which represents number of paths from this splitter
            return splitters.get(beam_row, beam_col)

    # base case: beam touching the ground = 1 path is done
    return 1


def main():
    sample = (
        ".......S.......\n"
        "...............\n"
        ".......^.......\n"
        "...............\n"
        "......^.^......\n"
        "...............\n"
        ".....^.^.^.....\n"
        "...............\n"
        "....^.^...^....\n"
        "...............\n"
        "...^.^...^.^...\n"
        "...............\n"
        "..^...^.....^..\n"
        "...............\n"
        ".^.^.^.^.^...^.\n"
        "..............."
    )

    with open("day7.txt", "r") as f:
        # lines = sample.splitlines()
        lines = f.read().splitlines()

        net_dimensions = (len(lines), len(lines[0]))

        beam = find_beam(lines[0])
        splitters = find_splitters(lines, net_dimensions)
        splitters2 = copy.deepcopy(splitters)

        print(f"Part 1: {move_beam(beam, splitters, net_dimensions)}")

        # print(f"Part 2: {calc_path(beam, splitters2, net_dimensions)}")
        print(f"Part 2: {calc_path3(beam, splitters2, net_dimensions)}")


if __name__ == "__main__":
    main()
