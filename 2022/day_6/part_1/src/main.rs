use std::fs;
use std::env;



fn analyze(prefix: &[u8]) -> Option<bool> {
    if prefix.len() != 4 {
        return None;
    }
    let mut res: bool = true;
    for i in 0..3 {
        res = res && (! prefix[(i+1)..].contains(&prefix[i]));
    }
    return Some(res);
}


fn solver(input: &[u8]) -> u32 {
    let mut n_prefix: u32 = 0;
    for prefix in input.windows(4) {
        n_prefix += 1;

        // if analyze(prefix).unwrap() {
        //     prefix.iter().for_each(|item| print!("{}, ",item));
        //     println!("");
        //     break;
        // }

        // println!("\nn: {} ##################", n_prefix);
        let t = analyze(prefix).unwrap();
        // prefix.iter().for_each(|item| print!("{}, ",item));
        // println!("Summary {}", if t {"unique"} else {"dublication"});
        if t {
            break;
        }
    }
    return n_prefix + 3;
}

fn test_solver1() {
    let input: Vec<u8> = Vec::from("mjqjpqmgbljsphdztnvjfqwrcgsmlb");
    let output = solver(&input);
    assert_eq!(output, 7);
}

fn test_solver2() {
    let input: Vec<u8> = Vec::from("bvwbjplbgvbhsrlpgdmjqwftvncz");
    let output = solver(&input);
    assert_eq!(output, 5);
}

fn test_solver3() {
    let input: Vec<u8> = Vec::from("nppdvjthqldpwncqszvftbrmjlhg");
    let output = solver(&input);
    assert_eq!(output, 6);
}

fn test_solver4() {
    let input: Vec<u8> = Vec::from("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg");
    let output = solver(&input);
    assert_eq!(output, 10);
}

fn test_solver5() {
    let input: Vec<u8> = Vec::from("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw");
    let output = solver(&input);
    assert_eq!(output, 11);
}

fn main() {
    test_solver1();
    test_solver2();
    test_solver3();
    test_solver4();
    test_solver5();
    
    let filename: String = env::args()
        .nth(1)
        .expect("File should be provided with puzzle's input");
    
    let input: Vec<u8> = fs::read(filename)
        .expect("Unable to read file");

    let output = solver(&input);

    println!("Prefix was reached after {}", output);
}
