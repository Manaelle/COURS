# Mini-shell

## Installation

* Télécharger tous les fichiers.
* Puis compiler :

> $ make

## Exécution

Taper la commande :

> ./shell

On peut alors taper des commandes. Les pipes, et redirections fonctionnent correctement. Pour tester :

> cat < in.txt | cut -c1-4 | wc -l > out.txt

Les commandes peuvent également être exécutée en tâche de fond. Pour ceci, ajouter '&'. ./chrono est un programme de test qui envoie "bip" toutes les secondes, pendant 1 min.

> ./chrono > out.txt &

Le PID (ainsi qu'un numéro de jobs) est ajoutée à une liste chainée. Il est alors possible de consulter la liste des jobs grâce à la commande "jobs" :

> jobs

Le shell envoie le résultat suivant :

> -----------------------------------------------------
> Liste des jobs :
> [PID]   [N°Job]         [CMD]
>
> 04330   00000001        ./chrono
> 
> -----------------------------------------------------

Ce job peut éventuellement être stoppée par la commande "stop <PID>" :

> stop 04330

Le shell reconnait également les signaux Ctrl+C et Ctrl+Z : Pour Ctrl+C, le shell envoie "Signal recu" et s'arrête. Néanmoins, il est préférable de stopper le shell par la commande "exit" :

> exit

Taper sur "Entrée" successivement ne plante pas le shell. 

Malheureusement, bien qu'il existe quelques essaies, la commande fg et bg n'ont pas été implémentées.
De même, il existe des bug si un programme en tâche de fond sollicite stdin. 
