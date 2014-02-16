package ar.com.botqueue.applet.graphic.arrow;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import ar.com.botqueue.applet.enums.BindSurface;
import ar.com.botqueue.applet.enums.NodeTypes;
import ar.com.botqueue.applet.graphic.node.DotNode;
import ar.com.botqueue.applet.graphic.node.Node;
import ar.com.botqueue.applet.graphic.node.NodeFactory;

public class GenericArrow implements Arrow{
	//los puntos en la flecha
	private List<DotNode> arrowNodes;
	private List<Node> handleredNodes;
	
	//the begin dot
	private Node tailArrow; 
	//the last dot
	private Node headArrow;
	
	private Color currentColor;
	private BindSurface tailSurface;
	private BindSurface headSurface;
	
	private double probabilisticBranch; 
	private boolean enableProb;
	
	//constantes
	private final double phi = Math.toRadians(20);
	private final int widthArrow = 10;
	
	public GenericArrow( List<Node> modelNodes,Node tail,Node head, List<DotNode> arrowNodes, boolean enableProb, double probabilisticBranch, BindSurface tailSurface, BindSurface headSurface){
		this.handleredNodes = modelNodes;
		this.enableProb = enableProb;
		this.probabilisticBranch = probabilisticBranch;
		this.arrowNodes = arrowNodes;
		this.tailArrow = tail;
		this.tailSurface = tailSurface;
		this.headArrow = head;
		this.headSurface = headSurface;
		modelNodes.addAll(arrowNodes);
		Collections.sort(this.arrowNodes);
		this.setProbabilisticBranch(this.probabilisticBranch, this.enableProb);
	}
	
	public GenericArrow(List<Node> modelNodes, Node tail,Node head, double zoom){
		this.handleredNodes = modelNodes;
		this.arrowNodes = new ArrayList<DotNode>();
		this.probabilisticBranch = 0.0;
		enableProb = false;
		this.currentColor = Color.BLACK;
		
		this.tailArrow = tail;
		this.tailSurface = BindSurface.EAST;
		
		this.headArrow = head;
		this.headSurface = BindSurface.WEST;
		
		int x = (this.headArrow.getXBindSurfacePoint(zoom, this.headSurface) + this.tailArrow.getXBindSurfacePoint(zoom, this.tailSurface))/2 ;
		int y = (this.headArrow.getYBindSurfacePoint(zoom, this.headSurface) + this.tailArrow.getYBindSurfacePoint(zoom, this.tailSurface))/2 ;
		Node e = NodeFactory.createNode(NodeTypes.ARROW_DOT, x, y, "", 0);
		this.arrowNodes.add((DotNode)e);
		this.handleredNodes.add(e);
	}
	
	public void deleteArrow() {
		this.tailArrow = null;
		this.tailSurface = BindSurface.CENTER;
		for(Node node: arrowNodes){
			this.handleredNodes.remove(node);
		}
		arrowNodes.clear();
	}
	
	public void invertArrow(){
		Node aux = this.tailArrow;
		this.tailArrow = this.headArrow;
		this.headArrow = aux;
	}

	public void quitDotArrow(Node dot) {
		//al menos un punto intermedio
		if(arrowNodes.size() > 1){
			arrowNodes.remove(dot);
			this.handleredNodes.remove(dot);
		}
	}
	
	public void tailRotate() {
		switch(this.tailSurface){
			case NORTH:  this.tailSurface = BindSurface.NORTH_EAST;
			break;
			case NORTH_EAST:  this.tailSurface = BindSurface.EAST;
			break;
			case EAST:  this.tailSurface = BindSurface.SOUTH_EAST;
			break;
			case SOUTH_EAST:  this.tailSurface = BindSurface.SOUTH;
			break;
			case SOUTH:  this.tailSurface = BindSurface.SOUTH_WEST;
			break;
			case SOUTH_WEST:  this.tailSurface = BindSurface.WEST;
			break;
			case WEST:  this.tailSurface = BindSurface.NORTH_WEST;
			break;
			case NORTH_WEST:  this.tailSurface = BindSurface.NORTH;
			break;
			case CENTER:   this.tailSurface = BindSurface.CENTER;
			break;
		}
		
	}

