
package cadeau;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Classe principale Cadeau : Permet d'emballer un objet dans du jolie papier.
 * @author Manal BENAISSA et Fabien LEFEBVRE
 */
public class Cadeau <T extends Product> {

    private T item;
    private double prix;
    
    /**
     * @param item L'objet a emballer dans le cadeau
     */
    public Cadeau(T item){
            this.item = item;
            this.prix = item.prix();
    }
    
    public void bonus(){
        item.bonus();
    }
    
    public double prix(){
        return item.prix();
    }
    
    //----------------------------------------------------
    public static void main(String[] args) {
        
        BoiteCaviar b = new BoiteCaviar(1.0, 300.0);
        Livre l;
        try {
            l = new Livre("La vie rêvée de Guigui HITLER", 12.0);   
                    
            Cadeau<BoiteCaviar> c1 = new Cadeau<>(b);
            Cadeau<Livre> c2 = new Cadeau<>(l);

            System.out.println("PRIX CADEAU 1 :"+c1.prix);
            c1.bonus();

            System.out.println("PRIX CADEAU 2 :"+c2.prix);
            c2.bonus();
        } catch (Exception ex) {
            System.err.println("Erreur ! " + ex.getMessage());
        }

    }

    
    
}
