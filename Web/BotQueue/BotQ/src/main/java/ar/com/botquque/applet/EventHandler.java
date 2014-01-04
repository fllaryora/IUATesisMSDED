package ar.com.botquque.applet;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import javax.swing.JPanel;
import ar.com.botquque.applet.graphic.GraphicDTO;


public class EventHandler  extends JPanel implements MouseListener, MouseMotionListener, KeyListener {
	private static final long serialVersionUID = 7L;
	Principal fatherApplet;
	GraphicDTO graphic;
	private double zoom ;
	
	//dimensiones del podio de los nodos
	private int xPodioMin = 0;
	private int xPodioMax = this.getWidth();
	private int yPodioMin = 0;
	private int yPodioMax = this.getHeight();
	
	private boolean controlPressed;
	
	EventHandler(Principal caller,GraphicDTO graphic){
		this.graphic = graphic;
		this.fatherApplet =  caller;
		this.fatherApplet.addMouseListener(this);
		this.fatherApplet.addMouseMotionListener(this);
		this.fatherApplet.addKeyListener(this);
		this.setDoubleBuffered(true);
		//no se puede escribir sobre algo que no tiene foco.
		this.setFocusable(true);
		this.zoom = 1.0;
		this.controlPressed = false;		
	}
	
	
	@Override
	public void paintComponent(Graphics g){
		super.paintComponents(g);
		this.xPodioMin = 0;
		this.xPodioMax = this.getWidth();
		this.yPodioMin = 0;
		this.yPodioMax = this.getHeight();
		
		Graphics2D g2 = (Graphics2D)g;
		RenderingHints rh = new RenderingHints(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		rh.add(new RenderingHints(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY));
		rh.add(new RenderingHints(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON));
		rh.add(new RenderingHints(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_NORMALIZE));
		rh.add(new RenderingHints(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BICUBIC));
		rh.add(new RenderingHints(RenderingHints.KEY_ALPHA_INTERPOLATION, RenderingHints.VALUE_ALPHA_INTERPOLATION_QUALITY));
		rh.add(new RenderingHints(RenderingHints.KEY_DITHERING, RenderingHints.VALUE_DITHER_ENABLE));
		rh.add(new RenderingHints(RenderingHints.KEY_COLOR_RENDERING, RenderingHints.VALUE_COLOR_RENDER_QUALITY));
		g2.setRenderingHints(rh);
		
		this.graphic.paint(g,this.zoom);
	
	}
		
	/** 
	 * el mouse se apreto sin soltar
	 */
	public void mousePressed(MouseEvent e) {
		this.graphic.click(e.getX(), e.getY(), this.controlPressed, this.zoom);
	}

	/**
	 *  el mouse se mueve apretando
	 */
	public void mouseDragged(MouseEvent e) {
		if(this.graphic.drag(e.getX(), e.getY(), this.zoom, this.xPodioMin, this.xPodioMax, this.yPodioMin, this.yPodioMax)){
			this.fatherApplet.repaint();
			this.repaint();	
		}
		e.consume();
	}
	
	/** 
	 * el mouse se acaba de soltar
	 */
	public void mouseReleased(MouseEvent e) {
		this.graphic.drop(e.getX(), e.getY(), this.zoom);
		this.fatherApplet.repaint();
		this.repaint();
		e.consume();

	}
	
	public void keyTyped(KeyEvent e) {
		this.controlPressed = e.isControlDown();
		e.consume();
	}
	
	public void keyPressed(KeyEvent e) {
		boolean change = false;
		if(e.getKeyCode() == KeyEvent.VK_DOWN){
			change = this.graphic.dragByKey (0,1, this.zoom, this.xPodioMin, this.xPodioMax, this.yPodioMin, this.yPodioMax);
		} 
		if(e.getKeyCode() == KeyEvent.VK_UP){
			change = this.graphic.dragByKey (0, -1, this.zoom, this.xPodioMin, this.xPodioMax, this.yPodioMin, this.yPodioMax);
		}
		if(e.getKeyCode() == KeyEvent.VK_LEFT){
			change = this.graphic.dragByKey ( -1, 0, this.zoom, this.xPodioMin, this.xPodioMax, this.yPodioMin, this.yPodioMax);
		}
		if(e.getKeyCode() == KeyEvent.VK_RIGHT){
			change = this.graphic.dragByKey ( 1, 0, this.zoom, this.xPodioMin, this.xPodioMax, this.yPodioMin, this.yPodioMax);
		}
		this.controlPressed = e.isControlDown();
		if(change){
			this.fatherApplet.repaint();
			this.repaint();
		}
		e.consume();
	}
	
	public void keyReleased(KeyEvent e) {
		this.controlPressed = e.isControlDown();
		e.consume();
		
	}
	
	//*******************Eventos que no uso pero quer debo implementar+++++++++++
	/**
	 * El mouse entro en el area del applet
	 */
	public void mouseEntered(MouseEvent e) {
		e.consume();
	}
	
	/**
	 * El mouse salio de el area del applet
	 * no se usa
	 */
	public void mouseExited(MouseEvent e) {
		e.consume();
	}

	/** 
	 * el mouse se mueve sin apretar
	 * no se usa
	 */
	public void mouseMoved(MouseEvent e) {
		e.consume();
	}
	/** 
	 * el mouse se pulso y solto en el lugar "rapido".
	 */
	public void mouseClicked(MouseEvent e) {
		e.consume();
	}


	public double getZoom() {
		return zoom;
	}
	
}