	public void headRotate() {
		switch(this.headSurface){
		case NORTH:  this.headSurface = BindSurface.NORTH_EAST;
		break;
		case NORTH_EAST:  this.headSurface = BindSurface.EAST;
		break;
		case EAST:  this.headSurface = BindSurface.SOUTH_EAST;
		break;
		case SOUTH_EAST:  this.headSurface = BindSurface.SOUTH;
		break;
		case SOUTH:  this.headSurface = BindSurface.SOUTH_WEST;
		break;
		case SOUTH_WEST:  this.headSurface = BindSurface.WEST;
		break;
		case WEST:  this.headSurface = BindSurface.NORTH_WEST;
		break;
		case NORTH_WEST:  this.headSurface = BindSurface.NORTH;
		break;
		case CENTER:   this.headSurface = BindSurface.CENTER;
		break;
	}
	}

	public Node addDotNextTo(Node thisDotInArrow, double zoom) {
		
		int xInitial =0, yInitial=0; //inicializo al pedo para evitar warnings
		int xFinal = thisDotInArrow.getXBindSurfacePoint(zoom, BindSurface.CENTER);
		int yFinal = thisDotInArrow.getYBindSurfacePoint(zoom, BindSurface.CENTER);
		int currentOrder = thisDotInArrow.getOrder();
		int size = this.arrowNodes.size();
		
		if( currentOrder + 1 == size){//es el ultimo punto
			//debo usar el punto extremo?
			xInitial = this.headArrow.getXBindSurfacePoint(zoom, this.headSurface) ;
			yInitial = this.headArrow.getYBindSurfacePoint(zoom, this.headSurface) ;
		} else{
			for(Node nodo: arrowNodes){
				if (nodo.getOrder() == thisDotInArrow.getOrder() + 1){
					xInitial = nodo.getXBindSurfacePoint(zoom, BindSurface.CENTER);
					yInitial = nodo.getYBindSurfacePoint(zoom, BindSurface.CENTER);
					break;
				}
			}
		}
		
		int x = (xFinal + xInitial)/2;
		int y = (yFinal + yInitial)/2;;
		Node e = NodeFactory.createNode(NodeTypes.ARROW_DOT, x, y, "", currentOrder+1);
		for(Node nodo: arrowNodes){
			if (nodo.getOrder() > thisDotInArrow.getOrder()){
				nodo.setOrder(nodo.getOrder() +1);
			}
		}
		arrowNodes.add((DotNode)e);
		this.handleredNodes.add(e);
		Collections.sort(arrowNodes);
		return e;
	}

	public Node addDotBeforeFrom(Node thisDotInArrow, double zoom){
		int xInicial =0, yInicial=0;
		int xFinal = thisDotInArrow.getXBindSurfacePoint(zoom, BindSurface.CENTER);
		int yFinal = thisDotInArrow.getYBindSurfacePoint(zoom, BindSurface.CENTER);
		int currentOrder = thisDotInArrow.getOrder();
		
		if( currentOrder == 0){//debo usar el punto extremo?
			xInicial = this.tailArrow.getXBindSurfacePoint(zoom, this.tailSurface) ;
			yInicial = this.tailArrow.getYBindSurfacePoint(zoom, this.tailSurface) ;
		} else{
			for(Node nodo: arrowNodes){
				if (nodo.getOrder() == thisDotInArrow.getOrder() - 1){
					xInicial = nodo.getXBindSurfacePoint(zoom, BindSurface.CENTER);
					yInicial = nodo.getYBindSurfacePoint(zoom, BindSurface.CENTER);
					break;
				}
			}
			
		}
		
		int x = (xFinal + xInicial)/2;
		int y = (yFinal + yInicial)/2;;
		Node e = NodeFactory.createNode(NodeTypes.ARROW_DOT, x, y, "", currentOrder-1);
		for(Node nodo: arrowNodes){
			if (nodo.getOrder() < thisDotInArrow.getOrder()){
				nodo.setOrder(nodo.getOrder() -1);
			}
		}
		arrowNodes.add((DotNode)e);
		this.handleredNodes.add(e);
		Collections.sort(arrowNodes);
		return e;

	}
	
	public void paint(Graphics g, double zoom){
		int x1, x2, y1, y2; 
		g.setColor(this.currentColor);
		
		x1 = this.tailArrow.getXBindSurfacePoint(zoom, tailSurface);
		y1 = this.tailArrow.getYBindSurfacePoint(zoom, tailSurface);
		Graphics2D g2 =(Graphics2D) g;
		for(Node nodo: arrowNodes){
			nodo.paint(g, zoom);
			x2 = nodo.getXBindSurfacePoint(zoom, BindSurface.CENTER);
			y2 = nodo.getYBindSurfacePoint(zoom, BindSurface.CENTER);
			g.setColor(this.currentColor);
			g2.draw(new Line2D.Double(x1, y1, x2, y2));
			x1 = x2;
			y1 = y2;
		}
		
		x2 = this.headArrow.getXBindSurfacePoint(zoom, headSurface);
		y2 = this.headArrow.getYBindSurfacePoint(zoom, headSurface);
		
		g2.draw(new Line2D.Double(x1, y1, x2, y2));
		
		paintHeadArrow(g, zoom, x1, y1, x2, y2);
		
	}

