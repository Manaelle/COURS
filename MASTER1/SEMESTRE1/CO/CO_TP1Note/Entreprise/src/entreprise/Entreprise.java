/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package entreprise;

import java.util.Date;

/**
 *
 * @author benaissm
 */
public class Entreprise {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Personnel p = new Personnel();
        Date d = new Date(92, 01, 01);
        
        p.ajouterEmploye(new Vendeur("BUSINESS", "Pierre", 77, d, 1000));
        p.ajouterEmploye(new Manutentionnaire("STOCKOUT", "Jeanne", 91, d, 500));
        p.ajouterEmploye(new Manutentionnaire("ABORDAGE", "Al", 02, d, 630));
        
        p.calculerSalaires();
        
        System.out.println("Le salaire moyen de l'entreprise est de " + p.salaireMoyen());
        
        
    }
    
}
