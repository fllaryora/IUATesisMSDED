package ar.com.botqueue.applet.graphic;

import java.awt.Graphics;
import java.io.ObjectOutputStream.PutField;
import java.util.ArrayList;
import java.util.List;

import ar.com.botqueue.applet.Principal;
import ar.com.botqueue.applet.enums.NodeFields;
import ar.com.botqueue.applet.enums.NodeTypes;
import ar.com.botqueue.applet.graphic.arrow.GenericArrow;
import ar.com.botqueue.applet.graphic.node.Combi;
import ar.com.botqueue.applet.graphic.node.Counter;
import ar.com.botqueue.applet.graphic.node.DotNode;
import ar.com.botqueue.applet.graphic.node.Function;
import ar.com.botqueue.applet.graphic.node.Node;
import ar.com.botqueue.applet.graphic.node.NodeFactory;
import ar.com.botqueue.applet.graphic.node.Normal;
import ar.com.botqueue.applet.graphic.node.Queue;

public class GraphicDTO {
	
	private List<Node> nodes;
	
	private List<GenericArrow> edges;
	
	public GraphicDTO(){
		this.nodes = new ArrayList<Node>();
		edges = new ArrayList<GenericArrow>();
		
	}
	
	public void paint(Graphics g, double zoom){
		for(Node node: nodes){
			node.paint(g, zoom);
		}
		
		for(GenericArrow ga: edges){
			ga.paint(g, zoom);
		}
	}
	
	public boolean createNode(NodeTypes nodeType, String label){
		Node nextNode = NodeFactory.createNode(nodeType, 600, 600, label);
		if(nextNode != null){
			this.nodes.add(nextNode);
			return true;
		}
		return false;
	}
	
	/**
	 * Retorna un nodo si y solo si
	 * hay solo uno seleccionado y
	 *  es no es nodo punto de una flecha 
	 * @return nodo seleccionado o null
	 */
	private Node getOnlyOneSelected(){
		Node nextNode = null;
		boolean flag = false;
		for(Node node: nodes){
			if( node.isSelected() ){
				if(flag == false){
					nextNode = node;
					flag = true;
				}else {
					return null; //hay 2 seleccionados
				}
			}
		}
		if(nextNode == null) return null;
		if(nextNode instanceof DotNode) return null;
		return nextNode;
	}
	
	public boolean deleteNode(){
		Node nextNode = getOnlyOneSelected();
		if (nextNode == null) return false;
		nodes.remove(nextNode);
		return true;
	}
	
	
	public boolean renameNode(String newLabel){
		Node nextNode = getOnlyOneSelected();
		if (nextNode == null) return false;
		nextNode.setLabel(newLabel);
		return true;
	}
	
	//************************************ movimientos************************
	/**
	 * Comportamiento de seleccionador común de iconos:
	 * -Cuando el usuario hace click afuera (sin colicionar con ningun icono), sin el ctrl apretado:
	 * se deseleccionan todos los iconos de haber alguno seleccionado.
	 * -Cuando el usuario hace click afuera (sin colicionar con ningun icono), con el ctrl apretado:
	 * se hace como si nada hubiera pasado.
	 * -Cuando el usuario hace click sobre unicono, con el ctrl apretado:
	 * Se agrega o se quita de la lista de iconos seleccionados.
	 * -Cuando el usuario hace click sobre unicono, sin el ctrl apretado:
	 * Se quita de la lista todos los iconos y solo se pone este.
	 * El problema es que si el usuario mueve el mouse antes de soltar el click significa que
	 *  lo que en realidad queria, hera mover los iconos seleccionados en masa.
	 *  Para eso esta el resolvedorDeConflicto
	 * **/
	private final int APAGADO = 0;
	private final int PRENDIDO = 1;
	private final int CALIENTE = 2;
	
	private int resolvedorDeConflicto = APAGADO;
			
	public void click(int mouseX, int mouseY, boolean controlPressed , double zoom){

		boolean clickEnLaNada = true;
		for(Node node: nodes){
			boolean coliciona =  node.hasHit( mouseX, mouseY, zoom) ;
			clickEnLaNada &= !coliciona;
			if(coliciona) break;
		}
		if(clickEnLaNada){ 
			if(!controlPressed){
				for(Node node: nodes){
					node.unselect();
				}
			}
			return;
		}
		if(controlPressed){
			for(Node node: nodes){
				if( node.hasHit( mouseX, mouseY, zoom) ){
					if( node.isSelected() ){
						node.unselect();
					} else {
						node.select();
					}
					break;
				}
			}
		} else {
			// se pone heavy
			resolvedorDeConflicto = PRENDIDO;
			for(Node node: nodes){
				if( node.hasHit( mouseX, mouseY, zoom) ){
					node.select();
					break;
				}
			}
		}
	}
	