	private void paintHeadArrow(Graphics g, double zoom, int x1, int y1, int x2, int y2) {
		double magnitud = (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
		double x3, y3, x4, y4;
		double dx, dy;
		double theta, rho;
		magnitud = Math.sqrt(magnitud);
		if(magnitud > widthArrow * zoom * 2.0 ){
			dx = x2 - x1;
			dy = y2 - y1;
			theta = Math.atan2(dy, dx);
			rho = theta + this.phi;
			x3 = x2 - this.widthArrow * zoom * Math.cos(rho);
			y3 = y2 - this.widthArrow * zoom * Math.sin(rho);
			rho = theta - this.phi;
			x4 = x2 - this.widthArrow * zoom * Math.cos(rho);
			y4 = y2 - this.widthArrow * zoom * Math.sin(rho);
			Graphics2D g2 =(Graphics2D) g;
			g2.draw(new Line2D.Double(x2, y2, x3, y3));
			g2.draw(new Line2D.Double(x2, y2, x4, y4));
		}
	}

	public Node getTailArrow() {
		return tailArrow;
	}

	public Node getHeadArrow() {
		return headArrow;
	}
	
	
	public boolean isOwnDot(Node externalNode){
		for(Node node: arrowNodes){
			if(externalNode.equals(node)){
				return true;
			}
		}
		return false;
	}

	public double getProbabilisticBranch() {
		return probabilisticBranch;
	}

	public void setProbabilisticBranch(double probabilisticBranch, boolean enable) {
		this.probabilisticBranch = probabilisticBranch;
		this.enableProb = enable;
		DotNode point = arrowNodes.get(0);
		if(this.enableProb){
			point.enableProbabilisticBranch(""+this.probabilisticBranch);
		} else {
			point.disableProbabilisticBranch();
		}
		
	}

	public boolean isEnableProb() {
		return enableProb;
	}
	
	/////////////*******************obtener json****************************************
	public String getArrowJson( List<Node> nodes) {
		String arrowJson = "{";
		
		int tail = nodes.indexOf(this.getTailArrow())+1;
		int head = nodes.indexOf(this.getHeadArrow())+1;
		
		arrowJson += putValue("tail",tail)+",";
		arrowJson += putValue("head",head)+",";
		
		System.out.println("tail "+ this.tailSurface +putValue("tailSurface",this.tailSurface));
		System.out.println("head "+ this.headSurface +putValue("headSurface",this.headSurface));
		
		arrowJson += putValue("tailSurface",this.tailSurface)+",";
		arrowJson += putValue("headSurface",this.headSurface)+",";
		arrowJson += putValue("enableProb",this.enableProb)+",";
		arrowJson += putValue("probabilisticBranch",this.probabilisticBranch)+",";
		arrowJson += "\"middlePoints\": [";
		for(DotNode dotN : this.arrowNodes){
			arrowJson += dotN.getJsonSaveFile() + " ,";
		}
		arrowJson = arrowJson.substring(0, arrowJson.length() - 1);
		arrowJson += "]}";
		return arrowJson;
	}
	
	protected String putValue(String field, int value){
		return "\""+field+"\":"+value;
	}
	
	protected String putValue(String field, boolean value){
		return "\""+field+"\":"+(value?"true":"false");
	}
	
	protected String putValue(String field, double value){
		return "\""+field+"\":"+value;
	} 
	
	protected String putValue(String field, BindSurface value){
		int val = 0;
		switch(value){
			case NORTH: val = 9;  
			break;
			case NORTH_EAST:  val = 1;
			break;
			case EAST:  val = 2;
			break;
			case SOUTH_EAST:  val = 3;
			break;
			case SOUTH:  val = 4;
			break;
			case SOUTH_WEST:  val = 5;
			break;
			case WEST:  val = 6;
			break;
			case NORTH_WEST:  val = 7;
			break;
			case CENTER:   val = 8;
			break;
		}
		return "\""+field+"\":"+val;
	}
	
	
	protected String putString(String field, String value){
		return "\""+field+"\":"+"\""+value+"\"";
	}
		
	
}
