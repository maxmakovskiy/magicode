import textwrap

def find_groups(num: int) -> list[int]:
    group = []

    for i in range(1, num // 2 + 1):
        if num % i == 0:
            group.append(i)

    return group


def is_invalid_2(word: str) -> bool:
    subword_lens = find_groups(len(word))

    counter = 0

    for l in subword_lens:
        parts = textwrap.wrap(word, l)
        s = set(parts)
        counter += len(s) == 1

    return counter > 0


def is_invalid_1(word: str) -> bool:
    w_len = len(word)
    parts = (word[:w_len // 2], word[w_len // 2:])
    return len(set(parts)) == 1


def main():
    # sample = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124"

    sample = "9226466333-9226692707,55432-96230,4151-6365,686836-836582,519296-634281,355894-471980,971626-1037744,25107-44804,15139904-15163735,155452-255998,2093-4136,829776608-829880425,4444385616-4444502989,2208288-2231858,261-399,66-119,91876508-91956018,2828255673-2828317078,312330-341840,6464-10967,5489467-5621638,1-18,426-834,3434321102-3434378477,4865070-4972019,54475091-54592515,147-257,48664376-48836792,45-61,1183-1877,24-43"

    r1 = 0
    r2 = 0

    for interval in sample.strip().split(","):
        raw = interval.split("-")
        lower_bound = int(raw[0])
        upper_bound = int(raw[1])
        for integer in range(lower_bound, upper_bound + 1):
            if is_invalid_1(str(integer)):
                r1 += integer

            if is_invalid_2(str(integer)):
                r2 += integer

    print(f"PART 1: Invalid ids produce {r1}")
    print(f"PART 2: Invalid ids produce {r2}")


if __name__ == "__main__":
    main()
