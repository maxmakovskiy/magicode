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

type value =
  | Integer of int
  | Symbol of char

type coord =
  | Point of int
  | Range of int * int

type vertex = {
  v : value;
  x : coord;
  y : coord;
}

let is_digit c = c >= '0' && c <= '9'
let is_anchor c = c <> '.'

let rec scan i acc line ~y : vertex list =
  let len = String.length line in
    if i >= len then
      (* acc is built backwards since List.rev_append (::) is used *)
      List.rev acc

    else if is_digit line.[i] then
      let j = ref i in 
        (* Consume the digits *)
        while !j < len && is_digit line.[!j] do
          incr j
        done;

        (* Parse consumed digits to the int *)
        let value = int_of_string (String.sub line i (!j - i)) in
          scan !j ({v=Integer value; x=Range (i, !j); y=Point y} :: acc) line ~y:y

    else if is_anchor line.[i] then
      scan (i + 1) ({v=Symbol line.[i]; x=Point i; y=Point y} :: acc) line ~y:y
    else
      scan (i + 1) acc line ~y:y

let process_row row_idx row : vertex list = scan ~y:row_idx 0 [] row

let coord_to_str v : string =
  match v with
  | Point p -> Printf.sprintf "%d" p
  | Range (x1,x2) -> Printf.sprintf "%d-%d" x1 x2

let print_point (p : vertex) =
  match p.v with
  | Integer i -> Printf.printf "(%d | x: %s, y: %s)\n" i (coord_to_str p.x) (coord_to_str p.y)
  | Symbol s -> Printf.printf "(%c | x: %s, y: %s)\n" s (coord_to_str p.x) (coord_to_str p.y)


(* let is_reachable v ~vertices = List.mem v vertices && v.x >= 0 && v.y >= 0 *)


let () = 
  List.map print_point (List.flatten (List.mapi process_row map_rows)) |> (fun _ -> ())
    (* List.filter (is_reachable ~vertices:vertices) vertices *)
      (* |> List.map (fun v -> match v.v with | Integer i -> i | Symbol _ -> 0)
      |> List.fold_left (+) 0
      |> (fun v -> Printf.printf "Part 1 : %d\n" v)
      |> (fun _ -> ()) *)
