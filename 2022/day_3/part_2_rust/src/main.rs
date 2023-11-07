//use std::io::prelude::*;
//use std::io::BufReader;
use std::fs::File;
use std::io::Read;


/*fn read_file_as_groups(filename: &str) -> i32 {
    let file = File::open(filename).expect("can't open file");
    let reader = BufReader::new(file);
    
    for line in reader.lines() {
        println!(" => {}", line.unwrap());
    }

    0
}*/ 


/*fn treat_as_ascii(source: &str) -> Vec<i32> {
    let mut res: Vec<i32> = Vec::new();

    for ch in source.chars() {
        res.push(ch as i32);
    }

    return res;
}*/


fn score(c: char) -> i32 {
    let ascii = c as i32;
    if 0x61 <= ascii && ascii <= 0x7A {
        ascii - 0x60
    } else {
        ascii - 0x26
    }
}


fn common_in(group: &[&str]) -> Option<char> {
    for c in group[0].chars() {
        if group[1].contains(c) &&
           group[2].contains(c) 
        {
            return Some(c)
        } 
    }

    None 
}


fn main() {
    // Qu'est-ce que j'ai besoin de faire?
    // 1. divide by groups of 3 lines in each
    // 2. find common char within group
    // 3. compute its score
    // 4. accamulate score over all groups
    let path = "/home/barni/devs/advent_of_code/2022/day_3/part_2_rust/src/input.txt";

    let mut file = File::open(path).expect("can't open file");
    let mut content = String::new();
    file.read_to_string(&mut content).expect("can't read from file");  
    println!("Have been read {} bytes", content.len());
    let lines: Vec<&str> = content
        .split('\n')
        .filter(|l| l.len() > 0) // gonna be 301 line without it
        .collect();

    let total = lines
        .chunks(3)
        .map(|group| common_in(group).unwrap())
        .fold(0, |acc, elem| acc + score(elem));

    // Correct puzzle answer is 2780
    println!("total score {}", total);


    
//    println!("Have {} lines", lines.len());
//    let lasts = &lines[299..];
//    for l in lasts {
//        println!("line {}", l);
//        let ascii: Vec<i32> = treat_as_ascii(l);
//        println!("ascii's codes: {:?}", &ascii);
//    }

}
