package ar.com.botqueue.applet.graphic.arrow;

import java.awt.Graphics;

import ar.com.botqueue.applet.graphic.node.Node;

/**
 * Representa una interfaz de flecha
 */
public interface Arrow {
	public void paint(Graphics g, double zoom);
	public void invertArrow();
	public void deleteArrow();
	public void quitDotArrow(Node dot);
	
	public void tailRotate();
	public void headRotate();
	
	public Node addDotBeforeFrom(Node thisDotInArrow, double zoom);
	public Node addDotNextTo(Node thisDotInArrow, double zoom);
		
}
