def main():
    # f1 = open("day12_sample_shapes.txt", "r")
    f1 = open("day12_shapes.txt", "r")
    shapes = f1.read()
    f1.close()

    # f2 = open("day12_sample_trees.txt", "r")
    f2 = open("day12_trees.txt", "r")
    trees = f2.read()
    f2.close()

    presents = []
    for sh in shapes.split("\n\n"):
        matrix = sh.split(":")[1]
        presents.append(sum([1 if ch == "#" else 0 for ch in matrix]))

    valid = 0
    for tr in trees.splitlines():
        parts = tr.split(":")
        x, y = [int(c) for c in parts[0].split("x")]

        qty_to_fit = 0

        for i, qty in enumerate(parts[1].strip().split(" ")):
            qty_to_fit += int(qty) * presents[i]

        # NOTE:
        # super easy heuristic
        if x * y >= qty_to_fit:
            valid += 1

    print(valid)




if __name__ == "__main__":
    main()

