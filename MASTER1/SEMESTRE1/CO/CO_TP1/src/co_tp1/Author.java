package co_tp1;

/**
 * Classe de test pour le TP1
 * @author Manal BENAISSA
 * @version 1.0.0
 */
public class Author {

    private final String firstName;
    private final String lastName;
    
    /**
     * Constructeur de Author
     * @param firstName prénom de l'auteur
     * @param lastName nom de famille de l'auteur
     */
    public Author(String firstName, String lastName){
        this.firstName = firstName;
        this.lastName = lastName;
        
    }
    //--------------------------------------------
    /**
     * EQUALS compare un objet à un Author
     * @param o objet à identifier
     * @return TRUE si o est un Author est que son firstName et son lastName est identique à celui de la classe, FALSE sinon.
     */
    public boolean equals(Object o){
        
        if(!(o instanceof Author)){
            return false;
        }
        
        Author author = (Author)o;
        return (author.firstName.equals(this.firstName))&&(author.lastName.equals(this.lastName));
    }
    //--------------------------------------------
    /**
     * Main de Author, permet des tests lambda
     * @param args inutile ici
     */
    public static void main(String []args){
            
            Author A = new Author("Jean","MOULOUD");
            Author B = new Author("Jean", "MAHMOUD");
            
            System.out.println(A.equals(B));
           
    }
    
    //--------------------------------------------
    
}
