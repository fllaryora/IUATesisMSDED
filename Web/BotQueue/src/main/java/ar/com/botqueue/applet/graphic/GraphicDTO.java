package ar.com.botqueue.applet.graphic;

import java.awt.Graphics;
import java.util.ArrayList;
import java.util.List;

import ar.com.botqueue.applet.enums.NodeTypes;
import ar.com.botqueue.applet.graphic.arrow.GenericArrow;
import ar.com.botqueue.applet.graphic.node.DotNode;
import ar.com.botqueue.applet.graphic.node.Node;
import ar.com.botqueue.applet.graphic.node.NodeFactory;

public class GraphicDTO {
	
	private List<Node> nodes;
	
	private List<GenericArrow> edges;
	
	public GraphicDTO(){
		this.nodes = new ArrayList<Node>();
		edges = new ArrayList<GenericArrow>();
		
	}
	
	public void runExample(){
		///*****provisorio
		Node a = NodeFactory.createNode(NodeTypes.COUNTER, 60, 60, "Contador");
		Node b = NodeFactory.createNode(NodeTypes.NORMAL, 180, 60, "Normal");
		nodes.add(a);
		nodes.add(b);
		nodes.add( NodeFactory.createNode(NodeTypes.QUEUE, 60, 180, "Cola") );
		nodes.add( NodeFactory.createNode(NodeTypes.FUNCTION, 60, 180+60, "Funcion") );
		nodes.add( NodeFactory.createNode(NodeTypes.COMBI, 180, 180, "Combi") );
		
		GenericArrow fg = new GenericArrow(nodes,a,b,1.0);
		edges.add(fg);
		
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
	
	
	public String getNameNode(){
		Node nextNode = getOnlyOneSelected();
		if (nextNode == null) return null;
		return nextNode.getLabel();
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
		edges.add(new GenericArrow(nodes , nodeA, nodeB, zoom));
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
					return true;
				}
			}
		}
		//no era nodo punto
		return false;
	}
	
	//TODO modelo Precursor
	public void setProperties(){}
	
}
