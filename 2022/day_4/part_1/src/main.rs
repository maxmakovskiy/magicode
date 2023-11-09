use std::fs;


// Task
// - parse file
// - line - is one pair of intervals
// - count how many pairs have fully interlapped intervals


fn main() {
    let filename: String = std::env::args()
        .nth(1).expect("please provide file");
    let content: String = fs::read_to_string(filename)
        .expect("unable to read a given file"); 

    println!("content has length of {}", content.len());
    let result = content.split('\n')
        .filter(|line| line.len() > 0)
        .map(|line| {
            // println!("Searching for comma in {}", line);
            let (task1, task2) = line.split_once(',').unwrap();
            let ((p1_1, p1_2), (p2_1, p2_2)) = (
                task1.split_once('-').unwrap(),
                task2.split_once('-').unwrap()
            );
            ( // itervals: [p1_1, p1_2] and [p2_1, p2_2]
                p1_1.parse::<u8>().unwrap(),       
                p1_2.parse::<u8>().unwrap(),       
                p2_1.parse::<u8>().unwrap(),       
                p2_2.parse::<u8>().unwrap(),       
            )
        })
        .filter(|(p1_1, p1_2, p2_1, p2_2)| (p2_2 >= p1_2 && p2_1 <= p1_1) || (p1_2 >= p2_2 && p1_1 <= p2_1))
        .count();

    println!("number of overlapped pair is {}", result);
}
