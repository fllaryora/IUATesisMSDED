package ar.com.botqueue.applet;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONObject;

import ar.com.botqueue.applet.enums.BindSurface;
import ar.com.botqueue.applet.enums.NodeFields;
import ar.com.botqueue.applet.enums.NodeTypes;
import ar.com.botqueue.applet.graphic.GraphicDTO;
import ar.com.botqueue.applet.graphic.arrow.GenericArrow;
import ar.com.botqueue.applet.graphic.node.Combi;
import ar.com.botqueue.applet.graphic.node.Counter;
import ar.com.botqueue.applet.graphic.node.Function;
import ar.com.botqueue.applet.graphic.node.Node;
import ar.com.botqueue.applet.graphic.node.NodeFactory;
import ar.com.botqueue.applet.graphic.node.Normal;
import ar.com.botqueue.applet.graphic.node.Queue;


/**
 * Recibe las ordenes de vaadin y las tranforma en ordenes a las clases
 */
public class VaadinFacade {
	private EventHandler nodeWorld;
	private GraphicDTO graphic;
	private Principal communicator;
	
	public VaadinFacade(GraphicDTO graphic, EventHandler nodeWorld,Principal communicator) {
		this.graphic = graphic;
		this.nodeWorld = nodeWorld;
		this.communicator = communicator;
	}
	
	public void doExecute(String command, Object[] params) {
		
		if (command.equalsIgnoreCase("createNode"))
        	createNode(params);
		else if (command.equalsIgnoreCase("deleteNode"))
            deleteNode();
        else if (command.equalsIgnoreCase("createArrow")) 	
   		 	createArrow();
        else if (command.equalsIgnoreCase("invertArrow"))
	   		invertArrow();
        else if (command.equalsIgnoreCase("deleteArrow"))
        	deleteArrow();
        else if (command.equalsIgnoreCase("rotateHead"))
   		 	rotateHead();
        else if (command.equalsIgnoreCase("rotateTail"))
   		 	rotateTail();
        else if (command.equalsIgnoreCase("deleteDot"))
   		 	deleteDot();
        else if (command.equalsIgnoreCase("createNextDot"))
   		 	createNextDot();
        else if (command.equalsIgnoreCase("createPreviusDot"))
   		 	createPreviusDot();
        else if (command.equalsIgnoreCase("getNodeInfo"))
        	getNodeInfo();
        else if (command.equalsIgnoreCase("editNode"))
        	editNode(params);
        else if (command.equalsIgnoreCase("getModelInfo"))
        	getModelInfo(params);
        else if (command.equalsIgnoreCase("getAllModelFile"))
        	getAllModelFile(params);
        else if (command.equalsIgnoreCase("getAllModelFile"))
        	getAllModelFile(params);
		
		this.nodeWorld.forcePaint();
	}
	
	/**
	 * Crea un nodo en el modelo 
	 */
	public void createNode(Object[] params){
		int node = 0;
		String label = null;
		NodeTypes nodeType;
		
		if(params.length == 2){
			node = this.toInteger(params[0]);
			label = (String)params[1];
			switch(node){
				case 1:
					nodeType = NodeTypes.COMBI;
					this.graphic.createNode(nodeType, label);
					break; 
				case 2:
					nodeType = NodeTypes.COUNTER;
					this.graphic.createNode(nodeType, label);
					break;
				case 3:
					nodeType = NodeTypes.QUEUE;
					this.graphic.createNode(nodeType, label);
					break;
				case 4: 
					nodeType = NodeTypes.FUNCTION;
					this.graphic.createNode(nodeType, label);
					break;
				case 5: 
					nodeType = NodeTypes.NORMAL;
					this.graphic.createNode(nodeType, label);
					break;
				default: break;
			}
		}
	}
	
	/**
	 * Elimina un nodo seleccionado en el modelo 
	 */
	public void deleteNode(){
		this.graphic.deleteNode();
	}
	
	/**
	 * Crea una flecha entre dos nodos 
	 */
	public void createArrow(){
		this.graphic.createArrow( nodeWorld.getZoom() );
	}
	