	public boolean drag(int mouseX, int mouseY , double zoom, int xPodioMin, int xPodioMax, int yPodioMin, int yPodioMax){
		// parte nueva
		if(resolvedorDeConflicto == PRENDIDO){
			resolvedorDeConflicto = CALIENTE;
		}
		//fin parte nueva
				
		boolean noChocaConAlgo = true;	
		
		for(Node node: nodes){
			if( node.isSelected() ){
				if( node.isHitOnMove(mouseX, mouseY, zoom, xPodioMin, xPodioMax, yPodioMin, yPodioMax ) ){
					
					noChocaConAlgo = false;
				}
			}
		}
		
		if(noChocaConAlgo){	
			for(Node node: nodes){
				if( node.isSelected()){
					node.dragged( mouseX, mouseY );
				}
			}
		}
		return noChocaConAlgo;
			
	}
	
	public void drop(int mouseX, int mouseY , double zoom){
		for(Node node: nodes){
			if(node.isDragged() ){
				node.dropped();
			}
		}
			
		if(resolvedorDeConflicto == PRENDIDO){
			for(Node node: nodes){
				node.unselect();
			}
			for(Node node: nodes){
				if( node.hasHit( mouseX, mouseY, zoom) ){
					node.select();
					break;
				}
			}
		}
		resolvedorDeConflicto = APAGADO;
	}
	
	public boolean dragByKey(int horizontal, int vertical , double zoom, int xPodioMin, int xPodioMax, int yPodioMin, int yPodioMax){
		
		boolean noChocaConAlgo = true;	
		//algun nodo choca contra la pared
		for(Node node: nodes){
			if( node.isSelected()){
				if( node.hasHitByArrowKeys(horizontal, vertical, zoom, xPodioMin, xPodioMax, yPodioMin, yPodioMax) ){
					noChocaConAlgo = false;
				}
			}
		}
		
		if(noChocaConAlgo){	
			for(Node node: nodes){
				if( node.isSelected() ){
					node.movedByArrowKeys(horizontal, vertical);
				}
			}
		}
		return noChocaConAlgo;
	}
	
	
	//Crea una flecha a partir de dos nodos seleccionados
	public boolean createArrow(double zoom){
		Node nodeA = null;
		Node nodeB = null;
		boolean tooManySelected = false;
		for(Node node: nodes){
			if( node.isSelected() ){
				if(nodeA == null){
					nodeA = node;
				} else {
					if(nodeB == null){
						nodeB = node;
					} else {
						tooManySelected = true;
					}
					
				}
			}
		}
		if (tooManySelected){
			return false;
		}
		
		if(nodeA == null || nodeB == null) return false;
		
		if(nodeA instanceof DotNode || nodeA instanceof DotNode){
			return false;
		}
		GenericArrow ga= new GenericArrow(nodes , nodeA, nodeB, zoom);
		edges.add(ga);
		ga.setProbabilisticBranch(0.0, nodeA.isProbBranch());
		return true;
		
	}
	
	/**
	 * retorna  una  flecha, 
	 * mediante la seleccion de un punto de flecha
	 *  o 2 nodos que tengan una flecha en comun
	 */
	private GenericArrow getArrow(){
		GenericArrow ga = getArrowByTwoNodes();
		if(ga == null)
			ga = getArrowByDotArrow();
		return ga;
	}
	
	
	private GenericArrow getArrowByTwoNodes(){
		Node nodeA = null;
		Node nodeB = null;
		boolean tooManySelected = false;
		for(Node node: nodes){
			if( node.isSelected() ){
				if(nodeA == null){
					nodeA = node;
				} else {
					if(nodeB == null){
						nodeB = node;
					} else {
						tooManySelected = true;
					}
					
				}
			}
		}
		if (tooManySelected){
			return null;
		}
		
		if(nodeA == null || nodeB == null) return null;
		
		if(nodeA instanceof DotNode || nodeA instanceof DotNode){
			return null;
		}
		
		Node nodeX = null, nodeY= null;
		for(GenericArrow ga : this.edges){
			nodeX = ga.getTailArrow();
			nodeY = ga.getHeadArrow();
			if(( nodeX == nodeA && nodeY == nodeB) || ( nodeY == nodeA && nodeX == nodeB))
				return ga;
		}
		//no existe tal flecha entre esos dos puntos
		return null;
	}
	
