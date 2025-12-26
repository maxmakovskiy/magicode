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

let coord_to_str v : string = String.concat "," (List.map (fun s -> string_of_int s) v)


(* let is_reachable v ~vertices = 
  match v.v with
  | Symbol _ -> false
  | Integer _ -> 
    let i_row_before = v.y *)


let () =
  let (values, symbols) = List.split (List.mapi process_row map_rows) in
  let str_vals = List.map 
    (fun p -> Printf.sprintf "(%d | x: %s, y: %d)" p.v (coord_to_str p.x) p.y) 
    (List.flatten values) in
  let str_syms = List.map 
    (fun p -> Printf.sprintf "(x: %d, y: %d)" p.x p.y) 
    (List.flatten symbols) in
  print_endline (String.concat "\n" str_vals);
  print_endline (String.concat "\n" str_syms)