	/**
	 * invierte solo una flecha 
	 */
	public void invertArrow(){
		this.graphic.invertArrow();
	}
	
	/**
	 * elimina solo una flecha 
	 */
	public void deleteArrow(){
		this.graphic.deleteArrow();
	}
	
	/**
	 * rota la punta de una flecha 
	 */
	public void rotateHead(){
		this.graphic.rotateHead();
	}
	
	/**
	 * rota la cola de una flecha 
	 */
	public void rotateTail(){
		this.graphic.rotateTail();
	}
	
	/**
	 * elimina punto de una flecha 
	 */
	public void deleteDot(){
		this.graphic.deleteDot();
	}
	
	/**
	 * crea un punto entre dos puntos
	 * punto posterior 
	 */
	public void createNextDot(){
		this.graphic.createNextDot(nodeWorld.getZoom());
	}
	
	/**
	 * crea un punto entre dos puntos
	 * punto anterior 
	 */
	public void createPreviusDot(){
		this.graphic.createPreviusDot(nodeWorld.getZoom());
	}
	
	/** 
	 * get all data of some node
	 * */
	public void getNodeInfo(){
		this.graphic.getNodeInfo(this.communicator);
	}
	
	/** 
	 * set all data of some node
	 * */
	public void editNode(Object[] params){
		int quantity = 0;
		int resource = 0;
		int cycle = 0;
		double fixedCost = 0.0;
		double variableCost = 0.0;
		int input = 0;
		int output = 0;
		boolean probBranch = false;
		String distribution = null; 
		int seed = 0;
		double least = 0.0;
		double highest = 0.0;
		double constant = 0.0;
		double mean = 0.0;
		double variance = 0.0;
		double lambda = 0.0;
		double mode = 0.0;
		double minimun = 0.0;
		double maximun = 0.0;
		double shapeAlpha = 0.0;
		double shapeBeta = 0.0;
		double shape = 0.0;
		double escale = 0.0;
		 
		 String probabilistics = null;
		 String[] numers = null;
		
		//esta escrita pensada en la menor sobrecarga de comprobaciones instanceof
		Node nextNode = this.graphic.getOnlyOneSelected();
		
		if (nextNode == null) return;
		
		switch (params.length){
			case 3:
				if(nextNode instanceof Counter){
					quantity = this.toInteger(params[0]);
					cycle = this.toInteger(params[1]);
					this.graphic.editCounter(nextNode, quantity , cycle, (String)params[2]);
				}
			break;
			case 4:
				if(nextNode instanceof Queue){
					resource = this.toInteger(params[0]);
					fixedCost = this.toDouble(params[1]);
					variableCost = this.toDouble(params[2]);
					this.graphic.editQueue( nextNode, resource, fixedCost, variableCost, (String)params[3]);
				}
			break;
			case 5:
				if(nextNode instanceof Function){
					input = this.toInteger(params[0]);
					output = this.toInteger(params[1]);
					probBranch = this.toBoolean(params[2]);
					probabilistics = (String)params[4];
					if(probabilistics != null &&  probabilistics.trim().isEmpty() == false){
						numers = probabilistics.split(",");
						if(numers.length % 2 == 0){
							List<Integer> idNodes = getIdNodes(numers);
							List<Double> probabilities = getProbabilities(numers);
							this.graphic.editFunction( nextNode, input, output, probBranch, (String)params[3]);
							this.graphic.editArrows(nextNode, probBranch, idNodes, probabilities);
							break;	
						}
					}
					System.err.println("probabilistic malformed");
				}
				break;
			case 18:
				if(nextNode instanceof Normal){
					distribution = (String)params[0];
					seed = this.toInteger(params[1]);
					least = this.toDouble(params[2]);
					highest = this.toDouble(params[3]);
					constant = this.toDouble(params[4]);
					mean = this.toDouble(params[5]);
					variance = this.toDouble(params[6]);
					lambda = this.toDouble(params[7]);
					mode = this.toDouble(params[8]);
					minimun = this.toDouble(params[9]);
					maximun = this.toDouble(params[10]);
					shapeAlpha = this.toDouble(params[11]);
					shapeBeta = this.toDouble(params[12]);
					shape = this.toDouble(params[13]);
					escale = this.toDouble(params[14]);
					probBranch = this.toBoolean(params[15]);
					
					probabilistics = (String)params[17];
					if(probabilistics != null &&  probabilistics.trim().isEmpty() == false){
						numers = probabilistics.split(",");
						if(numers.length % 2 == 0){
							List<Integer> idNodes = getIdNodes(numers);
							List<Double> probabilities = getProbabilities(numers);
							this.graphic.editNormal(nextNode, distribution, seed, least, highest, constant,
									mean, variance, lambda, mode, minimun, maximun, shapeAlpha,
									shapeBeta, shape, escale, probBranch,(String)params[16]);
							
							this.graphic.editArrows(nextNode, probBranch, idNodes, probabilities);	
						}
						break;
					}
					System.err.println("probabilistic malformed");
					break;
				}
				if(nextNode instanceof Combi){
					distribution = (String)params[0];
					seed = this.toInteger(params[1]);
					least = this.toDouble(params[2]);
					highest = this.toDouble(params[3]);
					constant = this.toDouble(params[4]);
					mean = this.toDouble(params[5]);
					variance = this.toDouble(params[6]);
					lambda = this.toDouble(params[7]);
					mode = this.toDouble(params[8]);
					minimun = this.toDouble(params[9]);
					maximun = this.toDouble(params[10]);
					shapeAlpha = this.toDouble(params[11]);
					shapeBeta = this.toDouble(params[12]);
					shape = this.toDouble(params[13]);
					escale = this.toDouble(params[14]);
					probBranch = this.toBoolean(params[15]);
					
					probabilistics = (String)params[17];
					if(probabilistics != null &&  probabilistics.trim().isEmpty() == false){
						numers = probabilistics.split(",");
						if(numers.length % 2 == 0){
							List<Integer> idNodes = getIdNodes(numers);
							List<Double> probabilities = getProbabilities(numers);
							this.graphic.editCombi(nextNode, distribution, seed, least, highest, constant,
									mean, variance, lambda, mode, minimun, maximun, shapeAlpha,
									shapeBeta, shape, escale, probBranch,(String)params[16]);
							
							this.graphic.editArrows(nextNode, probBranch, idNodes, probabilities);	
						}
						break;
					}
					System.err.println("probabilistic malformed");
				}
			break;
			default : break;
		}
	}
	
