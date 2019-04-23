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
public class BoiteCaviar implements Product {
    
    private double poids; //Plus pertinent d'utiliser des double plutôt que des String... (pour des raisons évidentes.)
    private double prixAuGramme;

    /**
     * 
     * @param prixAuGramme Le prix par gramme de caviar en euro
     * @param poids  Le poids de caviar dans la boîte en gramme
     */
    public BoiteCaviar(double prixAuGramme, double poids) {
        this.prixAuGramme = prixAuGramme;
        this.poids = poids;
    }
    
    
    @Override
    /**
     * Retourne le prix de la boite de caviar (€/gramme)
     * @return Le prix de la boîte
     */
    public double prix() {
        return this.poids*this.prixAuGramme;
    }

    
    @Override
    /**
     * 
     * Affichage du bonus à l'achat (réduction de 10%)
     */
    public void bonus() {
        System.out.println("REDUCTION de 10% ! Soit... "+ (this.prixAuGramme*this.poids*0.9)+"€");
    }
    
}
