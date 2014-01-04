package ar.com.botqueue.applet.graphic.node;

import java.awt.Color;
import java.awt.Graphics;

import ar.com.botqueue.applet.enums.BindSurface;

/**
 * Define un Un cubo donde habra un simbolo y debajo tendra un mensaje
 * La clase que extiende de NodoGenerico y deba extender la funcionalidad
 * de los metodos de nodo deberá poner el override.
 * @author redes3
 *
 */
public class GenericNode implements Node, Comparable<Node>{
	//van a ser usadas por subclases
	protected int posX;
	protected int posY;
	protected final int width ;
	protected final int height ;
	protected String label;
	protected final int fontSize;
	protected Color currentColor;
	
	protected int order;
	//auxiliares para eventos
	private boolean isSelected;
	//diferencia entre el puntero y la selección
	private int offsetX;
	private int offsetY;
	private boolean isDragged; //esta siendo arrastrado
	
	/**
	 * 
	 * @param posX vertice arriba a la izquierda
	 * @param posY vertice arriba a la izquierda
	 * @param label debajo del cubo
	 * @param width del cubo
	 * @param height del cubo
	 */
	public GenericNode(int posX, int posY, String label, int width, int height, int order) {
		this.posX = posX;
		this.posY = posY;
		this.label = label;
		this.width = width;
		this.height = height;
		this.order = order;
		this.currentColor = Color.BLACK;
		this.isDragged = false;
		this.fontSize = 12;
		this.isSelected = false;
		
	}
	
	public int getOrder(){
		return this.order;
	}

	public void setOrder(int order) {
		this.order = order;
		
	}

	public int compareTo(Node theAnotherInstance) {
		return this.order - theAnotherInstance.getOrder();
	}
	
	public boolean isSelected() {
		return this.isSelected;
	}

	public boolean isDragged() {
		return this.isDragged;
	}
	
	public boolean hasHit(int x, int y, double zoom) {
		if(x > this.posX +(int)(this.width * zoom)) return false;
		if(x < this.posX) return false;
		if(y > this.posY +(int)(this.height * zoom)) return false;
		if(y < this.posY) return false;
		return true;
	}

	public boolean isHitOnMove(int x, int y, double zoom, int fromX, int toX,int fromY, int toY ) {
		if(!this.isDragged){
			return false;
		} else{
			int nextX = x - this.offsetX; 
			int nextY = y - this.offsetY;
			if( nextX < fromX){
				return true;
				}
			if( nextY < fromY){
				return true;
			}
			
			if( nextX + (int)(this.width * zoom) > toX){
				return true;
			}
			if( nextY + (int)(this.height * zoom) > toY){
				return true;	
			}

			return false;	
		}
	}
	
	public void select() {
		this.isSelected = true;
		this.currentColor = Color.RED;
	}

	public void unselect() {
		this.isSelected = false;
		this.currentColor = Color.BLACK;
	}
	
	public void dragged(int x, int y ) {
		if(!this.isDragged){
			this.offsetX = x - this.posX;
			this.offsetY = y - this.posY;
			this.isDragged = true;
			this.isSelected = true;
			this.currentColor = Color.RED;
			return;
		} else{
			this.posX = x - this.offsetX;
			this.posY = y - this.offsetY;
			return;	
		}
	}

	public void dropped() {
		this.isDragged = false;
	}

	public void movedByArrowKeys(int movX, int movY) {
			this.posX += movX;
			this.posY += movY;
			return;	
	}
	
	public boolean hasHitByArrowKeys(int movX, int movY, double zoom, int fromX, int toX,int fromY, int toY ){
		int nextX = movX + this.posX; 
		int nextY = movY + this.posY;
		
		if( nextX < fromX) return true;
		if( nextY < fromY) return true;
		
		if( nextX + (int)(this.width * zoom) > toX) return true;
		if( nextY +(int)(this.height * zoom) > toY) return true;

		return false;
	}
	
	/**
	 * se overrrideara si o si
	 */
	public void paint(Graphics g, double zoom) {
		return;	
	}
	
	public int getXBindSurfacePoint(double zoom, BindSurface pto){
		switch(pto){
			case NORTH_WEST: return this.posX;
			case NORTH: return this.posX + (int)(this.width * zoom)/2;
			case NORTH_EAST: return this.posX + (int)(this.width * zoom);
			case EAST : return this.posX + (int)(this.width * zoom);
			case SOUTH_EAST: return this.posX + (int)(this.width * zoom);
			case SOUTH: return this.posX + (int)(this.width * zoom)/2;
			case SOUTH_WEST: return this.posX;
			case WEST: return this.posX;
			case CENTER:
			default: return this.posX + (int)(this.width * zoom)/2;
		}
	}
	
	public int getYBindSurfacePoint(double zoom, BindSurface pto){
		switch(pto){
			case NORTH_WEST: return this.posY;
			case NORTH: return this.posY;
			case NORTH_EAST: return this.posY;
			case EAST : return this.posY + (int)(this.height * zoom)/2;
			case SOUTH_EAST: return this.posY + (int)(this.height * zoom);
			case SOUTH: return this.posY + (int)(this.height * zoom);
			case SOUTH_WEST: return this.posY + (int)(this.height * zoom);
			case WEST: return this.posY + (int)(this.height * zoom)/2;
			case CENTER:
			default: return this.posY + (int)(this.height * zoom)/2;
		}
	}

	public String getLabel(){
		return this.label;
	}
	
	public void setLabel(String label){
		this.label = label;
	}
}
