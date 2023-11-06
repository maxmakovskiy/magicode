# Qu'est-ce que j'ai besoin de faire?
# 1. divide by groups of 3 lines in each
# 2. find common char within group
# 3. compute its score
# 4. accamulate score over all groups

def score(c):
    ascii = ord(c)
    if 0x41 <= ascii and ascii <= 0x5A:
        return ascii - 0x40 + 0x1A
    else:
        return ascii - 0x60


def find_common(lines):
    for c in lines[0]:
        if c in lines[1] and c in lines[2]:
            return c 

def minimize(s, limit=5):
    return f"{s[:limit+1]}..." if limit <= len(s) else s


def print_group(group):
    nices = list(map(
        lambda s: minimize(s),
        group
    )) 
    print(" | ".join(nices))


def emphase(c, s):
    class color:
       PURPLE = '\033[95m'
       CYAN = '\033[96m'
       DARKCYAN = '\033[36m'
       BLUE = '\033[94m'
       GREEN = '\033[92m'
       YELLOW = '\033[93m'
       RED = '\033[91m'
       BOLD = '\033[1m'
       UNDERLINE = '\033[4m'
       END = '\033[0m'

    pos = s.find(c)
    return s[:pos] + color.YELLOW + color.BOLD + s[pos] + color.END + s[pos+1:]


def print_comm_char_info(c, score, group):
    print(f"> common char {c} with score {score} has been found in:")
    for line in group:
        print(f"\t {emphase(c, line)}")


def main():
    filename = "input.txt"
    group = []
    k = 0
    result = 0
    with open(filename, "rt") as file:
#        all = file.read()
#        lines = list(filter(
#            lambda elem: not elem in ["\n", "", " "],
#            all.split("\n")
#        ))
#        groups = []
#        for i in range(0, 99):
#            result += score(find_common(
#                lines[i*3 : (i+1)*3]
#            ))

        
        for line in file:
            line = line[:-1]
            if k == 3:
                print_group(group)
                badge = find_common(group)
                team_score = score(badge)
                result += team_score
                print_comm_char_info(badge, team_score, group)
                group = [line]
                k = 0
            else:
                group.append(line)
            k += 1

    result += score(find_common(group))
    print_group(group)
    print(f"Score of badges overall elf's group is {result}")


#    with open(filename, "rt") as file:
#        iter = 1
#        for line in file:
#            print(f"{iter} |\t{line[:-1]}")
#            iter += 1


if __name__ == "__main__":
    main()
 
