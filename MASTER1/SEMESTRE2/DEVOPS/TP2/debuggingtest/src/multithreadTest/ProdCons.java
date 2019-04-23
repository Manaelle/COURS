package multithreadTest;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

/**
 * 
 * @author tropars
 *
 * Producer-consumer based on a circular buffer 
 */

public class ProdCons {

	static final int BUFFER_SIZE=15;
	
	final Integer[] buffer= new Integer[BUFFER_SIZE];
	final ReentrantLock lock = new ReentrantLock();
	final Condition notFull  = lock.newCondition();
	final Condition notEmpty = lock.newCondition();
	
	static int count=0;
	static int firstElem=0;
	static int lastElem=0;
	
	public void put(Integer a){
		lock.lock();
	     try {
	       while (count == buffer.length){
	    	   try {
	    		   notFull.await();
	    	   } catch (InterruptedException e) {
	    		   e.printStackTrace();
	    	   }
	       }
	       buffer[firstElem] = a;
	       firstElem=(firstElem+1)%buffer.length;
	       notEmpty.signal();
	     } finally {
	       lock.unlock();
	     }
	}
	
	
	public Integer get(){
		lock.lock();
		try{
			while (count == 0){
				try{
					notEmpty.await();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			Integer value =buffer[lastElem];
			lastElem=(lastElem+1)%buffer.length;
			count--;
			notEmpty.signal();
			return value;			
		} finally {
			lock.unlock();
		}
		
	}
	
}
