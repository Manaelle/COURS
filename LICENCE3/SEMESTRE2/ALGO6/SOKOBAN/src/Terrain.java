/*
Sokoban - implémentation manuelle et automatique du célèbre jeu
Copyright (C) 2009 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
import java.util.Scanner;
import java.io.InputStream;

class Terrain {
    private Case [][] terrain;
    private int largeur, hauteur;
    public int nbsac;
    public int score;
    
    private Terrain() {
    }

    public Terrain(InputStream s) {
        nbsac = lecture(new Scanner(s));
        score = 0;
    }

    public Terrain clone() {
        Terrain nouveau = new Terrain();
        nouveau.terrain = new Case[hauteur()][largeur()];
        nouveau.largeur = largeur();
        nouveau.hauteur = hauteur();
        nouveau.nbsac = nbsac;
        nouveau.copie(terrain);
        return nouveau;
    }

    public static Terrain defaut() {
        Terrain nouveau = new Terrain();
        nouveau.terrain = new Case[5][5];
        nouveau.nbsac = 1;
        nouveau.score = 0;
        nouveau.efface();
        nouveau.terrain[2][1] = Case.POUSSEUR;
        nouveau.terrain[2][2] = Case.SAC;
        nouveau.terrain[2][3] = Case.BUT;
        nouveau.largeur = 5;
        nouveau.hauteur = 5;
        return nouveau;
    }

    public int largeur() {
        return largeur;
    }
    
    public int hauteur() {
        return hauteur;
    }

    public Case consulter(int ligne, int colonne) {
        return terrain[ligne][colonne];
    }

    public void assigner(Case c, int ligne, int colonne) {
        terrain[ligne][colonne] = c;
    }
    
    public int lecture(Scanner s) {
    	int nbsac = 0;
        largeur = s.nextInt();
        hauteur = s.nextInt();
        terrain = new Case[hauteur][largeur];

        for (int i=0; i<hauteur(); i++) {
            String ligne;
            ligne = s.next();
            for (int j=0; j<largeur(); j++)
                switch (ligne.charAt(j)) {
                case '+':
                    terrain[i][j] = Case.OBSTACLE;
                    break;
                case 'X':
                    terrain[i][j] = Case.POUSSEUR;
                    break;
                case '&':
                    terrain[i][j] = Case.SAC;
                    nbsac ++;
                    break;
                case 'O':
                    terrain[i][j] = Case.BUT;
                    break;
                case 'Q':
                    terrain[i][j] = Case.SAC_SUR_BUT;
                    break;
                case '0':
                    terrain[i][j] = Case.POUSSEUR_SUR_BUT;
                    break;
                default:
                    terrain[i][j] = Case.LIBRE;
                }
        }
        return nbsac;
    }

    public String toString() {
        String resultat = largeur() + "\n" + hauteur();
        for (int i=0; i<hauteur(); i++) {
            resultat += "\n";
            for (int j=0; j<largeur(); j++)
                switch (terrain[i][j]) {
                case OBSTACLE:
                    resultat += "+";
                    break;
                case POUSSEUR:
                    resultat += "X";
                    break;
                case SAC:
                    resultat += "&";
                    break;
                case BUT:
                    resultat += "O";
                    break;
                case LIBRE:
                    resultat += ".";
                    break;
                case SAC_SUR_BUT:
                    resultat += "Q";
                    break;
                case POUSSEUR_SUR_BUT:
                    resultat += "0";
                    break;
                default:
                    resultat += "?";
                }
        }
        return resultat;
    }

    private void efface() {
        for (int i=0; i<terrain.length; i++)
            for (int j=0; j<terrain[0].length; j++)
                terrain[i][j] = Case.LIBRE;
    }

    private void copie(Case [][] t) {
        for (int i=0; i<t.length; i++)
            for (int j=0; j<t[0].length; j++)
                 terrain[i][j] = t[i][j];
    }

    public boolean equals(Terrain autre) {
        if ((autre.largeur() != largeur()) || (autre.hauteur() != hauteur()))
            return false;
        for (int i=0; i<hauteur(); i++)
            for (int j=0; j<largeur(); j++)
                if (terrain[i][j] != autre.terrain[i][j])
                    return false;
        return true;
    }
}