import java.util.*;

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
class Moteur {
    Terrain t;
    int lignePousseur, colonnePousseur;
    Stack<Point> sacs;

    Moteur(Terrain t) {
        this.t = t;
        sacs = new Stack<>();
        for (int i=0; i<t.hauteur(); i++){
            for (int j=0; j<t.largeur(); j++){
                if (t.consulter(i,j).contient(Case.POUSSEUR)) {
                    lignePousseur = i;
                    colonnePousseur = j;
                    
                }
                if (t.consulter(i,j).contient(Case.SAC)) {
                    sacs.push(new Point(i, j));
                }
            }
        }
        return;
    }

    public boolean action(int i, int j) {
        //DEPLACEMENT DU POUSSEUR
    	if (t.consulter(i,j).estLibre()) {
            Case courante = t.consulter(lignePousseur, colonnePousseur);
            
            if (existeChemin(lignePousseur, colonnePousseur, i, j)) {
            	courante = courante.retrait(Case.POUSSEUR);
                t.assigner(courante, lignePousseur, colonnePousseur);

                courante = t.consulter(i, j);
                courante = courante.ajout(Case.POUSSEUR);
                t.assigner(courante, i, j);

                lignePousseur = i;
                colonnePousseur = j;
                return true;
            } else {
            	System.out.println("ACTION IMPOSSIBLE");
            	return false;
            }
        //DEPLACEMENT DU SAC
        } else if (t.consulter(i, j).equals(Case.SAC) || t.consulter(i, j).equals(Case.SAC_SUR_BUT)) {
        	if (lignePousseur == i && colonnePousseur == j-1) {
        		deplacer(lignePousseur, colonnePousseur, i, j, i, j+1);
        	} else if (lignePousseur == i && colonnePousseur == j+1) {
        		deplacer(lignePousseur, colonnePousseur, i, j, i, j-1);
        	} else if (lignePousseur == i-1 && colonnePousseur == j) {
        		deplacer(lignePousseur, colonnePousseur, i, j, i+1, j);
        	} else if (lignePousseur == i+1 && colonnePousseur == j) {
        		deplacer(lignePousseur, colonnePousseur, i, j, i-1, j);
        	}
            return true;
        } else {
        	return false;
        }
    }
    
    public boolean deplacer(int pi, int pj, int i, int j, int newi, int newj) {
    	if (newi > 0 && newj > 0 && newi <t.hauteur() && newj < t.largeur()) {    	
	    	if (t.consulter(newi, newj).estLibre()) {
	    		if (t.consulter(i, j).equals(Case.SAC_SUR_BUT)) {
	                t.nbsac++;
	            }
	    		Case courante = t.consulter(i, j);
	            courante = courante.retrait(Case.SAC);
	            t.assigner(courante, i, j);
	
	            courante = t.consulter(newi, newj);
	            courante = courante.ajout(Case.SAC);
	            t.assigner(courante, newi, newj);
	            
	            courante = t.consulter(lignePousseur, colonnePousseur);
	            courante = courante.retrait(Case.POUSSEUR);
	            t.assigner(courante, lignePousseur, colonnePousseur);
	
	            courante = t.consulter(i, j);
	            courante = courante.ajout(Case.POUSSEUR);
	            t.assigner(courante, i, j);
	
	            lignePousseur = i;
	            colonnePousseur = j;
	            if (t.consulter(newi, newj).equals(Case.SAC_SUR_BUT)) {
	                t.nbsac--;
	            }
	        	t.score++;
	        	System.out.println("Score : " + t.score);
	    		return true;
	    	}
    	}
    	return false;
    }
    
    public boolean existeChemin(int di, int dj, int fi, int fj) {
    	Stack<Point> e = new Stack<Point>();
    	int [][] marq = new int[t.hauteur()][t.largeur()];
    	Point sc = new Point(di, dj);
    	Point sf = new Point(fi, fj);
    	marq[sc.x][sc.y] = 1;
    	e.add(sc);
    	while ((!e.isEmpty()) && (!sc.equals(sf))) {
    		sc = e.pop();
    		ArrayList<Point> a = new ArrayList<Point>();
    		a = sc.successeurs(t.hauteur(), t.largeur());
    		for (Point succ : a) {
    			if (t.consulter(succ.x, succ.y).estLibre()) {
    				if (marq[succ.x][succ.y] != 1) {
    					marq[succ.x][succ.y] = 1;
        				e.add(succ);
    				}
    			}
    		}
    	}
    	return sc.equals(sf);
    }
    
    public void solve() {
    	//solvM = new HashSet<>();
    	Point p = new Point(lignePousseur, colonnePousseur);
    	Point s = sacs.pop();
    	
    	//solveW(p, s);
    	System.out.println(solvM);
    	t.nbsac--;
    }
    /*
    public void solveW(Point p, Point s) {
    	CouplePoint c = new CouplePoint(p, s);
    	if (t.consulter(s.x, s.y).equals(Case.BUT)) {
    		return;
    	} else {
    		solvM.add(c);
    	}
    	ArrayList<CouplePoint> a = new ArrayList<CouplePoint>();
    	a = c.successeurs(t.hauteur(), t.largeur(), t);
    	System.out.println(a);
    	for (CouplePoint succ : a) {
    		if (!solvM.contains(succ)) {
				solveW(succ.p, succ.s);
				
    		}
    	}
    }
    */
}
