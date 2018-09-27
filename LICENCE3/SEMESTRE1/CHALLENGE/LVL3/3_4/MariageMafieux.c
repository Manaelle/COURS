#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define NMAX 200

#define VIDE 0

/*--------------------------------------*/
//TYPES

//On a une liste d'invité...qui sont des membres d'une famille (pas de doublons ici)
typedef struct {
	int m;
	int Member[NMAX];
} invites;

//une famille est un tab de n membres
typedef struct {
	int nb_membres;
	int Family[NMAX];
} famille ;

//on a f familles répertoriés dans EnsF
typedef struct {
	int f;
	famille EnsF[NMAX];
} ensembleFamille ;

//Une photo est un ensemble contenant p personnes
typedef struct {
	int p;
	int Picture[NMAX];
} photo ;

//Et il nous faut un lot de n photos (contenant tous p personnes)
typedef struct {
	int n;
	photo EnsP[NMAX];
} ensemblePhoto ;

//On garde la liste des familles qui ne sont pas représentés, sur aucune photo
typedef struct {
	int n;
	int L[NMAX];
} listeFamilleNonRepresente ;

//Enfin, un mariage est consitué des données ci dessus :
typedef struct {
	invites ListInv;
	ensembleFamille ListFamily;
	int pmax;
	int nmax;
	ensemblePhoto ListPhoto;
	listeFamilleNonRepresente Reg;
} mariage ;

//ne servira qu'à l'init, pour choper les entrée sur stdin
typedef struct {
	int n;
	int D[NMAX];
} donnee ;

//donne des indications sur l'identité du membre : I est le numm-ème membre de la numf-ème famille
typedef struct {
	int numf;
	int numm;
} ID_membre ;

//une personne peut être dans plusieurx familles à la fois...
typedef struct {
	int n;
	ID_membre ID[NMAX];
} IDList ;

//Deux tableaux de hachage répertoriant le nombre d'apparition des membres + des familles dans l'album photo
typedef struct {
	int App[NMAX];
	int FamilleApp[NMAX];
} apparition ;
/*--------------------------------------*/
//PROTOTYPES
donnee Split_line(char cmd[NMAX], donnee X);
invites AjoutInvite(invites I, int numInv);
void AffichageMariage(mariage T);
void AfficherPhoto(photo P);
mariage InitMariage();
IDList InfoMembre(mariage T, int X);
mariage SupprUneFamille(mariage T, int f);
mariage SupprFamilleRepresente(mariage T, int X);
int TouteFamilleEstRepresente(mariage T, photo P);
photo AjoutPersonneSurPhoto(photo P, int X);
int NbApparitionDansAlbum(mariage T, int X);
apparition ListApparition(mariage T);
apparition FamilleApparition(mariage T, apparition A);
invites SupprMembre(invites E, int X);
mariage *EnumPhotosPossibles(mariage *T, invites E, int i, photo P);
mariage Calcul(mariage T);
ensemblePhoto AjoutPhoto(ensemblePhoto Al, photo P);


