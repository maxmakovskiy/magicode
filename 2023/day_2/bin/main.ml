let red_ref = 12
let green_ref = 13
let blue_ref = 14

type set_t = {
  red : int;
  green : int;
  blue : int;
}

type game_t = {
  id : int;
  parts : set_t list;
}

(* set is " 1 red, 2 green, 6 blue" *)
(* 3 blue, 4 red *)
let parse_set (game_set : string) : set_t = 
  let parts = String.split_on_char ',' (String.trim game_set) in
  {
    red = List.fold_left (+) 0 (
      List.map (fun cubes -> Option.value (
        Scanf.sscanf_opt (String.trim cubes) "%d red" (fun b -> b)) ~default:0)
      parts);

    green = List.fold_left (+) 0 (
      List.map (fun cubes -> Option.value (
        Scanf.sscanf_opt (String.trim cubes) "%d green" (fun b -> b)) ~default:0) 
      parts);

    blue = List.fold_left (+) 0 (
      List.map (fun cubes -> Option.value (
        Scanf.sscanf_opt (String.trim cubes) "%d blue" (fun b -> b)) ~default:0) 
      parts);
  }


(* " 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green" *)
let parse_body (game_body : string) : set_t list = 
  List.map parse_set (String.split_on_char ';' game_body)


(* "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green" *)
let parse (row : string) : game_t = 
  let parts = String.split_on_char ':' row in
  {
    id = Scanf.sscanf (List.hd parts) "Game %d" (fun id -> id);
    parts = parse_body (List.nth parts 1)
  }


(* let is_legit (game : game_t) : bool = 
  (List.fold_left (+) 0 (List.map (fun p -> p.red) game.parts)) < red_ref &&
  (List.fold_left (+) 0 (List.map (fun p -> p.green) game.parts)) < green_ref &&
  (List.fold_left (+) 0 (List.map (fun p -> p.blue) game.parts)) < blue_ref *)


let is_legit (game : game_t) : bool = 
  not (List.mem false (List.map (fun p -> p.red <= red_ref) game.parts)) && 
  not (List.mem false (List.map (fun p -> p.green <= green_ref) game.parts)) && 
  not (List.mem false (List.map (fun p -> p.blue <= blue_ref) game.parts))


(* let rows: string list = [
  "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green";
  "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue";
  "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red";
  "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red";
  "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"
] *)

let () = 
  let res = Day_2.File.read_lines "data.txt"
  (* let res = rows *)
  |> List.map parse
  |> List.filter is_legit
  |> List.map (fun g -> g.id)
  |> List.fold_left (+) 0
in Printf.printf "Part 1: %d\n" res

