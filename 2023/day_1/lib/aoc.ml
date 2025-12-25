let is_digit ch =
  match ch with
  | '0' -> true
  | '1' -> true
  | '2' -> true
  | '3' -> true
  | '4' -> true
  | '5' -> true
  | '6' -> true
  | '7' -> true
  | '8' -> true
  | '9' -> true
  | _   -> false;;


let rec find_first_digit s =
  match s with
  | "" -> None
  | _ -> if is_digit (s.[0]) then Some s.[0]
         else find_first_digit (String.sub s 1 ((String.length s) - 1));;


let rec find_last_digit s =
  match s with
  | "" -> None
  | _ -> if is_digit (s.[(String.length s) - 1]) then Some s.[(String.length s) - 1]
  else find_last_digit (String.sub s 0 ((String.length s) - 1));;