/*--------------------------------------*/
//FONCTIONS
donnee Split_line(char cmd[NMAX], donnee X){
	
	const char delim[1] = " ";
	char *word;
	int i=0;
	
	X.n = 0;
	word = strtok(cmd, delim);
   
   //Pour chaque mot, le ranger
   while( word != NULL && i<NMAX ){

		X.D[i] = atoi(word);
		X.n++;
		i++;
		word = strtok(NULL, delim);
   }
   
   return X;
	
}
/*----*/
invites AjoutInvite(invites I, int numInv){
	
	int i;
	
	for(i=0;i<I.m; i++){	
		if (I.Member[i] == numInv){
			return I;
		}
	}
	I.Member[I.m] = numInv;
	I.m++;
	return I;
}
/*----*/
void AffichageMariage(mariage T){
	
	int i,j;
	
	printf("------------------------------------\n");
	printf("Membres assistant au mariage : m = %d \n [", T.ListInv.m);
	
	for(i=0;i<T.ListInv.m; i++){
		printf("%d ", T.ListInv.Member[i]);
	}
	printf("]\n\n------------------------------------\n");
	printf("Liste des familles : f = %d \n\n", T.ListFamily.f);
	
	for(i=0;i<T.ListFamily.f; i++){
		printf("Famille n° %d : %d membres :\n",i, T.ListFamily.EnsF[i].nb_membres);
		for(j=0;j<T.ListFamily.EnsF[i].nb_membres; j++){
			printf("%d ", T.ListFamily.EnsF[i].Family[j]);
		}
		printf("\n\n");
	}
	
	printf("------------------------------------\n");
	printf("Nb personnes par photo exigé : p = %d \n", T.pmax);
	printf("Nombre de photos demandées : n = %d \n", T.nmax);
	printf("Liste des photos prises : n = %d \n\n", T.ListPhoto.n);
	
	for(i=0;i<T.ListPhoto.n; i++){
		printf("Photo n° %d (%d pers.) : [ ",i, T.ListPhoto.EnsP[i].p);
		for(j=0;j<T.ListPhoto.EnsP[i].p; j++){
			printf("%d ", T.ListPhoto.EnsP[i].Picture[j]);
		}
		printf("]\n");
	}
	printf("------------------------------------\n");

	printf("\nActuellement, %d familles ont été oubliés sur les photos... :\n", T.Reg.n );
	for(i=0;i<T.Reg.n; i++){
		printf("%d ", T.Reg.L[i]);
	}
	printf("\n\n");
	return;
	
}
/*----*/
mariage InitMariage(){
	
	mariage M;
	char c[NMAX];
	donnee E;
	int i,j;
		
	//Initialisation de base : m f n et p
	E.n = 0;
	fgets(c, sizeof(c), stdin);
	E = Split_line(c,E);
	
	M.ListInv.m = E.D[0]; //m
	M.ListFamily.f = E.D[1]; //f
	M.nmax = E.D[2]; //n
	M.pmax = E.D[3]; //p
	
	//nombre de photos prises pour l'instant
	M.ListPhoto.n = 0;
	
	for(i=0;i<M.ListPhoto.n;i++){
		M.ListPhoto.EnsP[i].p = 0;
	}
	
	M.Reg.n = 0;
	M.ListInv.m = 0;
	//Initialisation des familles + des invités
	for(i=0;i<M.ListFamily.f;i++){
		E.n = 0;
		fgets(c, sizeof(c), stdin);
		E = Split_line(c,E);
		
		//Ajout des familles non représentés (cad toutes les famille ici) 
		M.Reg.L[i] = i;
		M.Reg.n++;
		
		//remplissage de la famille i
		M.ListFamily.EnsF[i].nb_membres = E.n;
		
		for(j=0;j<E.n; j++){
			M.ListFamily.EnsF[i].Family[j] = E.D[j];
			
		}	
		//remplissage des ListInv
		for(j=0;j<E.n; j++){
			 M.ListInv = AjoutInvite(M.ListInv, E.D[j]);
			 
		}
		
	}

	
	return M;
}

/*----*/
IDList InfoMembre(mariage T, int X){
	
	int i, j;
	ID_membre I;
	IDList IDx;
	
	IDx.n = 0;
	
	//On regarde où se trouve X dans chaque famille : 
	for(i=0;i< T.ListFamily.f;i++){
			for(j=0;j< T.ListFamily.EnsF[i].nb_membres ;j++){
					
					if (T.ListFamily.EnsF[i].Family[j] == X){
							I.numf = i;
							I.numm = j;
							IDx.ID[IDx.n] = I;
							IDx.n++;
					}
					
			}
	}
	
	return IDx;
}

