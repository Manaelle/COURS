package multithreadTest;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Reader implements Runnable {

	private String filename;
	private ProdCons buffer;
	
	public Reader(String filename, ProdCons buffer){
		this.filename=filename;
		this.buffer=buffer;
	}
	
	@Override
	public void run() {
		Scanner sc2 = null;
	    try {
	        sc2 = new Scanner(new File(filename));
	    } catch (FileNotFoundException e) {
	        e.printStackTrace();  
	    }

	    while (sc2.hasNextLine()) {
	        Scanner s2 = new Scanner(sc2.nextLine());
	        while (s2.hasNext()) {
	            String s = s2.next();
	            buffer.put(s.length());
	        }
	        s2.close();
	    }
	    buffer.put(-1);
	}

}
