Section type_booleen.
Variable T: Set.

(* les booleens sont des fonctions a deux parametres *)
Definition cbool := T -> T -> T.

(* codage de true *)
Definition true : cbool := fun x y => x.

(* codage de false *)
Definition false : cbool := fun x y => y.

(* codage de if *)
Variable U : T.
Variable V : T.

Definition cif : cbool -> (T -> (T -> T)) := fun b x y => b x y.
Compute cif true U V.

Definition cnot : cbool -> cbool := fun b => (fun x y => b y x).
Compute cif (cnot true) U V.

Definition cand : cbool -> (cbool -> cbool) := fun a b => fun x y => a (b x y) y.
Compute cif (cand true false) U V.
Definition cor : cbool -> (cbool -> cbool) := fun a b => fun x y => a x (b x y).
Compute cif (cor true false) U V.

(* Test de preuves *)
Theorem not_true : cnot true = false. Proof. reflexivity. Qed.
(*Theorem not_false : cnot true = true. Proof. reflexivity. Qed.*)

End type_booleen.

Section type_entier.
Variable T: Set.
Definition cnat := (T->T) -> T->T.

Definition c0 : cnat := fun f x => x.
Compute c0.

Definition c1 : cnat := fun f x => f x.
Compute c1.

Definition c2 : cnat := fun f x => f (f x).
Compute c2.

Definition succ : cnat -> cnat := fun n => fun f x => (f (n f x)).
Compute succ c0. (*renvoie bien c1*)
Compute succ c1. (*renvoie bien c2*)

(* Definition de la composition de g et f *)
Definition compo : (T->T) -> (T->T) -> (T->T) := fun g f => fun x => g (f x).
(* Un raccourci syntaxique pour ecrire g o f au lieu de (compo g f ) *)
Notation "g ° f" := (compo g f) (at level 10).
(* Un iterateur de f, n fois *)
Fixpoint iter (f:T->T) (n: nat) :=
match n with
| O => fun x => x
| S p => f ° (iter f p)
end.
(* Utilisation de cet it ́erateur pour construire un cnat a partir d’un nat standard *)
Definition cnat_of : nat -> cnat := fun n => fun f => (iter f n).

(* Raccourci syntaxique pour  ́ecrire le nème entier de Church [n]N au lieu de (cnat n) *)
Notation "[ X ]N " := (cnat_of X) (at level 5).
(* par exemple [3]N signifie (cnat 3) et donc (après réduction) : λf x.f(f(f x)) *)

Compute succ [ 1 ]N. (*renvoie bien c2*)
Compute succ [ 5 ]N. (*renvoie bien c6*)

(*ADDITION*)
Definition add : cnat -> cnat -> cnat := fun n => fun m => fun f x => (n f (m f x)).
Compute add [ 5 ]N [ 1 ]N. (*renvoie bien [ 6 ]N *)
Compute add [ 5 ]N [ 4 ]N. (*renvoie bien [ 6 ]N *)

(*MULTIPLICATION*)
Definition mul : cnat -> cnat -> cnat := fun n => fun m => fun f => (n (m f)).
Compute mul [ 5 ]N [ 0 ]N. (*renvoie bien [ 0 ]N *)
Compute mul [ 3 ]N [ 2 ]N. (*renvoie bien [ 6 ]N *)

(*TEST A ZERO*)
Definition testzero : cnat -> (T -> T -> T) := fun n => fun x y => (n (fun z => y) x).
Compute testzero [ 5 ]N. (*renvoie bien faux *)
Compute testzero [ 0 ]N. (*renvoie bien vrai *)

(*BUG : Pour le test à zéro, si je met cbool à la place de (T -> T -> T), coqide n'aime pas. Pourquoi ?*)

End type_entier.