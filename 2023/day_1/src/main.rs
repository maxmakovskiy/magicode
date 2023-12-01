/*
// Examples of input and expecting rust
1abc2 -> 12
pqr3stu8vwx -> 38
a1b2c3d4e5f -> 15
treb7uchet -> 77

in total: sum() = 124
*/

use std::fs;
use std::env;
use regex::Regex;
use std::collections::HashMap;


fn compute_line(row: &str) -> u32 {
    let mut iter = row.chars().filter(|ch| ch.is_ascii_digit());
    let ch1: char = iter.next().unwrap();
    let d1: u32 = ch1.to_digit(10).unwrap();
    let d2: u32 = iter.last().unwrap_or(ch1).to_digit(10).unwrap();
    return d1*10 + d2;
}

fn compute_line_2(look_ups: &HashMap<&str, u32>, row: &str) -> u32 {
    // let re = Regex::new(r"Homer (?<middle>.)\. Simpson").unwrap();
    // let hay = "Homer J. Simpson";
    // let Some(caps) = re.captures(hay) else { return };
    // assert_eq!("J", &caps["middle"]);

    // two1nine -> 29
        
    let re = Regex::new(r"(?<digit>one|two|three|four|five|six|seven|eight|nine|[0-9])").unwrap();
    let buf: Vec<u32> = re.captures_iter(row).map(|caps| {
        let sample = caps.name("digit").unwrap().as_str();
        if sample.len() > 1 {
            return *(look_ups.get(sample).unwrap());
        }
        return u32::from_str_radix(sample, 10).unwrap();
    }).collect();
    
    let mut iter = buf.into_iter();
    let d1: u32 = iter.next().unwrap();
    let d2: u32 = iter.last().unwrap_or(d1);
    return d1*10 + d2;
}

fn test1() {
    let sample = "1abc2";
    let result = compute_line(sample);
    assert_eq!(result, 12);
}

fn test2() {
    let sample = "pqr3stu8vwx";
    let result = compute_line(sample);
    assert_eq!(result, 38);
}
fn test3() {
    let sample = "a1b2c3d4e5f";
    let result = compute_line(sample);
    assert_eq!(result, 15);
}
fn test4() {
    let sample = "treb7uchet";
    let result = compute_line(sample);
    assert_eq!(result, 77);
}
fn test5() {
    let sample = "1abc2\npqr3stu8vwx\na1b2c3d4e5f\ntreb7uchet";
    let result = sample.split(|ch: char| ch == '\n').count();
    assert_eq!(result, 4);
}

/*

in total = 281
*/


fn test6(table: &HashMap<&str, u32>) {
    let sample = "two1nine";
    let result = compute_line_2(table, sample);
    assert_eq!(result, 29);
}

fn test7(table: &HashMap<&str, u32>) {
    let sample = "abcone2threexyz";
    let result = compute_line_2(table, sample);
    assert_eq!(result, 13);
}

fn test8(table: &HashMap<&str, u32>) {
    let sample = "xtwone3four";
    let result = compute_line_2(table, sample);
    assert_eq!(result, 24);
}

fn test9(table: &HashMap<&str, u32>) {
    let sample = "4nineeightseven2";
    let result = compute_line_2(table, sample);
    assert_eq!(result, 42);
}

fn test10(table: &HashMap<&str, u32>) {
    let sample = "zoneight234";
    let result = compute_line_2(table, sample);
    assert_eq!(result, 14);
}

fn test11(table: &HashMap<&str, u32>) {
    let sample = "7pqrstsixteen";
    let result = compute_line_2(table, sample);
    assert_eq!(result, 76);
}

fn test12(table: &HashMap<&str, u32>) {
    let sample = "eightwothree";
    let result = compute_line_2(table, sample);
    assert_eq!(result, 83);
}

fn test13(table: &HashMap<&str, u32>) {
    let sample = "eightwothree\ntwo1nine\nabcone2threexyz\nxtwone3four\n4nineeightseven2\nzoneight234\n7pqrstsixteen";

    let mut result = 0;
    for line in sample.split(|ch: char| ch == '\n') {
        result += compute_line_2(table, &line);
    }
    assert_eq!(result, 281);
}

fn test14(table: &HashMap<&str, u32>) {
    let sample = "m2\ntwo19\nf3\n2nv\none8four1\n2one5\n286nx";

    let mut result = 0;
    for line in sample.split(|ch: char| ch == '\n') {
        result += compute_line_2(table, &line);
    }
    assert_eq!(result, 22+29+33+22+11+25+26);
}

fn main() {
    test1();
    test2();
    test3();
    test4();
    test5();

    let look_ups = HashMap::from([
        ("one",   1),
        ("two",   2),
        ("three", 3),
        ("four",  4),
        ("five",  5),
        ("six",   6),
        ("seven", 7),
        ("eight", 8),
        ("nine",  9),
    ]);
    test6(&look_ups);
    test7(&look_ups);
    test8(&look_ups);
    test9(&look_ups);
    test10(&look_ups);
    test11(&look_ups);
    test12(&look_ups);
    test13(&look_ups);
    test14(&look_ups);
    
    let filename = env::args().nth(1).expect("il faut donner le titre d'un fichier");
    let input: Vec<u8> = fs::read(filename).expect("c'est impossible de lire un ficher donné");

    let mut result: u32 = 0;
    let mut result2: u32 = 0;
    for line in input.split(|ch| *ch == 0xA) {
        let buf: String = line.into_iter().map(|b| char::from(*b)).collect();
        // result += compute_line(&buf);
        result2 += compute_line_2(&look_ups, &buf);
    }

    println!("1st part {}", result);
    println!("2nd part {}", result2);
}