	private GenericArrow getArrowByDotArrow(){
		Node dotNode = null;
		boolean tooManySelected = false;
		for(Node node: nodes){
			if( node.isSelected() ){
				if(tooManySelected == false){
					dotNode = node;
					tooManySelected = true;
				}else {
					return null; //hay 2 seleccionados
				}
			}
		}
		if(dotNode == null) return null;
		if(dotNode instanceof DotNode){
			
			for(GenericArrow ga : this.edges){
				if( ga.isOwnDot(dotNode) ){
					return ga; 
				}
			}
		}
		//no era un nodo de flecha
		return null;
	}
	
	public boolean invertArrow(){
		GenericArrow ga = getArrow();
		if(ga != null){
			ga.invertArrow();
			ga.setProbabilisticBranch(ga.getProbabilisticBranch(), ga.isEnableProb());
			return true;
		}
		return false;
	}
	
	public boolean deleteArrow(){
		GenericArrow ga = getArrow();
		if(ga != null){
			//limpia la lista general de nodos
			//limpia su propia lista de puntos
			ga.deleteArrow();
			this.edges.remove(ga);
			return true;
		}
		return false;
	}
	
	public boolean rotateHead(){
		GenericArrow ga = getArrow();
		if(ga != null){
			ga.headRotate();
			return true;
		}
		return false;
	}
	
	public boolean rotateTail(){
		GenericArrow ga = getArrow();
		if(ga != null){
			ga.tailRotate();
			return true;
		}
		return false;
	}
	
	public boolean deleteDot(){
		Node dotNode = null;
		boolean tooManySelected = false;
		for(Node node: nodes){
			if( node.isSelected() ){
				if(tooManySelected == false){
					dotNode = node;
					tooManySelected = true;
				}else {
					return false; //hay 2 seleccionados
				}
			}
		}
		if(dotNode == null) return false;
		if(dotNode instanceof DotNode){
			for(GenericArrow ga : this.edges){
				if(ga.isOwnDot(dotNode)){
					//la saca de la flecha y del modelo
					ga.quitDotArrow(dotNode);
					ga.setProbabilisticBranch(ga.getProbabilisticBranch(), ga.isEnableProb());
					return true;
				}
			}
		}
		//no era nodo punto
		return false;
	}
	
	public boolean createNextDot(double zoom){
		Node dotNode = null;
		boolean tooManySelected = false;
		for(Node node: nodes){
			if( node.isSelected() ){
				if(tooManySelected == false){
					dotNode = node;
					tooManySelected = true;
				}else {
					return false; //hay 2 seleccionados
				}
			}
		}
		if(dotNode == null) return false;
		if(dotNode instanceof DotNode){
			for(GenericArrow ga : this.edges){
				if(ga.isOwnDot(dotNode)){
					//la saca de la flecha y del modelo
					ga.addDotNextTo(dotNode, zoom);
					ga.setProbabilisticBranch(ga.getProbabilisticBranch(), ga.isEnableProb());
					return true;
				}
			}
		}
		//no era nodo punto
		return false;
	}
	
	public boolean createPreviusDot(double zoom){
		Node dotNode = null;
		boolean tooManySelected = false;
		for(Node node: nodes){
			if( node.isSelected() ){
				if(tooManySelected == false){
					dotNode = node;
					tooManySelected = true;
				}else {
					return false; //hay 2 seleccionados
				}
			}
		}
		if(dotNode == null) return false;
		if(dotNode instanceof DotNode){
			for(GenericArrow ga : this.edges){
				if(ga.isOwnDot(dotNode)){
					//la saca de la flecha y del modelo
					ga.addDotBeforeFrom(dotNode, zoom);
					ga.setProbabilisticBranch(ga.getProbabilisticBranch(), ga.isEnableProb());
					return true;
				}
			}
		}
		//no era nodo punto
		return false;
	}
	
	public void getModelInfo(Principal destination,Object[] params){	
		getModelInfo(destination,gi(params[0]),gi(params[1]));
	}
	

