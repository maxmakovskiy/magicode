import sys
import re


def modify(line):
    if len(line) == 11 or len(line) == 0:
        return line

    olds = sum(list(map(
        lambda item: item.split('-'),
        line.split(',')
    )), [])

    news = []
    for n in olds:
        if len(n) < 2:
            news.append(f"0{n}")
        else:
            news.append(n)
            
    return "-".join(news[:2]) + "," + "-".join(news[2:])


def main():
    if (len(sys.argv) < 2):
        raise Exception("Provide file please")

    filename = sys.argv[1]
    with open(filename, "rt") as file:
        content = file.read()

    modified = []
    for line in list(filter(lambda item: len(item) > 0, content.split('\n'))):
        modified.append(modify(line))
        
    with open(filename, "wt") as f:
        for line in modified:
            print(line, file=f) 


if __name__ == "__main__":
    main()

     
