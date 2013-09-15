package ar.com.botquque.pruebas;

import java.applet.Applet;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

public class PrimerApplet extends Applet implements MouseListener, MouseMotionListener, KeyListener {
	
	private static final long serialVersionUID = 1L;
	private Normal norm ;
	private double zoom ;
	private boolean mouseApretado;
	private boolean controlApretado;
	private long ultimoClick;
	/**
	 * Se llama despues de que se procesa el tag html
	 */
	public void init(){
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
		this.addKeyListener(this);
		
		this.norm = new Normal(35,35, "Soy Normal");
		this.zoom = 1.5;
		this.mouseApretado = false;
		this.controlApretado = false;
		this.ultimoClick= 0; 
	}
	
	/**
	 * Se llama despues de que el usuario cierra el navegador
	 */
	public void stop(){
		
	}
	
	/**
	 * Se llama despues de init o despues de stop 
	 * si ya estaba precargada la pagina
	 */
	public void start(){
		this.setBackground(Color.WHITE);
		/* Lo que no aclaro el color se pone en rojo
		 * asi me doy cuenta que no estoy pensando bien*/
		this.setForeground(Color.RED);
	}
	
	/**
	 * Se llama cuando el browser se cerro 
	 */
	public void destroy(){
		
	}
	
	/**
	 * Se llama para repintar o despues del metodo start
	 */
	public void paint(Graphics g){
		this.norm.pintar(g,this.zoom);
		
	}
	/** 
	 * el mouse se clickio
	 */
	public void mouseClicked(MouseEvent e) {
		long clickDeAhora =  e.getWhen();
		long tolerancia = 600; // latencia maxima para el  dobleClick en milisegundos	
		if ( clickDeAhora -this.ultimoClick  < tolerancia){//dobleclick
			
		} else{ //click simple
			if(norm.isHit(e, this.zoom)){
				norm.setColorNormal(Color.RED);
				//añado a la lista de los señalados
			}
			else{
				norm.setColorNormal(Color.BLACK);
			}
			
		}
		this.repaint();	
		e.consume();
	}

	/** 
	 * el mouse se apreto sin soltar
	 */
	public void mousePressed(MouseEvent e) {
		this.mouseApretado = true;
		if(norm.isHit(e, this.zoom)){
			norm.setColorNormal(Color.RED);
			//añado a la lista de los aggarrados
			norm.agarrar(e);
		}
		else{
			norm.setColorNormal(Color.BLACK);
		}
		this.repaint();
		e.consume();
		
	}

	/** 
	 * el mouse se acaba de soltar
	 */
	public void mouseReleased(MouseEvent e) {
		this.mouseApretado = false;
		//recorro la lista de los aggarrados y los libero
		norm.setColorNormal(Color.BLACK);
		norm.soltado();
		this.repaint();
		e.consume();
	}
	
	/**
	 *  el mouse se mueve apretando
	 */
	public void mouseDragged(MouseEvent e) {
		this.repaint();
		//recorro la lista de los agarrados y los samarreo
		norm.arrastrar(e,this.zoom, 0, this.getWidth(), 0 , this.getHeight());
		this.repaint();
		e.consume();
	}
	
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

	
	public void keyTyped(KeyEvent e) {
		System.out.println("bbbbbbbbbbbb");
		e.consume();
		
	}
	
	public void keyPressed(KeyEvent e) {
		System.out.println("AAAAAAAAAAAAAa");
		e.consume();
		
	}
	
	public void keyReleased(KeyEvent e) {
		System.out.println("ccccccccccc");
		e.consume();
		
	}
}
