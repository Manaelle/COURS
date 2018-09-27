/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "readcmd.h"
#include "fap.h"

//----------------------------------------------------
static void memory_error(void)
{
	errno = ENOMEM;
	perror(0);
	exit(1);
}

//----------------------------------------------------
static void *xmalloc(size_t size)
{
	void *p = malloc(size);
	if (!p) memory_error();
	return p;
}

//----------------------------------------------------
static void *xrealloc(void *ptr, size_t size)
{
	void *p = realloc(ptr, size);
	if (!p) memory_error();
	return p;
}

//----------------------------------------------------
/* Read a line from standard input and put it in a char[] */
static char *readline(void)
{
	size_t buf_len = 16;
	char *buf = xmalloc(buf_len * sizeof(char));
	
	if (fgets(buf, buf_len, stdin) == NULL) {
		free(buf);
		return NULL;
	}

	do {
		size_t l = strlen(buf);
		if ((l > 0) && (buf[l-1] == '\n')) {
			l--;
			buf[l] = 0;
			return buf;
		}
		if (buf_len >= (INT_MAX / 2)) memory_error();
		buf_len *= 2;
		buf = xrealloc(buf, buf_len * sizeof(char));
		if (fgets(buf + l, buf_len - l, stdin) == NULL) return buf;
	} while (1);
}

//----------------------------------------------------
/* Split the string in words, according to the simple shell grammar. */
static char **split_in_words(char *line)
{
	char *cur = line;
	char **tab = 0;
	size_t l = 0;
	char c;

	while ((c = *cur) != 0) {
		char *w = 0;
		char *start;
		switch (c) {
		case ' ':
		case '\t':
			/* Ignore any whitespace */
			cur++;
			break;
		case '<':
			w = "<";
			cur++;
			break;
		case '>':
			w = ">";
			cur++;
			break;
		case '|':
			w = "|";
			cur++;
			break;
		default:
			/* Another word */
			start = cur;
			while (c) {
				c = *++cur;
				switch (c) {
				case 0:
				case ' ':
				case '\t':
				case '<':
				case '>':
				case '|':
					c = 0;
					break;
				default: ;
				}
			}
			w = xmalloc((cur - start + 1) * sizeof(char));
			strncpy(w, start, cur - start);
			w[cur - start] = 0;
		}
		if (w) {
			tab = xrealloc(tab, (l + 1) * sizeof(char *));
			tab[l++] = w;
		}
	}
	tab = xrealloc(tab, (l + 1) * sizeof(char *));
	tab[l++] = 0;
	return tab;
}

//----------------------------------------------------
static void freeseq(char ***seq)
{
	int i, j;

	for (i=0; seq[i]!=0; i++) {
		char **cmd = seq[i];

		for (j=0; cmd[j]!=0; j++) free(cmd[j]);
		free(cmd);
	}
	free(seq);
}

//----------------------------------------------------
/* Free the fields of the structure but not the structure itself */
static void freecmd(struct cmdline *s)
{
	if (s->in) free(s->in);
	if (s->out) free(s->out);
	if (s->seq) freeseq(s->seq);
}

//----------------------------------------------------

struct cmdline *readcmd(void)
{
	static struct cmdline *static_cmdline = 0;
	struct cmdline *s = static_cmdline;
	char *line;
	char **words;
	int i;
	char *w;
	char **cmd;
	char ***seq;
	size_t cmd_len, seq_len;

	line = readline();
	if (line == NULL) {
		if (s) {
			freecmd(s);
			free(s);
		}
		return static_cmdline = 0;
	}

	cmd = xmalloc(sizeof(char *));
	cmd[0] = 0;
	cmd_len = 0;
	seq = xmalloc(sizeof(char **));
	seq[0] = 0;
	seq_len = 0;

	words = split_in_words(line);
	free(line);

	if (!s)
		static_cmdline = s = xmalloc(sizeof(struct cmdline));
	else
		freecmd(s);
	s->err = 0;
	s->in = 0;
	s->out = 0;
	s->seq = 0;
	s->esp = 0; //AJOUTE

	i = 0;
	while ((w = words[i++]) != 0) {
		switch (w[0]) {
		case '<':
			/* Tricky : the word can only be "<" */
			if (s->in) {
				s->err = "only one input file supported";
				goto error;
			}
			if (words[i] == 0) {
				s->err = "filename missing for input redirection";
				goto error;
			}
			s->in = words[i++];
			break;
		case '>':
			/* Tricky : the word can only be ">" */
			if (s->out) {
				s->err = "only one output file supported";
				goto error;
			}
			if (words[i] == 0) {
				s->err = "filename missing for output redirection";
				goto error;
			}
			s->out = words[i++];
			break;
		case '|':
			/* Tricky : the word can only be "|" */
			if (cmd_len == 0) {
				s->err = "misplaced pipe";
				goto error;
			}

			seq = xrealloc(seq, (seq_len + 2) * sizeof(char **));
			seq[seq_len++] = cmd;
			seq[seq_len] = 0;

			cmd = xmalloc(sizeof(char *));
			cmd[0] = 0;
			cmd_len = 0;
			break;
		//AJOUTE
		case '&':
			s->esp = 1;
			break;
		default:
			cmd = xrealloc(cmd, (cmd_len + 2) * sizeof(char *));
			cmd[cmd_len++] = w;
			cmd[cmd_len] = 0;
		}
	}

