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
Theorem not_false : cnot true = true. Proof. reflexivity. Qed.

End type_booleen.