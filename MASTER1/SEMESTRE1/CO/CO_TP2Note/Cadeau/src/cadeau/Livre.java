/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cadeau;

/**
 *
 * @author benaissm
 */
public class Livre implements Product {
    
    private String titre;
    private double prix;
    
    /**
     * 
     * @param titre Le titre du livre. 
     * @param prix Le prix du livre
     * @throws Exception Le titre ne doit pas être vide
     */
    public Livre(String titre, double prix) throws Exception{
        this.titre = titre;   
        this.prix = prix;
        
        if(this.titre.trim().length() == 0){
            throw new Exception("Le titre doit contenir au moins un mot");
        }
    }

    /**
     * 
     * @return Retourne le titre du livre 
     */
    public String getTitre() {
        return titre;
    }

      
    
    @Override
    /**
     * Renvoie le prix du livre
     * @return Retourne le prix du livre
     */
    public double prix() {
       return this.prix;
    }

    
    @Override
    /**
     * 
     * Affichage du bonus à l'achat (marque page offert ! <3)
     */
    public void bonus() {
        System.out.println("Un livre acheté = Un marque page offert ! <3");
    }
    
    
    
}
