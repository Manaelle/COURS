package firstTest;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;


/**
 * 
 * @author tropars
 *
 * Un premier code simple pour tester le débugger d'Eclipse
 * 
 * Ce code peut être vue comme une variante du problème "word count". 
 * Au lieu de compter le nombre d'occurence de mots dans un texte, il compte l'occurence de taille de mots.
 * On pourait appeler ça "word size count"
 */

public class SimpleCode {

	static ArrayList<Integer> occurence= new ArrayList<Integer>();
	
	
	public static void main(String[] args) {
		
            // scan the input file and count the length of each word
            Scanner sc2 = null;
	    try {
	        sc2 = new Scanner(new File(args[1]));
	    } catch (FileNotFoundException e) {
	        e.printStackTrace();  
	    }
	    
	    // I'm not proud of this piece of code
	    while (sc2.hasNextLine()) {
	        Scanner s2 = new Scanner(sc2.nextLine());
	        while (s2.hasNext()) {
	            String s = s2.next();
	            if(s.length() > occurence.size()){
	            	// add new entries initialized to 0 in the ArrayList (up to size s.length)
	            	occurence.ensureCapacity(s.length()+1);
	            	for(int i=occurence.size(); i<=s.length(); i++){
	            		occurence.add(0);
	            	}
	            }
                    try{
                        occurence.set(s.length(), occurence.get(s.length())+1);
                    }catch(Exception e){
                        System.out.println("!!!!!!! I IGNORE EXCEPTIONS !!!!!!!!");
                    }
                }
	    	s2.close();
	    }
		
	    
	    // display the results
	    System.out.println("word_size \t occurence_count");
	    for(int i=0; i < occurence.size(); i++){
	    	System.out.println(i+" \t\t "+occurence.get(i));
	    }
	    
	}

}
