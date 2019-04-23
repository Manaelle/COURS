/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package entreprise;

import java.util.ArrayList;

/**
 *
 * @author Manal BENAISSA & Fabien LEFEBVRE
 */
public class Personnel{
    
    ArrayList<Employe> ListEmploye;

    public Personnel() {
        this.ListEmploye = new ArrayList<>();
    }
    
    //--------------------------------------------------

    public void ajouterEmploye(Employe e){
        ListEmploye.add(e);
    }
    
    public void calculerSalaires(){
        
        
        ListEmploye.forEach((e) -> {
            System.out.println(e.getNom() + " gagne " + e.calculerSalaire());
        }
        );
    }
    
    public int salaireMoyen(){
        
        int somme = 0;
        
        for(Employe e : ListEmploye){
            somme += e.calculerSalaire();
        }
        
    
        return somme / ListEmploye.size();
        
    }
    
}
