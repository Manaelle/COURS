#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stddef.h>
#include "mem.h"

//-----------------------------------------
//--------------- DONNEES -----------------
//-----------------------------------------
//FB : Structure de métadonnées pour les blocks mémoire LIBRES
typedef struct fb{
	size_t taille;
	fb* next;
}fb;

//FB : Structure de métadonnées pour les blocks mémoire OCCUPEES
typedef struct ub{
	size_t taille;
}ub;

//MEMOIRE : donnée récupérée à init (pas modifié ensuite)
typedef struct memoire{
	char * mem;
	size_t taille;
}memoire;

memoire M;
size_t size_to_alloc;
//-----------------------------------------
//--------------- FONCTIONS ---------------
//-----------------------------------------
//MEM_INIT : crée un pointeur T vers le premier FB + création du 1er FB
void mem_init(char* mem, size_t taille){
	
	M.mem = mem;
	M.taille = taille;
	
	fb * T = ((fb *)M.mem)+1; //T est le pointeur vers fb, qui sera stocké en mem[0]
	fb **mem0 = (fb **)M.mem; //mem0 est pareil que mem mais casté en fb** (et non en char*)
	*mem0 = T;// idem que mem[0] = T mais sans problème de cast
	T->taille = taille - sizeof(fb *) - sizeof(fb) ;
	T->next = NULL;
	
	printf("---------------------------------\n");
	printf("INITIALISATION :\n Adresse de T : %p \n Nombre de zones libres : 1 \n Taille disponible :%zu \n", T, T->taille);
}	
//-----------------------------------------
//RECHERCHE : Cherche la première zone mémoire libre AC assez grande dans la liste chainée T,
//Si AC est le dernier de la liste -> créer un nouveau FB en fin de liste
// Sinon, rediriger AP->next vers AC->next
//Prend la liste T et une taille size en paramètre, retourne AC.
fb* mem_fit_first(fb* T, size_t size){
		
		fb* AC = T; //Le FB actuel
		fb* AP = T; //le FB précédent
		
		printf("---------------------------------\n");
		printf("ALLOCATION DE %zu OCTETS (+%lu octets pour UB) :\n", size, sizeof(ub));
		
		//ETAPE 1 : Chercher une zone libre assez grande dans la mémoire pour l'allouer.
		while((AC != NULL)&&((AC->taille) <= size+sizeof(ub))){
			AP = AC;
			AC = AC->next;
		}		
		
		//ETAPE 2 : On change la liste chainée pour exclure AC (qui sera remplacé par UB+ZONE_ALLOUEE)
		if(AC != NULL){
			
			printf("\nTAILLE AC vs SIZE+UB :%zu   %zu)\n", AC->taille, size+sizeof(ub));
			
			//CAS 1 : cas où on doit créer un nouveau FB en fin de liste chainée
			if(AC->next == NULL){
				//on décale le pointeur AC pour laisser de la place à l'allocation
				char* newAC = (char*)AC+sizeof(ub)+size+1; 
				
				//création de FB
				fb* newFB = (fb*)newAC;
				newFB->taille = (AC->taille)-sizeof(ub)-size; 
				newFB->next=NULL;
				printf("Nouveau FB : %p (de taille %zu)\n", newFB, newFB->taille);
				
				//redirection de la chaine
				if(AC == T){
					T = newFB;
					fb **mem0 = (fb **)M.mem; 
					*mem0 = T;
					printf("Nouveau T : %p\n", *(fb **)M.mem);
				}
				else{
					AP->next = newFB;
				}
				
				
				
			}
			//CAS 2 : La zone libre est en milieu de chaine
			else{
				//on vérifie dans la zone libre qu'il y a assez de place pour créer un nouveau FB
				if((AC->taille) > size+sizeof(ub)+sizeof(fb)){
						printf("[Construction de FB en milieu de chaine !] \n");
						char* newAC = (char*)AC+sizeof(ub)+size+1; 
						
						fb* newFB = (fb*)newAC;
						newFB->taille = (AC->taille)-sizeof(ub)-size; 
						newFB->next=AC->next;
						printf("Nouveau FB : %p (de taille %zu)\n", newFB, newFB->taille);
						
						if(AC == T){
							T = newFB;
							fb **mem0 = (fb **)M.mem; 
							*mem0 = T;
							printf("Nouveau T : %p\n", *(fb **)M.mem);
						}
						else{
							AP->next = newFB;
						}
				}
				// cas où on prend TOUTE la zone libre
				else {
					printf("[La zone allouée prend toute la zone libre !(pas de nouveau FB)] \n");
					
					if(AC == T){
							T = AC->next;
							fb **mem0 = (fb **)M.mem; 
							*mem0 = T;
							printf("Nouveau T : %p\n", *(fb **)M.mem);
						}
						else{
							AP->next = AC->next;
						}
					
				}
				
			}
		}
	return AC;
	
}
//-----------------------------------------
fb* mem_fit(mem_fit_function_t* f){
	
	fb * T = *((fb**)M.mem);
	fb* AC = f(T, size_to_alloc);
	return AC;
	
}
//-----------------------------------------
void* mem_alloc(size_t S) {
	
	ub * newU; //La future zone à allouer
	
	//ETAPE 1 ET 2 : Chercher une zone libre et remanipuler la liste chainée.
	size_to_alloc = S;
	newU = (ub*)mem_fit(&mem_fit_first); //retourne l'adresse où écrire newU
	printf("Création de UB : %p\n", newU);
	
	//ETAPE 3 : On crée UB
	newU -> taille = S;
	return ((void*)newU) + sizeof(ub); //On retourne le DEBUT de la zone allouée, UB exclue ! Il ne faut pas laisser l'occasion à l'utilisateur d'écraser UB...
	
}
//-----------------------------------------
void mem_free(void* m){
	
	ub* U;
	fb* newFB;
	
	//ETAPE 1 : Chercher le (vrai) début de la zone à libérer, cad avec UB
	U = (ub*)(m-sizeof(ub));
	size_t size = U->taille;
	
	//ETAPE 2 : Création du nouveau FB
	newFB = (fb*)U;
	newFB -> taille = size + sizeof(ub) - sizeof(fb);
	newFB->next = NULL;
	
	printf("---------------------------------\n");
	printf("LIBERATION DE %zu OCTETS (+%lu octets pour FB) :\n", size, sizeof(fb));
	
	//ETAPE 3 : Chercher la zone libre avant ET après newFB
	fb * T = *((fb**)M.mem);
	fb* AC = T; //Le FB actuel
	fb* AP = T; //le FB précédent
	
	while((AC != NULL)&&(AC < newFB)){ //on cherche la zone libre JUSTE avant newFB
		AP = AC;
		AC = AC->next;
	}
	
	printf("AP(old) : %p (de taille %zu) pointe vers %p \n", AP, AP->taille, AP->next);
	printf("AC(old) : %p (de taille %zu) pointe vers %p \n \n", AC, AC->taille, AC->next);
	
	//Normalement, à la fin, newFB se trouve entre AC et AP.
	if(AC != NULL){
		
	//ETAPE 4 : Rediriger les liens de la chaine
		if(AC==T){
			newFB->next = T;
			T = newFB;
			fb **mem0 = (fb **)M.mem; 
			*mem0 = T;
			printf("T(new) : %p (de taille %zu) pointe vers %p \n", T, T->taille, T->next);
		}
		else{
			newFB->next = AC;
			AP->next = newFB;
			
			printf("AP : %p (de taille %zu) pointe vers %p \n", AP, AP->taille, AP->next);
			printf("newFB : %p (de taille %zu) pointe vers %p \n", newFB, newFB->taille, newFB->next);
			printf("AC : %p (de taille %zu) pointe vers %p \n", AC, AC->taille, AC->next);
		}
		
	//ETAPE 5 : Fusionner newFB avec les zones libres voisines (AC et AP) SI POSSIBLE
	
		//Cas où on peut fusionner avec AC	 
		if ((char*)newFB + (newFB->taille) + sizeof(fb)+1 == (char*)AC){ 
			newFB->taille = (newFB->taille) + (AC->taille) + sizeof(fb);
			newFB->next = AC->next;
			printf("\n FUSION AC :\n newFB : %p (de taille %zu) pointe vers %p \n", newFB, newFB->taille, newFB->next);
		}
		
		//Cas où on peut fusionner avec AP
		if ((char*)AP + (AP->taille) + sizeof(fb)+1 == (char*)newFB){ 
			AP->taille = (AP->taille) + (newFB->taille) + sizeof(fb);	
			AP->next = newFB->next;
			printf("\n FUSION AP :\n AP : %p (de taille %zu) pointe vers %p \n", AP, AP->taille, AP->next);
		}
		
	}
	
}
//-----------------------------------------
size_t mem_get_size(void *m){
	
	return (size_t)NULL;
}
//-----------------------------------------
void mem_show(void (*print)(void *, size_t, int free)){

}