	public String getModelInfo(Principal destination,int length, int seedModel){
		//clasifico
		String queues = "\""+NodeFields.QUEUES+"\":[  ";
		String combis = "\""+NodeFields.COMBIS+"\":[  ";
		String normals = "\""+NodeFields.NORMALS+"\":[  ";
		String functions = "\""+NodeFields.FUNCTIONS+"\":[ ";
		String counters = "\""+NodeFields.COUNTERS+"\":[  ";
		for(Node nextNode:this.nodes){
			String jsonArrayElement = getNodeJson(nextNode)+",";
			if(nextNode instanceof Queue)
				queues += jsonArrayElement;
			if(nextNode instanceof Normal)
				normals += jsonArrayElement;
			if(nextNode instanceof Combi)
				combis += jsonArrayElement;
			if(nextNode instanceof Function)
				functions += jsonArrayElement;
			if(nextNode instanceof Counter)
				counters += jsonArrayElement;
		}
		queues = queues.substring(0, queues.length() - 1)+" ]";
		combis = combis.substring(0, combis.length() - 1)+" ]";
		normals = normals.substring(0, normals.length() - 1)+" ]";
		functions = functions.substring(0, functions.length() - 1)+" ]";
		counters = counters.substring(0, counters.length() - 1)+" ]";
		
		
		String model = "{ \"length\":"+ length +
				", \"seed\":"+seedModel+
				", \"transformation\": {"+
				queues+","+combis+","+normals+","+functions+","+counters+"} }";
		destination.vaadinUpdateVariable("editModel", model, true);
		return model;
		
	}
	
	public String getModelConstructInfo(Principal destination, Object[] p){
		return getModelConstructInfo( destination, gi(p[0]), gi(p[1]));
	}
	public String getModelConstructInfo(Principal destination,int length, int seedModel){
		//clasifico
		String queues = "\""+NodeFields.QUEUES+"\":[  ";
		String combis = "\""+NodeFields.COMBIS+"\":[  ";
		String normals = "\""+NodeFields.NORMALS+"\":[  ";
		String functions = "\""+NodeFields.FUNCTIONS+"\":[ ";
		String counters = "\""+NodeFields.COUNTERS+"\":[  ";
		for(Node nextNode:this.nodes){
			String jsonArrayElement = getNodeConstructJson(nextNode)+",";
			if(nextNode instanceof Queue)
				queues += jsonArrayElement;
			if(nextNode instanceof Normal)
				normals += jsonArrayElement;
			if(nextNode instanceof Combi)
				combis += jsonArrayElement;
			if(nextNode instanceof Function)
				functions += jsonArrayElement;
			if(nextNode instanceof Counter)
				counters += jsonArrayElement;
		}
		queues = queues.substring(0, queues.length() - 1)+" ]";
		combis = combis.substring(0, combis.length() - 1)+" ]";
		normals = normals.substring(0, normals.length() - 1)+" ]";
		functions = functions.substring(0, functions.length() - 1)+" ]";
		counters = counters.substring(0, counters.length() - 1)+" ]";
		
		
		String model = "{ \"length\":"+ length +
				", \"seed\":"+seedModel+
				", \"transformation\": {"+
				queues+","+combis+","+normals+","+functions+","+counters+"} }";
		destination.vaadinUpdateVariable("editModelConstruct", model, true);
		return model;
		
	}

	private String getNameList(Node nextNode){
		List<GenericArrow> nodeEdges = this.getEdges(nextNode);
		String json2 = "{ \"nameList\":[  ";
		for(GenericArrow currentEdge : nodeEdges){
			int idNode = this.nodes.indexOf(currentEdge.getHeadArrow()) +1;
			String name = currentEdge.getHeadArrow().getLabel();
			json2 += "{\"id\" : "+idNode+", \"name\": \""+name+"\"} ,";
			
		}
		json2 = json2.substring(0, json2.length()-1);
		json2 += " ] }";
		return json2;
	} 
	
	public String getNodeInfo(Principal destination){
		Node nextNode = getOnlyOneSelected();
		if (nextNode == null)
			return "{}";
		String json = getNodeJson(nextNode);
		if(nextNode instanceof Queue)
			destination.vaadinUpdateVariable("editQueue", json, true);
		if(nextNode instanceof Normal){
			json = getNameList(nextNode)+ "***"+json;
			destination.vaadinUpdateVariable("editNormal", json, true);
		}
			
		if(nextNode instanceof Combi){
			json = getNameList(nextNode)+ "***"+json;
			destination.vaadinUpdateVariable("editCombi", json, true);
		}
		if(nextNode instanceof Function){
			json = getNameList(nextNode)+ "***"+json;
			destination.vaadinUpdateVariable("editFunction", json, true);
		}
		
		if(nextNode instanceof Counter)
			destination.vaadinUpdateVariable("editCounter", json, true);
		
		return json; 
	}

