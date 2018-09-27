import java.io.*;

class EnumP {
    public static void main(String [] args) {
        FileInputStream f;
        Graphe g;

        try {
            f = new FileInputStream(args[0]);
            g = new Graphe(f);
            
        } catch (Exception e) {
            System.out.println(e);
        }
           
   }
}
