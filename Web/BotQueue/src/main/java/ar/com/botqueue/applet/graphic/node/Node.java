package ar.com.botqueue.applet.graphic.node;

import java.awt.Graphics;
import java.util.List;

import ar.com.botqueue.applet.enums.BindSurface;

/**
 * Representa una interfaz de nodo
 * con todos los estados por los que pasa. 
 */

public interface Node{
	//set del numero de orden de un punto de flecha
	public int getOrder();

	public void setOrder(int order);
	
	//probar seleccion
	public boolean isSelected();
	
	//probar arrastre
	public boolean isDragged();
	
	//se pregunta si el punto coliciona sobre la figura del icono
	public boolean hasHit(int x, int y, double zoom);
	
	//se pregunta si el punto coliciona sobre la figura del icono
	public boolean isHitOnMove(int x, int y, double zoom, int fromX, int toX,int fromY, int toY );
	
	//se clickio o una parte de una flecha le esta pasando por encima
	public void select();
	
	//se clickio afuera o una parte de una flecha dejo de pasarle por encima
	public void unselect();
	
	//empeso a arrastrarse o se esta arrastrando
	public void dragged(int x, int y);
		
	//dejo de ser arrastrado
	public void dropped();
	
	//se movio los objetos con las flechitas
	public boolean hasHitByArrowKeys(int movX, int movY, double zoom, int fromX, int toX,int fromY, int toY );
	
	public void movedByArrowKeys(int movX, int movY);
	
	public void paint(Graphics g, double zoom);
	
	public int getXBindSurfacePoint(double zoom, BindSurface pto);
	public int getYBindSurfacePoint(double zoom, BindSurface pto);
	
	public String getLabel();
	public void setLabel(String label);
	
	public boolean isProbBranch();
	
	public String getJson(int id, List<Integer> preceders, List<Integer> followers ,  List<Double> probabilisticBranch);
	public String getJsonConstruct(int idNode, List<Integer> preceders, List<Integer> followers,  List<Double> probabilisticBranch); 
}