	private String getNodeJson(Node nextNode) {
		int idNode = this.nodes.indexOf(nextNode)+1;
		List<Integer> preceders = new ArrayList<Integer>();
		List<Integer> followers = new ArrayList<Integer>();
		List<Double> probabilisticBranch = new ArrayList<Double>();
		for(GenericArrow edge: this.edges){
			if( edge.getHeadArrow().equals(nextNode) ){
				preceders.add( this.nodes.indexOf(edge.getTailArrow()) +1);				
			}
			if( edge.getTailArrow().equals(nextNode) ){
				probabilisticBranch.add(edge.getProbabilisticBranch());
				followers.add( this.nodes.indexOf(edge.getHeadArrow()) +1);

			}
		}
		return nextNode.getJson(idNode, preceders, followers, probabilisticBranch);
	}
	
	
	private String getNodeConstructJson(Node nextNode) {
		//TODO puntos y prob del las flechas
		int idNode = this.nodes.indexOf(nextNode)+1;
		List<Integer> preceders = new ArrayList<Integer>();
		List<Integer> followers = new ArrayList<Integer>();
		List<Double> probabilisticBranch = new ArrayList<Double>();
		for(GenericArrow edge: this.edges){
			if( edge.getHeadArrow().equals(nextNode) ){
				preceders.add( this.nodes.indexOf(edge.getTailArrow()) +1);				
			}
			if( edge.getTailArrow().equals(nextNode) ){
				probabilisticBranch.add(edge.getProbabilisticBranch());
				followers.add( this.nodes.indexOf(edge.getHeadArrow()) +1);

			}
		}
		return nextNode.getJsonConstruct(idNode, preceders, followers, probabilisticBranch);
	}
	
	private List<GenericArrow> getEdges(Node nextNode) {
		List<GenericArrow> ret = new ArrayList<GenericArrow>();
		
		for(GenericArrow edge: this.edges){
			
			if( edge.getTailArrow().equals(nextNode) ){
				ret.add(edge);
			}
		}
		return ret;
	}
	
	public void editNode(Object[] p){
		String[] numers = null;
		//(String)params[0]), (String)params[1]
		String probabilistics;
		Node nextNode = getOnlyOneSelected();
		if (nextNode == null)
			return;
		if(nextNode instanceof Queue)
			//resource, fixedCost, variableCost
			((Queue)nextNode).editQueue(gi(p[0]), gd(p[1]), gd(p[2]),(String)p[3]);
		if(nextNode instanceof Normal){
			//distribution, seed, least, highest, constant, mean, variance, lambda, mode, minimun, maximun, shapeAlpha, shapeBeta, shape, escale, probBranch
			 probabilistics = (String)p[17];
			 if(probabilistics != null &&  probabilistics.trim().isEmpty() == false)
				 numers = probabilistics.split(",");
			((Normal)nextNode).editNormal( (String)p[0], gi(p[1]), gd(p[2]), gd(p[3]), gd(p[4]),gd(p[5]), gd(p[6]), gd(p[7]), gd(p[8]), gd(p[9]), gd(p[10]),gd(p[11]), gd(p[12]), gd(p[13]), gd(p[14]), gb(p[15]),(String)p[16]);
			List<GenericArrow> nodeEdges = getEdges(nextNode);
			for(GenericArrow edge: nodeEdges){
				if(gb(p[15])){
					for (int i = 0; i < numers.length/2 ; i++){
						int idNode = gi(numers[i*2]);
						int currentIdNode = this.nodes.indexOf(edge.getTailArrow()) +1;
						if(idNode == currentIdNode)
							edge.setProbabilisticBranch(gd(numers[i*2+1]), true);
					}
				}
				else
					edge.setProbabilisticBranch(0.0, false);
			}
			
			}
		if(nextNode instanceof Combi){
			 probabilistics = (String)p[17];
			 if(probabilistics != null &&  probabilistics.trim().isEmpty() == false)
				 numers = probabilistics.split(",");
			//distribution, seed, least, highest, constant, mean, variance, lambda, mode, minimun, maximun, shapeAlpha, shapeBeta, shape, escale, probBranch
			((Combi)nextNode).editCombi((String)p[0], gi(p[1]), gd(p[2]), gd(p[3]), gd(p[4]),gd(p[5]), gd(p[6]), gd(p[7]), gd(p[8]), gd(p[9]), gd(p[10]),gd(p[11]), gd(p[12]), gd(p[13]), gd(p[14]), gb(p[15]),(String)p[16]);
			List<GenericArrow> nodeEdges = getEdges(nextNode);
			for(GenericArrow edge: nodeEdges){
				if(gb(p[15])){
					for (int i = 0; i < numers.length/2 ; i++){
						int idNode = gi(numers[i*2]);
						int currentIdNode = this.nodes.indexOf(edge.getTailArrow()) +1;
						if(idNode == currentIdNode)
							edge.setProbabilisticBranch(gd(numers[i*2+1]), true);
					}
				}
				else
					edge.setProbabilisticBranch(0.0, false);
			}
		}

		if(nextNode instanceof Function){
			 probabilistics = (String)p[4];
			 if(probabilistics != null &&  probabilistics.trim().isEmpty() == false)
				 numers = probabilistics.split(",");
			//input, output, probBranch
			((Function)nextNode).editFunction(gi(p[0]), gi(p[1]), gb(p[2]),(String)p[3]);
			List<GenericArrow> nodeEdges = getEdges(nextNode);
			for(GenericArrow edge: nodeEdges){
				if(gb(p[2])){
					for (int i = 0; i < numers.length/2 ; i++){
						int idNode = gi(numers[i*2]);
						int currentIdNode = this.nodes.indexOf(edge.getTailArrow()) +1;
						if(idNode == currentIdNode)
							edge.setProbabilisticBranch(gd(numers[i*2+1]), true);
					}
				}
				else
					edge.setProbabilisticBranch(0.0, false);
			}
		}

		if(nextNode instanceof Counter)
			//quantity, cycle
			((Counter)nextNode).editCounter(gi(p[0]),gi(p[1]),(String)p[2]);
		return ;
		
	}
	
