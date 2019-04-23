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
public abstract class Employe {
    
    private String nom;
    private String prenom;
    private int age;
    private Date dateEntreeService;
    
    public Employe(String n, String p, int a, Date d){
        this.nom = n;
        this.prenom = p;
        this.age = a;
        this.dateEntreeService = d;
    }
    
    //--------------------------------------------------
    public String getNom(){
        return "Employé :"+this.nom+" "+this.prenom;
    }
    
    //--------------------------------------------------
    public String getName(){
        return this.nom;
    }

    public void setNom(String nom) {
        this.nom = nom;
    }

    public void setPrenom(String prenom) {
        this.prenom = prenom;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public void setDateEntreeService(Date dateEntreeService) {
        this.dateEntreeService = dateEntreeService;
    }
    
    public String getPrenom(){
        return this.prenom;
    }
    
    public Number getAge(){
        return this.age;
    }
    
    public Date getDateEntreeService(){
        return this.dateEntreeService;
    }
     //--------------------------------------------------
    abstract int calculerSalaire();
    
}
