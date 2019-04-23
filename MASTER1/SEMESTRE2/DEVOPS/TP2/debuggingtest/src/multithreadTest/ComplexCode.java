package multithreadTest;


import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class ComplexCode {

	public static void main(String[] args) {

		final int NUM_COUNTER=4;
		Thread[] counterThreads= new Thread[NUM_COUNTER];
		Map<Integer,Integer> result=new ConcurrentHashMap<Integer,Integer>();
		ProdCons buffer=new ProdCons();
		
		new Thread(new Reader(args[0],buffer)).start();
		
		for(int i=0; i<NUM_COUNTER; i++){
			counterThreads[i]=new Thread(new Counting(buffer,result));
			counterThreads[i].start();
		}
		
		for(int i=0; i<NUM_COUNTER; i++){
			try {
				counterThreads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		//display results
		int valueCount=0;
		int value=1;
	    System.out.println("word_size \t occurence_count");
		while(valueCount < result.size()){
			if(result.get(value)!=null){
				System.out.println(value+" \t\t "+result.get(value));
				valueCount++;
			}
			else{
		    	System.out.println(value+" \t\t "+0);
			}
			value++;
		}
		
	}

}
