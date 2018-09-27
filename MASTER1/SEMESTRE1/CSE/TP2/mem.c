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
	
}
//-----------------------------------------
//RECHERCHE : Cherche la première zone mémoire libre AC assez grande dans la liste chainée T,
//Si AC est le dernier de la liste -> créer un nouveau FB en fin de liste
// Sinon, rediriger AP->next vers AC->next
//Prend la liste T et une taille size en paramètre, retourne AC.
fb* mem_fit_first(fb* T, size_t size){
		
		fb* AC = T; //Le FB actuel
		fb* AP = T; //le FB précédent
		size_to_alloc = size;
		
		//ETAPE 1 : Chercher une zone libre assez grande dans la mémoire pour l'allouer.
		while((AC != NULL)&&((AC->taille) <= size+sizeof(ub))){
			AP = AC;
			AC = AC->next;
		}
		
		//ETAPE 2 : On change la liste chainée pour exclure AC (qui sera remplacé par UB+ZONE_ALLOUEE)
		if(AC != NULL){
			
			//CAS 1 : cas où on doit créer un nouveau FB en fin de liste chainée
			if(AC->next == NULL){
				//on décale le pointeur AC pour laisser de la place à l'allocation
				char* newAC = (char*)AC+sizeof(ub)+size+1; 
				
				//création de FB
				fb* newFB = (fb*)newAC;
				newFB->taille = (AC->taille)-sizeof(ub)-size; 
				newFB->next=NULL;
				
				//redirection de la chaine
				if(AP = T){
					T = newFB;
				}
				else{
					AP->next = newFB;
				}
				
			}
			//CAS 2 : La zone libre est en milieu de chaine
			else{
				//on vérifie dans la zone libre qu'il y a assez de place pour UB+ZONE_ALLOUEE+FB
				if((AC->taille) > size+sizeof(ub)+sizeof(fb)){
						char* newAC = (char*)AC+sizeof(ub)+size+1; 
						
						fb* newFB = (fb*)newAC;
						newFB->taille = (AC->taille)-sizeof(ub)-size; 
						newFB->next=AC->next;
						
						if(AP = T){
							T = newFB;
						}
						else{
							AP->next = newFB;
						}
				}
				else{
					//A COMPLETER : Cas où pas de place pour FB
				}
				
				
			}
			
			
			return AC;
		}
	
}
//-----------------------------------------
void mem_fit(mem_fit_function_t* f){
	
	fb* T = (fb *)M.mem;
	fb* AC = f(T, size_to_alloc);
	
}
//-----------------------------------------
void* mem_alloc(size_t S) {
	
	fb * T = (fb *)M.mem;
	fb * AC = T; //Le FB actuel
	fb * AP = T; //le FB précédent
	
	
	mem_fit(&mem_fit_first);
	
	
	return NULL;
}
//-----------------------------------------
void mem_free(void* m){

}
//-----------------------------------------
size_t mem_get_size(void *m){
	
	return (size_t)NULL;
}
//-----------------------------------------
void mem_show(void (*print)(void *, size_t, int free)){

}


