package ar.com.botquque.pruebas;

import java.applet.Applet;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.util.List;
import java.util.ArrayList;

public class PrimerApplet extends Applet implements MouseListener, MouseMotionListener, KeyListener {
	
	private static final long serialVersionUID = 2L;
	private List<Nodo> nodos;
	Flecha ff = new Flecha();
	
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
		nodos = new ArrayList<Nodo>();
		
		Nodo a = NodoBuilder.createNodo(NodoBuilder.CONTADOR, 60, 60, "Muy fumado");
		Nodo b = NodoBuilder.createNodo(NodoBuilder.FUNCION, 120, 60, "Muy fooo");
		ff.empieza = a;
		ff.termina = b;
		nodos.add(a);
		nodos.add(b);
		
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
		for(Nodo nodo: nodos){
			nodo.pintar(g,this.zoom);
		}
		ff.pintar(g, zoom);
		
	}
	/** 
	 * el mouse se clickio
	 */
	public void mouseClicked(MouseEvent e) {
		long clickDeAhora =  e.getWhen();
		long tolerancia = 600; // latencia maxima para el  dobleClick en milisegundos
		boolean clickIzquierdo = (e.getButton() == MouseEvent.BUTTON1);
		boolean dobleClick = (clickDeAhora -this.ultimoClick  < tolerancia);
		
		if ( dobleClick){
			//TODO que si coliciona en 2 nodos solo tome el primero
			if(clickIzquierdo){
				for(Nodo nodo: nodos){
					if( nodo.siColiciona( e.getX(), e.getY(), zoom) ){
						nodo.dobleClick();
					}
				}
			} else {
				//TODO dobleclick derecho!
			}
			
		} else{ //click simple
			if(clickIzquierdo){
				//TODO que si coliciona en 2 nodos solo tome el primero
				for(Nodo nodo: nodos){
					boolean coliciona =  nodo.siColiciona( e.getX(), e.getY(), zoom) ;
					
					//seleccion individual
					if(coliciona && !this.controlApretado){
						nodo.seleccionar();
					}
					
					//seleccion colectiva
					if(coliciona && this.controlApretado){
						if( nodo.estaSeleccionado() ){
							nodo.deseleccionar();
						} else {
							nodo.seleccionar();
						}
					}
					
					//libero grupal
					if(!coliciona && !this.controlApretado){
							nodo.deseleccionar();
					}
				}
			} else {
				//TODO click derecho!
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
		boolean porMoverSeguro = false;
		
		//algun nodo debe moverse?
		for(Nodo nodo: nodos){
			if( nodo.siColiciona( e.getX(), e.getY(), zoom) ){
				nodo.seleccionar();
				porMoverSeguro = true;
			} else {
				if(!this.controlApretado){
					nodo.deseleccionar();
				}
			}
			
		}
		if(porMoverSeguro){
			for(Nodo nodo: nodos){
				if( nodo.estaSeleccionado() ){
					nodo.arrastrarPorMouse( e.getX(), e.getY());
				}
			}
		}
		this.repaint();
		e.consume();
		
	}

	/** 
	 * el mouse se acaba de soltar
	 */
	public void mouseReleased(MouseEvent e) {
		this.mouseApretado = false;
		for(Nodo nodo: nodos){
			if(nodo.estaArrastrado() ){
				nodo.liberadoPorMouse();
			}
		}
		this.repaint();
		e.consume();
	}
	
	/**
	 *  el mouse se mueve apretando
	 */
	public void mouseDragged(MouseEvent e) {
		this.repaint();
		boolean porMoverSeguro = true;	
		
		//algun nodo choca contra la pared
		for(Nodo nodo: nodos){
			if( nodo.estaArrastrado() ){
				if ( nodo.siColicionaAlMover(e.getX(), e.getY(), zoom , nodos , 0, this.getWidth() , 0, this.getHeight() ) ){
					porMoverSeguro = false;
					break;
				}	
			}
		}
	
		if(porMoverSeguro){	
			for(Nodo nodo: nodos){
				if( nodo.estaArrastrado() || nodo.estaSeleccionado() ){
					nodo.arrastrarPorMouse( e.getX(), e.getY() );
				}
			}
		}
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
