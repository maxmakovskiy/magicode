use std::fs;
use std::env;


fn solve_puzzle(stream: &[u8]) -> u32 {
    let mut res: u32 = 13;

    for interval in stream.windows(14) {
        res += 1;
        let t = interval.iter().enumerate().fold(true, |acc, (idx, elem)| {
            return acc && (! interval[(idx+1)..].contains(elem));
        });

        // println!("{:?}", interval);
        if t {
            // println!("Last one was unique");
            break;
        }

    }
   
    return res;
}

fn test1() {
    let source: Vec<u8> = Vec::from("mjqjpqmgbljsphdztnvjfqwrcgsmlb");
    let result = solve_puzzle(&source);
    assert_eq!(result, 19);
}

fn test2() {
    let source: Vec<u8> = Vec::from("bvwbjplbgvbhsrlpgdmjqwftvncz");
    let result = solve_puzzle(&source);
    assert_eq!(result, 23);
}

fn test3() {
    let source: Vec<u8> = Vec::from("nppdvjthqldpwncqszvftbrmjlhg");
    let result = solve_puzzle(&source);
    assert_eq!(result, 23);
}

fn test4() {
    let source: Vec<u8> = Vec::from("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg");
    let result = solve_puzzle(&source);
    assert_eq!(result, 29);
}

fn test5() {
    let source: Vec<u8> = Vec::from("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw");
    let result = solve_puzzle(&source);
    assert_eq!(result, 26);
}

fn main() {
    test1();
    test2();
    test3();
    test4();
    test5();

    let file = env::args().nth(1).expect("Filename was been expecting as argument");
    let content: Vec<u8> = fs::read(file).expect("Unable to read provided file");
    println!("Result for given input is {}", solve_puzzle(&content));
}
