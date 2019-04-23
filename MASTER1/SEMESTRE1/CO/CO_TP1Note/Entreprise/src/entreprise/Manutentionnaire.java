/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package entreprise;

import java.util.Date;

/**
 *
 * @author Manal BENAISSA & Fabien LEFEBVRE
 */
public class Manutentionnaire extends Employe {
    
    
    private int nbHeureTravail;
    
    public Manutentionnaire(String n, String p, int a, Date d, int nb) {
        super(n, p, a, d);
        this.nbHeureTravail= nb;
    }
    
    //--------------------------------------------------
    
    @Override
    public int calculerSalaire() {
        
        return this.nbHeureTravail*10;
    }
    
    @Override
    public String getNom(){
        return "Le Manutentionnaire "+this.getName()+" "+this.getPrenom();
    }
    
    
}
