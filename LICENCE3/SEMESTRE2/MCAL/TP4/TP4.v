Section type_polymorphe.

(*-----------------------------------------------------*)
(* Q1.1 et 1.2 :  Type de l’identite polymorphe *)
Definition tid : Set := forall T: Set, T -> T.
Definition id : tid := fun T:Set => fun x:T => x.
Compute id nat 3. (*Renvoie bien 3. Si je met bool 3, on a une erreur.*)

(*Q1.3 : Application de id sur une fonction cette fois !*)
Definition nbtrue1 : bool -> nat := fun b =>
match b with
| true => 1
| false => 0
end.
Compute id (bool -> nat) nbtrue1. (*Il renvoie bien nbtrue1*)

(*Q1.4 : Application de id sur lui-même ?*)
Compute id tid id. (*Renvoie bien id*)

(*-----------------------------------------------------*)
(*Q2.1 : Définition des booléens polymorphes*)
Definition pbool : Set := forall T: Set,  T -> T -> T.

(* codage de true *)
Definition ptrue : pbool := fun T : Set => fun (x : T) (y : T) => x.

(* codage de false *)
Definition pfalse : pbool := fun T : Set => fun (x : T) (y : T) => y.
Compute ptrue nat 2 3. (*Renvoie bien 2*)
Compute pfalse nat 2 3. (*Renvoie bien 3*)

(*Q2.2 : Définition de la négation*)
Definition pnot : (pbool -> pbool) := fun (b : pbool) => fun T : Set => (fun x y => b T y x).
Compute (pnot ptrue).

End type_polymorphe.