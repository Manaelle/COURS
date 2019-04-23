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
public class Vendeur extends Employe {
    
    private int chiffreAffaire;
    
    public Vendeur(String n, String p, int a, Date d, int c) {
        super(n, p, a, d);
        this.chiffreAffaire = c;
    }
    
    //--------------------------------------------------
    
    @Override
    public int calculerSalaire() {
        
        return (this.chiffreAffaire*20/100)+100;
    }
    
    @Override
    public String getNom(){
        return "Le vendeur "+this.getName()+" "+this.getPrenom();
    }
    
    
}
