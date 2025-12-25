let read_lines file_name =
  In_channel.with_open_text file_name In_channel.input_lines


(* let rows = [
  "1abc2";
  "pqr3stu8vwx";
  "a1b2c3d4e5f";
  "treb7uchet"
]

let raw_found = List.map (fun s -> 
    String.make 1 (Option.get (Day_1.Aoc.find_first_digit s)) ^
    String.make 1 (Option.get (Day_1.Aoc.find_last_digit s))
  )
  rows *)

let raw_found = List.map (fun s -> 
    String.make 1 (Option.get (Day_1.Aoc.find_first_digit s)) ^
    String.make 1 (Option.get (Day_1.Aoc.find_last_digit s))
  )
  (read_lines "data.txt")

let rec sum l = 
  match l with
  | [] -> 0
  | head::tail -> head + sum tail;;

let () = Printf.printf "%d\n" (sum (List.map Stdlib.int_of_string raw_found))

