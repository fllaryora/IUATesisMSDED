package ar.com.botqueue.applet.graphic.List;

import java.util.ArrayList;
import ar.com.botqueue.applet.graphic.node.DotNode;
public class BotQueueList<E> extends ArrayList<E> {

	/**
	 * 
	 */
	public BotQueueList(){
		
		super();
	}
	
	private static final long serialVersionUID = 1L;
	
	
	public int indexOfJson(Object o) {
	    if (o != null) {
	    	int j = 0;
	        for (int i = 0; i < super.size(); i++){
	        	if(super.get(i) == null ){
	        		continue;
	        	}
	        	if(super.get(i) instanceof DotNode){
	        		continue;
	        	}
	        		
	            if (o.equals(super.get(i))){
	                return j;
	            }
	            j++;
	        }
	    }
	    return -1;
	}

}