	/** 
	 * get data of model
	 * */
	public void getModelInfo(Object[] params){
		if(params.length == 2){
			int length = toInteger(params[0]);
			int modelSeed = toInteger(params[1]);
			this.graphic.getModelInfo(this.communicator, length, modelSeed);
		}
	}
	
	/** 
	 * get all data of model to save
	 * */
	public void getAllModelFile(Object[] params){
		if(params.length == 2){
			int length = toInteger(params[0]);
			int modelSeed = toInteger(params[1]);
			this.graphic.getAllModelFile(this.communicator, length, modelSeed);
		}
	}
	
	/** 
	 * set all data of model to open
	 * */
	public void setAllModelFile(Object[] params){
		Map<Integer, Node> nodeMap = new HashMap<Integer, Node>();
		if(params.length == 1){
			String oldModelJson = (String)params[0];
			
			JSONObject model = new JSONObject(oldModelJson);
			
			JSONObject transformation = model.getJSONObject(NodeFields.TRANSFORMATION);
			JSONArray arrows = model.getJSONArray(NodeFields.ARROWS);
			
			JSONArray queues = transformation.getJSONArray(NodeFields.QUEUES);
			JSONArray normals = transformation.getJSONArray(NodeFields.NORMALS);
			JSONArray combis = transformation.getJSONArray(NodeFields.COMBIS);
			JSONArray counters = transformation.getJSONArray(NodeFields.COUNTERS);
			JSONArray functions = transformation.getJSONArray(NodeFields.FUNCTIONS);
			
			for(int i = 0; i < queues.length(); i++){
				JSONObject queue = queues.getJSONObject(i);
				Node queueNode = NodeFactory.createCompleteNode(NodeTypes.QUEUE,
						queue.getInt(NodeFields.POS_X), queue.getInt(NodeFields.POS_Y),
						queue.getString(NodeFields.NAME), false, null, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0,
						queue.getInt(NodeFields.RESOURCE),
						queue.getDouble(NodeFields.FIXED_COST),
						queue.getDouble(NodeFields.VARIABLE_COST), 0, 0);
				nodeMap.put(queue.getInt(NodeFields.ID_NODE), queueNode);
			}
			
			for(int i = 0; i < counters.length(); i++){
				JSONObject counter = queues.getJSONObject(i);
				Node counterNode = NodeFactory.createCompleteNode(NodeTypes.COUNTER,
						counter.getInt(NodeFields.POS_X), counter.getInt(NodeFields.POS_Y),
						counter.getString(NodeFields.NAME),
						false, null, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
						counter.getInt(NodeFields.QUANTITY), counter.getInt(NodeFields.CYCLE), 0, 0.0, 0.0, 0, 0);
				nodeMap.put(counter.getInt(NodeFields.ID_NODE), counterNode);
			}
			
			for(int i = 0; i < functions.length(); i++){
				JSONObject function = queues.getJSONObject(i);
				Node functionNode = NodeFactory.createCompleteNode(NodeTypes.FUNCTION,
						function.getInt(NodeFields.POS_X), function.getInt(NodeFields.POS_Y),
						function.getString(NodeFields.NAME), 
						function.getBoolean(NodeFields.PROBAB_BRANCH),
						null, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0.0, 0.0,
						function.getInt(NodeFields.INPUT), function.getInt(NodeFields.OUT_PUT));
				nodeMap.put(function.getInt(NodeFields.ID_NODE), functionNode);
			}
			
			for(int i = 0; i < normals.length(); i++){
				JSONObject normal = queues.getJSONObject(i);
				Node normalNode = NodeFactory.createCompleteNode(NodeTypes.NORMAL,
						normal.getInt(NodeFields.POS_X), normal.getInt(NodeFields.POS_Y),
						normal.getString(NodeFields.NAME), 
						normal.getBoolean(NodeFields.PROBAB_BRANCH),
						normal.getString(NodeFields.DISTRIBUTION),
						normal.getInt(NodeFields.SEED),
						normal.getDouble(NodeFields.LEAST),
						normal.getDouble(NodeFields.HIGHEST),
						normal.getDouble(NodeFields.CONSTANT),
						normal.getDouble(NodeFields.MEAN),
						normal.getDouble(NodeFields.VARIANCE),
						normal.getDouble(NodeFields.LAMBDA),
						normal.getDouble(NodeFields.MODE),
						normal.getDouble(NodeFields.MINIMUN),
						normal.getDouble(NodeFields.MAXIMUN),
						normal.getDouble(NodeFields.SHAPE_ALPHA),
						normal.getDouble(NodeFields.SHAPE_BETA),
						normal.getDouble(NodeFields.SHAPE),
						normal.getDouble(NodeFields.ESCALE),
						0, 0, 0, 0.0, 0.0, 0, 0);
				nodeMap.put(normal.getInt(NodeFields.ID_NODE), normalNode);
			}
			
			for(int i = 0; i < combis.length(); i++){
				JSONObject combi = queues.getJSONObject(i);
				Node combiNode = NodeFactory.createCompleteNode(NodeTypes.COMBI,
						combi.getInt(NodeFields.POS_X), combi.getInt(NodeFields.POS_Y),
						combi.getString(NodeFields.NAME), 
						combi.getBoolean(NodeFields.PROBAB_BRANCH),
						combi.getString(NodeFields.DISTRIBUTION),
						combi.getInt(NodeFields.SEED),
						combi.getDouble(NodeFields.LEAST),
						combi.getDouble(NodeFields.HIGHEST),
						combi.getDouble(NodeFields.CONSTANT),
						combi.getDouble(NodeFields.MEAN),
						combi.getDouble(NodeFields.VARIANCE),
						combi.getDouble(NodeFields.LAMBDA),
						combi.getDouble(NodeFields.MODE),
						combi.getDouble(NodeFields.MINIMUN),
						combi.getDouble(NodeFields.MAXIMUN),
						combi.getDouble(NodeFields.SHAPE_ALPHA),
						combi.getDouble(NodeFields.SHAPE_BETA),
						combi.getDouble(NodeFields.SHAPE),
						combi.getDouble(NodeFields.ESCALE),
						0, 0, 0, 0.0, 0.0, 0, 0);
				nodeMap.put(combi.getInt(NodeFields.ID_NODE), combiNode);
			}
			
			int max = nodeMap.size();
			for(int i = 0; i< max;i++){
				Node current = nodeMap.get(new Integer(i+1));
				this.graphic.appendNode(i, current);
			}
			
			//TODO proces json
			
			for(int i = 0; i < arrows.length(); i++){
				JSONObject arrow = queues.getJSONObject(i);
				JSONArray middlePoints = arrow.getJSONArray("middlePoints");
				List<Node> dotList = new ArrayList<Node>();
				for(int j = 0; j < middlePoints.length(); j++){
					JSONObject point = middlePoints.getJSONObject(i);
					int order = point.getInt("order");
					int posX = point.getInt(NodeFields.POS_X);
					int posY = point.getInt(NodeFields.POS_Y);
					Node dot = NodeFactory.createNode(NodeTypes.ARROW_DOT, posX, posY, null, order);
					dotList.add(dot);
				}
				
				int tail = arrow.getInt("tail");
				int head = arrow.getInt("head");
				
				BindSurface tailSurface = toBindSurface(arrow.getInt("tailSurface"));
				BindSurface headSurface = toBindSurface(arrow.getInt("headSurface"));
				
				boolean enableProb = arrow.getBoolean("enableProb");
				double probabilisticBranch = arrow.getDouble("probabilisticBranch");
				//TODO insert arrows
				
			}
			
		}
		
	}

	
	///********************Parcers***************************************/
	private List<Integer> getIdNodes(String[] numers){
		List<Integer> list = new ArrayList<Integer>();
		for (int i = 0; i < numers.length/2 ; i++)
			list.add(toInteger(numers[i*2]));
		return list;
	}
	
