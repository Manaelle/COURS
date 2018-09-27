import java.io.FileInputStream;
import java.util.LinkedList;

class enump {
    public static void main(String [] args) {
        FileInputStream f;
        Graphe g;

        try {
            f = new FileInputStream(args[0]);
            g = new Graphe(f);
            System.out.println("Le graphe est :");
            System.out.println(g);
            System.out.println("Son degre est : " + g.degre());
            if (g.arcsIndependants())
                System.out.println("Ses arcs sont independants");
            else
                System.out.println("Ses arcs ne sont pas independants");
            if (g.estCouplage())
                System.out.println("C'est un couplage");
            else
                System.out.println("Ce n'est pas un couplage");
            LinkedList<Arc>  m = new LinkedList<Arc>();
            m = couplage(g,m);
            if (2*m.size() >= g.nombreSommets()) {
            	System.out.println("Couplage parfait :");
            	System.out.println(m.toString());
            } else if (m.size() != 0) {
            	System.out.println("Pas de couplage parfait. Le meilleur couplage obtenu est :");
            	System.out.println(m.toString());
            } else {
            	System.out.println("Aucun couplage n'est possible");
            }
        } catch (Exception e) {
            System.out.println(e);
        }
   }
    
    public static LinkedList<Arc> couplage(Graphe A, LinkedList<Arc> M) {
    	if (estVide(A)) {
  		   return M;
  	   } else if (PasDArc(A)) {
  		   return null;
  	   } else {
  		   Arc a = Choisir(A);
  		   A = extraire(A, a.source, a.destination);
  		   M = couplage(A, M);
  		   if (!estVide(A)) {
  			   return M;
  		   } else {
  			   couplage(A, ajouter(M, a));
  		   }
  		   return M;
  	   }
    }
    
    private static LinkedList<Arc> ajouter(LinkedList<Arc> m, Arc a) {
    	m.add(a);
		return m;
	}

	private static Graphe extraire(Graphe g, int s, int d) {
		g.sommets[s] = null;
		g.sommets[d] = null;
		for (int i = 0; i<g.nombreSommets(); i++) {
    		Maillon courant = g.sommets[i];
            while (courant != null) {
            	if (courant.arc.source == s || courant.arc.source == d || courant.arc.destination == s || courant.arc.destination == d) {
            		courant.arc.source = -1;
            		courant.arc.destination = -1;
            	}
                courant = courant.suivant;
            }
    	}
		return g;
	}

	private static boolean PasDArc(Graphe g) {
    	for (int i = 0; i<g.nombreSommets(); i++) {
    		Maillon courant = g.sommets[i];
            while (courant != null) {
            	if (courant.arc.source != -1 && courant.arc.destination != -1) {
            		return false;
            	}
                courant = courant.suivant;
            }
    	}
    	return true;
	}

	public static boolean estVide(Graphe g) {
    	for (int i = 0; i<g.nombreSommets(); i++) {
    		if (g.sommets[i] != null) {
    			Maillon courant = g.sommets[i];
    			while (courant != null && courant.arc.source == -1 && courant.arc.destination == -1) {
    				courant = courant.suivant;
    			}
    			if (courant != null) {
        			return false;
    			}
    		}
    	}
    	return true;
    }
	
	public static Arc Choisir(Graphe g) {
		for (int i = 0; i<g.nombreSommets(); i++) {
			Maillon courant = g.sommets[i];
            while (courant != null) {
            	if (courant.arc.source != -1 && courant.arc.destination != -1) {
            		return courant.arc;
            	}
                courant = courant.suivant;
            }
		}
		System.out.println("ERREUR");
		return null;
	}
}
