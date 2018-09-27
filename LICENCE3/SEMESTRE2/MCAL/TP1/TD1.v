Section td11.
Variable x : nat.
(*necessaire pour évaluation*)
Definition quest1 := x + 2.
Print quest1.
Compute quest1.
End td11.

Section td12.
Definition quest2 := fun x => x + 2.
Print quest2.
Compute quest2.
End td12.

Section td13.
Definition quest3 := fun x => x + 2.
Print quest3.
Compute quest3.
Definition quest3bis := quest3 3.
Print quest3bis.
Compute quest3bis.
End td13.

Section td14.
Definition quest4 := (fun x y => x - y) 10 8.
Print quest4.
Compute quest4.
End td14.

Section td15.
Variable f : nat -> nat.
Definition quest5 := f 3 + 1.
Print quest5.
Compute quest5.
End td15.

Section td16.
Variable f : nat -> nat.
Definition quest6 :=(fun f => f 3 + 1).
Print quest6.
Compute quest6.
End td16.

Section td17.
Variable f : nat -> (nat -> nat).
Definition quest7 := (fun f => f 10 8 + 1).
Print quest7.
Compute quest7.
End td17.

Section td18.
Variable y : nat.
(*necessaire pour évaluation*)
Definition quest8 := (fun x => x + 7) y.
Print quest8.
Compute quest8.
End td18.


Section td19.
Variable y : nat.
(*Impossible de définir:
Definition quest8 := (fun x => x + 7) y 4.
car (fun x => x + 7) y est du type nat si y est naturel
or on ne peut pas appliquer un nat comme 4 à un autre 
nat*)
Definition quest9 := (fun x => x + 7) y.
(*valide uniquement si on applique pas 4*)
Print quest9.
Compute quest9.
End td19.


Section td110.
Variable u : nat -> nat -> nat.
Variable w : nat.
Definition quest10 := (fun v => u v w) 4.
Print quest10.
Compute quest10.
End td110.


Section td111.
(*Non typable car dans (fun u => u v w) 4
on est obligé d'avoir u : nat mais on ne peut pas
appliquer des fonctions ou des variables à un nat
Definition quest11 := (fun u => u v w) 4. est invalide*)
End td111.


Section td112.
Definition quest12 := (fun f => f 1) (fun x => x + 2).
Print quest12.
Compute quest12.
End td112.
Section TD1.
(* Commentaire *)
Definition f := fun x => x + 2.
Print f.
Compute f.
Definition t := f 3.
Compute t.
Definition u := (fun x y => x - y) 8 10.
Compute u.

End TD1.