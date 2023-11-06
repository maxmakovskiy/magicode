use std::fs;
// use std::fmt;


// TASK:
// given - GRgllbgfRgbzfRmwwcGdFMcTVPrFCF
// divide it by half
// find common chars
// rate those chars according to: 
// - [a, z] has [1, 26] values 
// - [A, Z] has [27, 52] values 
// Repeat this process for all strings
// while collecting score of common chars overall


fn score(c: char) -> i32 {
    let r: i32 = c as i32;
    if 97 <= r && r <= 122 {
       return r - 96; 
    } else {
        return r - 64 + 26;
    }
}

fn compute_line(line: &str) -> i32 {
    let m = line.len() / 2;
    let left = &line[..m];
    let right = &line[m..];
    
    for c in left.chars() {
        if right.contains(c) {
            println!("find match '{}' in {}", c, line);
            return score(c);        
        }
    }

    return 0;
}


fn process(content: String) -> Vec<i32> {
    let mut result: Vec<i32> = Vec::new();
    for line in content.split('\n') {
        result.push(compute_line(line));
    }

    return result;
}


fn read_file(filename: &str) -> String {
    let content: String  = fs::read_to_string(filename)
        .expect(format!("Unable to open {}", filename).as_str());

    return content;
}


fn main() {
    let filename = "/home/barni/devs/advent_of_code/2022/day_3/first_part/src/input.txt";
    let content = read_file(filename);
    let intermediate: Vec<i32> = process(content);
    let result: i32 = intermediate.into_iter().reduce(|l, r| l + r).unwrap();
    println!("overall score {}", result);
}