	if (cmd_len != 0) {
		seq = xrealloc(seq, (seq_len + 2) * sizeof(char **));
		seq[seq_len++] = cmd;
		seq[seq_len] = 0;
	} else if (seq_len != 0) {
		s->err = "misplaced pipe";
		i--;
		goto error;
	} else
		free(cmd);
	free(words);
	s->seq = seq;
	return s;
error:
	while ((w = words[i++]) != 0) {
		switch (w[0]) {
		case '<':
		case '>':
		case '|':
			break;
		default:
			free(w);
		}
	}
	free(words);
	freeseq(seq);
	for (i=0; cmd[i]!=0; i++) free(cmd[i]);
	free(cmd);
	if (s->in) {
		free(s->in);
		s->in = 0;
	}
	if (s->out) {
		free(s->out);
		s->out = 0;
	}
	return s;
}

//----------------------------------------------------
//AJOUTEE : renvoie le nombre total de commandes
int nbcommandes (struct cmdline *commandes) {
	int i, nb =0;
	
	for (i=0 ; commandes->seq[i]!=0; i++) {	
		nb++;
	}
	
	return nb;
}

//----------------------------------------------------
//AJOUTEE : fonction de test de réception d'un signal
void testsig(){
	
	printf("Signal reçu \n");
	exit(1);
}

//----------------------------------------------------
// AJOUTE : Exécute une commande par fils. prend en paramètre l->seq[i]
jobs executer(struct cmdline *commandes, jobs j ){
	
	int i;
	pid_t pid_actual, pid_pere;
	int  tubes[10][2]; // On se permet 10 tubes au max
	int status;
	int nbcmd = nbcommandes(commandes); //vaut 3 si y a 3 commandes...
	
	pid_pere = getpid();
	
	//Pour CHAQUE commande...
	for (i=0 ; i<nbcmd ; i++) {		
		
		//Création du tube SI il existe une commande suivante (seul le père crée les tubes, mais les fils héritent)
		if ( i < nbcmd-1 && getpid() == pid_pere ) { 
			if ( pipe(tubes[i]) == -1 ){
				perror("ERROR PIPE");
				exit(1);
			} 
		} 
		
		//Création des fils : Seul le père crée des fils
		if ( getpid() == pid_pere ) { 
			if( (pid_actual = fork()) == -1) {
				perror("ERROR FORK");
				exit(1);
			} 
		}
		
		switch (pid_actual){
		
		/* FILS */
		case 0:                     
				
				//Si le fils reçoit le signal Crtl+C ou Crtl+Z, il va exécuter la fonction testsig.
				if (!commandes->esp){
					signal(SIGINT, testsig); 
					signal(SIGTSTP, testsig);
				}
				
				/*Redirection sortie du tube vers entrée standard (cas du dernier fils)*/
				if (i>0) {

					dup2(tubes[i-1][0], 0);    
				}
				
				/*Redirection sortie standard vers entrée du tube (cas de 1er fils)*/
				if ( i < nbcmd-1 ) {
					
					dup2(tubes[i][1], 1); 
				}
				
				//Gestion du fichier d'entrée (in)
				if (commandes->in && i==0) {
					int f = open(commandes->in, O_RDWR); 
					dup2(f,0);
					close(f);
				}
				
				//Gestion du fichier de sortie (out)
				if (commandes->out && i==nbcmd-1) {
					int f = open(commandes->out, O_CREAT | O_RDWR | O_TRUNC  , S_IRUSR | S_IWUSR);
					dup2(f,1);
					close(f);
				}
				
				//Exécution (avec gestion d'erreur)
				if (execvp(commandes->seq[i][0], commandes->seq[i]) == -1) {
						perror("ERROR EXEC");
						exit(1);
				}
				exit(0);
				
						
		break;
		
		/*PERE */
		default:    
				//on stocke tous les fils bossant en arrière plan (=jobs)
				if (commandes->esp){
					j.list_jobs = Inserer(j.list_jobs, pid_actual, j.nbjobs++, commandes->seq[i][0] ); //Insère dans la file le pid du job, et un numéro ID.
				}
				
				// Fermeture des entrées de tubes inutiles (les tubes sont fermées chez les fils aussi)
				if (nbcmd > 1 && i < nbcmd-1 ){
					close(tubes[i][1]); 
				}
		break;
		}
		
		//On attend proprement les fils SI on a pas &. Sinon, les fils s'exécutent en tâche de fond, on ne les attend pas.
		if (!commandes->esp){
			waitpid(pid_actual, &status, 0); 
		}
   
	}
	
	
	//Fermeture de tous les tubes (que SI il y a des tubes à fermer)
	if (nbcmd > 1){
		for (i=0 ; i<nbcmd-1; i++) {	
				close(tubes[i][0]);	
		}
	}
	
	return j;
}

//----------------------------------------------------
//AJOUTE : Permet de stopper un processus grâce à son pid
void stop(pid_t pid){
	
	kill(pid,SIGTERM);
	
}

//----------------------------------------------------
//AJOUTE : Permet de stopper un processus grâce à son pid
void foreground(jobs j, pid_t pid){
	
	j.list_jobs = Extraire(j.list_jobs, &pid);
	if (tcsetpgrp(0, pid)==-1) {
		perror("ERREUR FG");
	}
	
}

//----------------------------------------------------
