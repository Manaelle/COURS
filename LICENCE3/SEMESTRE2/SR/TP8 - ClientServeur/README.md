# SR-Serveur-Projet

## Compilation :

> gcc -o clientFTP client.c  csapp.c -lpthread

> gcc -o esclaveFTP esclaves.c  echo.c csapp.c -lpthread

> gcc -o maitreFTP maitre.c  csapp.c -lpthread

## Exécution :

Le [port maitre] est le port sur lequel maitre et esclaves communiqueront. S'il y a N esclaves, le maitre communiquera sur les port [port maitre] à [port maitre + N]. 

> ./esclaveFTP <port_maitre>

> -----------------------------------------------------

- nb_esclaves : Le nombre d'esclaves en service.

- port_clients : Habituellement 2121. C'est le port par lequel les clients communiqueront avec le maitre.

- port_esclaves : idem que port_maitre. Attention, le port du premier esclave doit être identique que port_esclaves.

- host_esclave_N : Hostname de l'esclave numéro N, habituellement 127.0.0.1.

> ./maitreFTP <nb_esclaves> <port_clients> <port_esclaves> <host_esclave_1> <host_esclave_2> etc...

> -----------------------------------------------------
Le client ne communique que le port du maitre et son hostname.

> ./clientFTP <host_maitre> <port_maitre>

> -----------------------------------------------------

A SAVOIR : la commande ifconfig permet d'obtenir les adresses IP des machines. Bien insérer l'IP du MAITRE lors de l'exécution du client, et l'IP des ESCLAVES pour l'exécution du maitre.

## Utilisation :

En tant que client, vous avez un ensemble de commandes utilisables :

- GET : permet de télécharger le fichier spécifié.
> get <nom_du_fichier> 

- PUT : fournit le fichier spécifié au serveur.
> put <nom_de_fichier>

Ces deux commandes reprennent le téléchargement/chargement là où il en était en cas d'interruption. Si le serveur/client dispose déjà du fichier en entier, le téléchargement n'a pas lieu.
Ne pas donner plusieurs noms de fichiers en même temps !

- LS : affiche la liste des fichiers/dossiers dans le répertoire courant du serveur.
> ls

- PWD : Affiche le chemin absolu du répertoire courant.
> pwd

- CD : Permet de se déplacer au chemin spécifié, celui-ci pouvant être relatif ou absolu
> cd <chemin_voulu>

- MKDIR : permet de créer un dossier vide dont le nom est spécifié en paramètre
> mkdir <nom_dossier>

- RM : supprime le fichier spécifié. si l'option est -r, il supprime le dossier ainsi que son contenu. Une erreur est envoyée si la commande n'est pas adaptée à la nature de l'objet à supprimer.
> rm <nom_fichier>

> rm -r <nom_dossier>

- BYE : termine la session.
> bye