	private List <Double> getProbabilities(String[] numers){
		List<Double> list = new ArrayList<Double>();
		for (int i = 0; i < numers.length/2 ; i++)
			list.add(toDouble(numers[i*2+1]));
		return list;
	}
	
	
	private int toInteger(Object value){
		int integerValue = 0;
		if(value != null){
			try {  
				integerValue = Integer.parseInt((String)value);  
			} catch(NumberFormatException nfe)  {
				System.err.println("No se puede parcear a entero...."+value.toString());
			} 
		}
		return integerValue;
	}
	
	private BindSurface toBindSurface(int val){
		switch(val){
			case 1: return BindSurface.NORTH_EAST;
			case 2: return BindSurface.EAST;
			case 3: return BindSurface.SOUTH_EAST;
			case 4: return BindSurface.SOUTH;
			case 5: return BindSurface.SOUTH_WEST;
			case 6: return BindSurface.WEST;
			case 7: return BindSurface.NORTH_WEST;
			case 8: return BindSurface.CENTER;
			case 9: return BindSurface.NORTH;
			default:break;
		}
		System.err.println("Bad bind surface");
		return BindSurface.CENTER;
	}	
	
	
	private double toDouble(Object value){
		double doubleValue = 0;
		if(value != null){
			try{  
				doubleValue = Double.parseDouble((String)value);
			} catch(NumberFormatException nfe){
				System.err.println("No se puede parcear a double...."+value.toString());
			}	
		}
		return doubleValue;
	}
	
	private boolean toBoolean(Object value){
		boolean booleanValue = false;
		if(value != null){
			booleanValue = ((String)value).equalsIgnoreCase("yes"); 
		}
		return booleanValue;
	}
	
}