	private int gi(Object value){
     try  
     {  
         return Integer.parseInt((String)value);  
         
      } catch(NumberFormatException nfe)  
      {  
          return 0;  
      }  
		
	}
	
	private double gd(Object value){
	     try  
	     {  
	         return Double.parseDouble((String)value);  
	         
	      } catch(NumberFormatException nfe)  
	      {  
	          return 0.0;  
	      }  
			
	}
	
	private boolean gb(Object value){
		if( ((String)value).equalsIgnoreCase("yes")) return true;
		else return false;
	}
	

	public void testFastExample(Principal destination){
		Node panadero = NodeFactory.createNode(NodeTypes.QUEUE, 70+40, 20+10, "Panadero");
	    Node amazar = NodeFactory.createNode(NodeTypes.COMBI, 70+26, 20+112, "Amazar");
	    Node cocinar = NodeFactory.createNode(NodeTypes.NORMAL, 70+170, 20+112, "Cocinar");
	    Node cortar = NodeFactory.createNode(NodeTypes.FUNCTION, 70+314, 20+112, "Cortar");
	    Node servir = NodeFactory.createNode(NodeTypes.COUNTER, 70+458, 20+112, "Servir"); 
	    
	    nodes.add(servir);
	    nodes.add(cortar);
	    nodes.add(cocinar);
	    nodes.add( amazar );
	    nodes.add(panadero);
	    
	   
	    GenericArrow toAmazar = new GenericArrow(nodes,panadero,amazar,1.0);
	    toAmazar.headRotate();
	    toAmazar.tailRotate();toAmazar.tailRotate();toAmazar.tailRotate();
	    GenericArrow toPanadero = new GenericArrow(nodes,amazar,panadero,1.0);
	    toPanadero.headRotate();toPanadero.headRotate();toPanadero.headRotate();toPanadero.headRotate();toPanadero.headRotate();
	    toPanadero.tailRotate();toPanadero.tailRotate();toPanadero.tailRotate();toPanadero.tailRotate();toPanadero.tailRotate();toPanadero.tailRotate();toPanadero.tailRotate();
	    
	    GenericArrow toCocinar = new GenericArrow(nodes,amazar,cocinar,1.0);
	    GenericArrow toCortar = new GenericArrow(nodes,cocinar,cortar,1.0);
	    GenericArrow toServir = new GenericArrow(nodes,cortar,servir,1.0);
	    
	    
	    
	    edges.add( toServir );
	    edges.add( toCortar );
	    edges.add( toCocinar );
	    edges.add( toAmazar );
	    edges.add( toPanadero );
	    
	    System.out.println( this.getModelInfo(destination, 600 , -1) );
	  }
	
}
