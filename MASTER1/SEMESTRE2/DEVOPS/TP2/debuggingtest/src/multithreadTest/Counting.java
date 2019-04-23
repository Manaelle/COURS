package multithreadTest;

import java.util.Map;

public class Counting implements Runnable {
	
	private ProdCons buffer;
	private Map<Integer,Integer> result;
	
	public Counting(ProdCons buffer, Map<Integer,Integer> result){
		this.buffer=buffer;
		this.result=result;
	}

	@Override
	public void run() {
		
		while(true){
			Integer wordsize=buffer.get();

			// that's our way to terminate
			if(wordsize==-1){
				buffer.put(wordsize);
				return;
			}

			Integer value;
			if((value=result.get(wordsize))==null){
				result.put(wordsize, 1);
			}
			else{
				result.put(wordsize, value+1);
			}
			System.out.println("Thread "+Thread.currentThread().getId()+": count word of size "+wordsize);
		}
	}

}