/*----*/
invites SupprMembre(invites E, int X){
	//Suppression du membre dans la liste des membres	
	int i,j;
	
	
	for(i=0;i< E.m ;i++){
	
		if( X == E.Member[i]){
			for(j=i;j< E.m ;j++){
				E.Member[j] = E.Member[j+1];
			}
			E.m--;
			return E;
		}
	}
	
	return E;
}
/*----*/
photo AjoutPersonneSurPhoto(photo P, int X){
	//On veut ajouter le membre X sur la photo numPhoto

	P.Picture[P.p] = X;
	P.p++;
	
	return P;
}
/*----*/
int NbApparitionDansAlbum(mariage T, int X){
	//renvoie le nombre d'apparition de la personne X dans l'Album photo
	int nbX = 0;
	int i, j;
	
	for(i=0;i<T.ListPhoto.n; i++){
		
		for(j=0;j<T.ListPhoto.EnsP[i].p; j++){
			if( T.ListPhoto.EnsP[i].Picture[j] == X){
				nbX++;
			}
		}	
	}
	
	return nbX; 
}
/*----*/
apparition ListApparition(mariage T){
	//Tableau d'apparition : La personne i est apparu A[i] fois.
	apparition A;
	int i;
	
	for(i=0;i<T.ListInv.m; i++){
		
		A.App[T.ListInv.Member[i]] = NbApparitionDansAlbum(T,T.ListInv.Member[i]);
		printf("%d est apparu %d fois sur l'album.\n",T.ListInv.Member[i], A.App[T.ListInv.Member[i]]);
		
	}
	
	return A; 
}
/*----*/
apparition FamilleApparition(mariage T, apparition A){
	
	int i,j,k;
	
	// i est le num de la famille
	for(i=0;i<T.ListFamily.f; i++){
		A.FamilleApp[i] = 0;
		
		//j est un membre
		for(j=0;j<T.ListInv.m; j++){
			
			//on regarde les familles k de j
			for(k=0;k<InfoMembre(T,T.ListInv.Member[j]).n; k++){
				
				if (InfoMembre(T,T.ListInv.Member[j]).ID[k].numf == i ){
					A.FamilleApp[i] = A.FamilleApp[i] + A.App[T.ListInv.Member[j]];
				}
			}
		}
		printf("La famille %d a fait %d apparitions \n", i, A.FamilleApp[i] );
	}
	
	return A; 
}
/*----*/
ensemblePhoto AjoutPhoto(ensemblePhoto Al, photo P){
	
	int i;
	
	for(i=0; i< P.p;i++){
		Al.EnsP[Al.n].Picture[i] = P.Picture[i];
		Al.EnsP[Al.n].p++;
	}
	Al.n++;
	return Al;
}
/*----*/
mariage SupprFamilleRepresente(mariage T, int X){
	//Suppression des familles prises en photo dans la liste	
	int i,j, k;
	
	IDList I = InfoMembre(T,X);
	
	for(k=0;k< I.n;k++){
		//printf("Suppr de la famille %d \n", I.ID[k].numf);
		T = SupprUneFamille(T, I.ID[k].numf);
	}
	
	return T;
	
}
/*----*/
mariage SupprUneFamille(mariage T, int f){
	//Suppression des familles prises en photo dans la liste	
	int i,j;
	
		
		for(i=0;i< T.Reg.n ;i++){
	
			if(f == T.Reg.L[i]){
					//printf("Suppr de la famille %d \n", f);
					for(j=i;j< T.Reg.n-1;j++){
						T.Reg.L[j] = T.Reg.L[j+1];
					}
					T.Reg.n--;
			}
		}
	

	return T;
}
/*----*/
int TouteFamilleEstRepresente(mariage T, photo P){
	
	int i;
	
	for(i=0;i<P.p;i++){
			T = SupprFamilleRepresente(T, P.Picture[i]);
	}
	
	return (T.Reg.n == 0);
	
}
/*----*/
mariage Calcul(mariage T){
	
	int i;
	photo Px;
	mariage *ptrM = &T;
	apparition A;
	
	Px.p = 0;

	//On construit l'Album Photo...
	invites E = T.ListInv;
	E = SupprMembre(E,1);
	E = SupprMembre(E,2);
	Px.p = 0;
	Px = AjoutPersonneSurPhoto(Px, 1);
	Px = AjoutPersonneSurPhoto(Px, 2);
	ptrM = EnumPhotosPossibles(ptrM,E,E.m,Px);
	
	//Et on regarde si y a n apparitions de chaque famille
	 A = ListApparition(T);
	 A = FamilleApparition(T,A);
	//T = SupprFamilleRepresente(T, 1);
	
	//NbApparitionDansAlbum(mariage T, int X)
	
	return T;
}
/*----*/
void AfficherPhoto(photo P){
	
	int i;
	
	printf("[ ");
	for(i=0;i<P.p; i++){
		printf("%d ", P.Picture[i]);
	}
	printf("]\n");
	
}
/*----*/
mariage *EnumPhotosPossibles(mariage *T, invites E, int i, photo P){
	
	if (i==0){
		
		if ((P.p <= T->pmax)){
			//AfficherPhoto(P);
			T->ListPhoto = AjoutPhoto(T->ListPhoto,P);
		}
		return T;
		
	}
	else{	
		
		T = EnumPhotosPossibles(T, E, i-1, P);
		
		//On ajoute un membre dans la photo
		P = AjoutPersonneSurPhoto(P, E.Member[i-1]);
		//Et on supprime cette personne de la liste E
		E = SupprMembre(E, E.Member[i-1]);
		
		T = EnumPhotosPossibles(T, E, i-1, P);
		
	}	
}
	
/*--------------------------------------*/
//MAIN
int main(){
	
	mariage DelPadre;
	photo P;
	
	
	DelPadre = InitMariage();
	//AffichageMariage(DelPadre);
	
	/*P.Picture[0]=1;
	P.Picture[1]=2;
	P.Picture[2]=8;
	P.Picture[3]=7;
	P.Picture[4]=3;
	P.p=5;
	AfficherPhoto(P);
	DelPadre = AjoutPhoto(DelPadre,P);
	//printf("OK = %d\n", TouteFamilleEstRepresente(DelPadre,P));*/
	
	DelPadre = Calcul(DelPadre);
	AffichageMariage(DelPadre);
	 
	
	
	 
  return 0;
}


