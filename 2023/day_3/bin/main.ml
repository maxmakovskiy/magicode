let map_rows : string list = [
  "467..114..";
  "...*......";
  "..35..633.";
  "......#...";
  "617*......";
  ".....+.58.";
  "..592.....";
  "......755.";
  "...$.*....";
  ".664.598.."
]

type vertex = {
  v : int;
  x : int list;
  y : int;
}

type symbol = {
  x: int;
  y: int;
}

let is_digit c = c >= '0' && c <= '9'
let is_anchor c = c <> '.'

let rec scan i line ~values ~symbols ~y : vertex list * symbol list  =
  let len = String.length line in
    if i >= len then
      (* acc is built backwards since List.rev_append (::) is used *)
      List.rev values, List.rev symbols

    else if is_digit line.[i] then
      let j = ref i in 
        (* Consume the digits *)
        while !j < len && is_digit line.[!j] do
          incr j
        done;

        (* Parse consumed digits to the int *)
        let value = int_of_string (String.sub line i (!j - i)) in
          scan !j line ~values:({v=value; x=(List.init (!j - i) (fun dx -> i + dx)); y=y} :: values) ~symbols:symbols ~y:y

    else if is_anchor line.[i] then
      scan (i + 1) line ~values:values ~symbols:({x=i; y=y} :: symbols) ~y:y
    else
      scan (i + 1) line ~values:values ~symbols:symbols ~y:y


let process_row row_idx row : vertex list * symbol list = scan 0 row ~values:[] ~symbols:[] ~y:row_idx 


let is_adjacent (v: vertex) (s: symbol) =
  List.exists (fun dx -> 
  abs (dx - s.x) <= 1 && abs (v.y - s.y) <= 1 &&
  not (dx = s.x && v.y = s.y)) v.x


let is_reachable v ~symbols = List.exists (fun sym -> is_adjacent v sym) symbols


(* let coord_to_str v : string = String.concat "," (List.map (fun s -> string_of_int s) v) *)

(* let () =
  let (values, symbols) = List.split (List.mapi process_row map_rows) in
  let str_vals = List.map 
    (fun p -> Printf.sprintf "(%d | x: %s, y: %d)" p.v (coord_to_str p.x) p.y) 
    (List.flatten values) in
  let str_syms = List.map 
    (fun p -> Printf.sprintf "(x: %d, y: %d)" p.x p.y) 
    (List.flatten symbols) in
  print_endline (String.concat "\n" str_vals);
  print_endline (String.concat "\n" str_syms)
 *)

let () =
  let (values, symbols) = List.split (List.mapi process_row map_rows) in
  let flat_vals = List.flatten values in
  let flat_syms = List.flatten symbols in
  List.filter (is_reachable ~symbols:flat_syms) flat_vals
  |> List.map (fun point -> point.v)
  |> List.fold_left (+) 0
  |> Printf.printf "(Sample) Part 1 : %d\n"


let () =
  let rows = Day_3.File.read_lines "data.txt" in
  let (values, symbols) = List.split (List.mapi process_row rows) in
  let flat_vals = List.flatten values in
  let flat_syms = List.flatten symbols in
  List.filter (is_reachable ~symbols:flat_syms) flat_vals
  |> List.map (fun point -> point.v)
  |> List.fold_left (+) 0
  |> Printf.printf "(Real) Part 1 : %d\n"

