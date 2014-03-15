package ar.com.botqueue.applet.graphic;

import java.awt.Graphics;
import java.util.ArrayList;
import java.util.List;

import ar.com.botqueue.applet.Principal;
import ar.com.botqueue.applet.enums.BindSurface;
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
	
	@Deprecated
	public void testFastExample(){
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
	    amazar.select();    
	}
	
	/**
	 * Llama a pintar cada elemento del grafo
	 * */
	public void paint(Graphics g, double zoom){
		for(Node node: nodes){
			node.paint(g, zoom);
		}
		
		for(GenericArrow ga: edges){
			ga.paint(g, zoom);
		}
	}
	
	/**
	 * Crea un nodo en el grafo
	 * */
	public boolean createNode(NodeTypes nodeType, String label){
		Node nextNode = NodeFactory.createNode(nodeType, 600, 600, label);
		if(nextNode != null){
			this.nodes.add(nextNode);
			return true;
		}
		return false;
	}
	
	/**
	 * Elimina un nodo del grafo
	 * */
	public boolean deleteNode(){
		Node nextNode = getOnlyOneSelected();
		if (nextNode == null) return false;
		nodes.remove(nextNode);
		return true;
	}
	
	/** 
	 * Crea una flecha a partir de dos nodos seleccionados
	 * @param zoom
	 * @return
	 */
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
		 * invierte una flecha seleccionada
		 * @return improved
		 */
		public boolean invertArrow(){
			GenericArrow ga = getArrow();
			if(ga != null){
				ga.invertArrow();
				ga.setProbabilisticBranch(ga.getProbabilisticBranch(), ga.isEnableProb());
				return true;
			}
			return false;
		}
		
		/** 
		 * elimina una flecha del grafo
		 * @return
		 */
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
		
		/**
		 *  rota una cola de flecha
		 * @return
		 */
		public boolean rotateHead(){
			GenericArrow ga = getArrow();
			if(ga != null){
				ga.headRotate();
				return true;
			}
			return false;
		}
		
		/** 
		 * rota la cola de flecha
		 * @return
		 */
		public boolean rotateTail(){
			GenericArrow ga = getArrow();
			if(ga != null){
				ga.tailRotate();
				return true;
			}
			return false;
		}
		
		/** 
		 * elimina un punto del modelo
		 * @return
		 */
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
		
		/** 
		 * crea el siguiente punto
		 * @param zoom
		 * @return
		 */
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
		
		/** 
		 * crea  punto anterior al punto
		 * @param zoom
		 * @return
		 */
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
		
		/////********************setters y getters del modelo*********************************************
		/** 
		 * edita las probabilidades de las flechas que parten de un nodo
		 * @param nextNode
		 * @param probBranch
		 * @param idNodes
		 * @param probabilities
		 */
		public void editArrows(Node nextNode, boolean probBranch,
			List<Integer> idNodes, List<Double> probabilities) {
			List<GenericArrow> nodeEdges = getEdges(nextNode);
			for(GenericArrow edge: nodeEdges){
				if(probBranch){
					
					int currentIdNode = this.nodes.indexOf(edge.getHeadArrow()) +1;
					for (Integer idNode: idNodes){
						if(idNode == currentIdNode){
							edge.setProbabilisticBranch( probabilities.get(idNodes.indexOf(idNode)), true);
						}
					}
				} else {
					edge.setProbabilisticBranch(0.0, false);
				}
			}
		}
		
		/** 
		 * setea una cola
		 * @param nextNode
		 * @param quantity
		 * @param cycle
		 * @param label
		 */
		public void editCounter(Node nextNode, int quantity, int cycle, String label){
			((Counter)nextNode).editCounter(quantity, cycle, label);
		}
		
		/** 
		 * setea una cola
		 * @param nextNode
		 * @param resource
		 * @param fixedCost
		 * @param variableCost
		 * @param label
		 */
		public void editQueue(Node nextNode, int resource, double fixedCost, double variableCost, String label){
			((Queue)nextNode).editQueue(resource,  fixedCost,  variableCost,  label);
		}
		
		/** 
		 * edita una funcion
		 * @param nextNode
		 * @param input
		 * @param output
		 * @param probBranch
		 * @param label
		 */
		public void editFunction(Node nextNode, int input, int output, boolean probBranch, String label){
			((Function)nextNode).editFunction(input, output, probBranch, label);
		}
		
		/** 
		 * edita nua normal
		 * @param nextNode
		 * @param distribution
		 * @param seed
		 * @param least
		 * @param highest
		 * @param constant
		 * @param mean
		 * @param variance
		 * @param lambda
		 * @param mode
		 * @param minimun
		 * @param maximun
		 * @param shapeAlpha
		 * @param shapeBeta
		 * @param shape
		 * @param escale
		 * @param probBranch
		 * @param label
		 */
		public void editNormal(Node nextNode, String distribution, int seed, double least, double highest, double constant, double mean, double variance, double lambda, double mode, double minimun, double maximun, double shapeAlpha, double shapeBeta, double shape, double escale, boolean probBranch, String label){
			((Normal)nextNode).editNormal(distribution,  seed,  least,  highest,  constant,  mean,  variance,  lambda,  mode,  minimun,  maximun,  shapeAlpha,  shapeBeta,  shape,  escale,  probBranch,  label);
		}
		
		/** 
		 * edita nua normal
		 * @param nextNode
		 * @param distribution
		 * @param seed
		 * @param least
		 * @param highest
		 * @param constant
		 * @param mean
		 * @param variance
		 * @param lambda
		 * @param mode
		 * @param minimun
		 * @param maximun
		 * @param shapeAlpha
		 * @param shapeBeta
		 * @param shape
		 * @param escale
		 * @param probBranch
		 * @param label
		 */
		public void editCombi(Node nextNode, String distribution, int seed, double least, double highest, double constant, double mean, double variance, double lambda, double mode, double minimun, double maximun, double shapeAlpha, double shapeBeta, double shape, double escale, boolean probBranch, String label){
			((Combi)nextNode).editCombi(distribution,  seed,  least,  highest,  constant,  mean,  variance,  lambda,  mode,  minimun,  maximun,  shapeAlpha,  shapeBeta,  shape,  escale,  probBranch,  label);
		}
		
		/** 
		 * obtiene informacion de un nodo para el formulario de vaadin
		 * @param destination
		 * @return
		 */
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
		
		/** 
		 * obtiene el json de todo el modelo para la entrada
		 * @param destination
		 * @param length
		 * @param seedModel
		 * @return
		 */
		public String getModelInfo(Principal destination,int length, int seedModel){
			//clasifico
			String queues = "\""+NodeFields.QUEUES+"\":[  ";
			String combis = "\""+NodeFields.COMBIS+"\":[  ";
			String normals = "\""+NodeFields.NORMALS+"\":[  ";
			String functions = "\""+NodeFields.FUNCTIONS+"\":[ ";
			String counters = "\""+NodeFields.COUNTERS+"\":[  ";
			for(Node nextNode:this.nodes){
				if(nextNode instanceof Queue)
					queues += getNodeJson(nextNode)+",";
				if(nextNode instanceof Normal)
					normals += getNodeJson(nextNode)+",";
				if(nextNode instanceof Combi)
					combis += getNodeJson(nextNode)+",";
				if(nextNode instanceof Function)
					functions += getNodeJson(nextNode)+",";
				if(nextNode instanceof Counter)
					counters += getNodeJson(nextNode)+",";
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
			
			//FIXME: ESto es para que el usuario no tenga que hacer click en guardar modelo en construccion y dame el pending
			String modelTotal = getAuxAllModelFile( destination, length, seedModel);
			model = modelTotal + "***" + model;
			destination.vaadinUpdateVariable("editModel", model, true);
			return model;
			
		}
		
		/** 
		 * obtiene el json de todo el modelo para retomar el trabajo luego
		 * @param destination
		 * @param length
		 * @param seedModel
		 * @return
		 */
		public String getAuxAllModelFile(Principal destination,int length, int seedModel){
			String queues = "\""+NodeFields.QUEUES+"\":[  ";
			String combis = "\""+NodeFields.COMBIS+"\":[  ";
			String normals = "\""+NodeFields.NORMALS+"\":[  ";
			String functions = "\""+NodeFields.FUNCTIONS+"\":[ ";
			String counters = "\""+NodeFields.COUNTERS+"\":[  ";
			for(Node nextNode:this.nodes){
				String jsonArrayElement = getNodeSaveFileJson(nextNode)+",";
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
					queues+","+combis+","+normals+","+functions+","+counters+"}"+
					", \"arrows\": [";
			model +=getArrowsSaveFileJson();
			
			model += "] }";
			return model;
		}
		
		public String getAllModelFile(Principal destination,int length, int seedModel){
			String modelTotal = getAuxAllModelFile( destination, length, seedModel);
			destination.vaadinUpdateVariable("editModelConstruct", modelTotal, true);
			return modelTotal;
			
		}
		
		/** 
		 * obtiene el json para guardar de cada una de las flechas
		 * @return
		 */
		private String getArrowsSaveFileJson() {
			String jsonEdges = "";
			for(GenericArrow ga: this.edges){
				jsonEdges += ga.getArrowJson(this.nodes)+" ,";
			}
			jsonEdges = jsonEdges.substring(0, jsonEdges.length() - 1);
			return jsonEdges;
		}
		
		/** 
		 * obtiene el json de construccion para guardar de un nodo
		 * @param nextNode
		 * @return
		 */
		private String getNodeSaveFileJson(Node nextNode) {
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
			return nextNode.getJsonSaveFile(idNode, preceders, followers, probabilisticBranch);
		}
		
	/////**********************json helper********************************************
		/**
		 *  listo idnode y nombre de nodo en un array json
		 * @param nextNode
		 * @return
		 */
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
		
		/** 
		 * tranforma el modelo en un json para el modelo precursor
		 * @param nextNode
		 * @return
		 */
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
					//System.out.println("prob: "+edge.getProbabilisticBranch()); 
					probabilisticBranch.add(edge.getProbabilisticBranch());
					followers.add( this.nodes.indexOf(edge.getHeadArrow()) +1);

				}
			}
			return nextNode.getJson(idNode, preceders, followers, probabilisticBranch);
		}
		
	////***************************helpers******************************************
		/** 
		 * retorna las flachas que salen del nodo
		 * @param nextNode
		 * @return lista de flechas
		 */
		private List<GenericArrow> getEdges(Node nextNode) {
			List<GenericArrow> ret = new ArrayList<GenericArrow>();
			
			for(GenericArrow edge: this.edges){
				
				if( edge.getTailArrow().equals(nextNode) ){
					ret.add(edge);
				}
			}
			return ret;
		}
		
		/**
		 * Retorna un nodo si y solo si
		 * hay solo uno seleccionado y
		 *  es no es nodo punto de una flecha 
		 * @return nodo seleccionado o null
		 */
		public Node getOnlyOneSelected(){
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
		
		/** 
		 * retorna una  flacha identificandola por los dos nodos que enlaza
		 * @return 1 flecha
		 */
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
		
		/** 
		 * obtiene flecha identificandola por un punto que le pertenece
		 * @return
		 */
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
	
		/**
		 * Mete de un modelo ya hecho un nodo
		 */
		public void appendNode(int index, Node newNode){
			this.nodes.add(index, newNode);
		}
		
		/**
		 * retorna lista de nodos
		 */
		public List<Node> getModelNode(){
			return this.nodes;
		}
		
		/** 
		 * mete de un modelo ya hecho una flecha
		 */
		public void appendArrow(GenericArrow e ){
			this.edges.add(e);
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
	
}
