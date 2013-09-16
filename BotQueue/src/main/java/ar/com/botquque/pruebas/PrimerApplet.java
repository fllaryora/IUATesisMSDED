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
	private boolean controlApretado;
	private long ultimoClick;
	
	//dimensiones del podio de los nodos
	private int xPodioMin = 0;
	private int xPodioMax = this.getWidth();
	private int yPodioMin = 0;
	private int yPodioMax = this.getHeight();
	
	/**
	 * Se llama despues de que se procesa el tag html
	 */
	public void init(){
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
		this.addKeyListener(this);
		//no se puede escribir sobre algo que no tiene foco.
		this.setFocusable(true);
		this.requestFocusInWindow();
		
		nodos = new ArrayList<Nodo>();
		
		Nodo a = NodoBuilder.createNodo(NodoBuilder.CONTADOR, 60, 60, "Contador");
		Nodo b = NodoBuilder.createNodo(NodoBuilder.NORMAL, 180, 60, "Normal");
		ff.empieza = a;
		ff.termina = b;
		nodos.add(a);
		nodos.add(b);
		
		nodos.add( NodoBuilder.createNodo(NodoBuilder.COLA, 60, 180, "Cola") );
		nodos.add( NodoBuilder.createNodo(NodoBuilder.FUNCION, 60, 180+60, "Funcion") );
		nodos.add( NodoBuilder.createNodo(NodoBuilder.COMBI, 180, 180, "Combi") );
		this.zoom = 1.0;
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
	 * Se llama para repintar o despues del metodo start, o la mierda del resizear que me hacia bosta!!!!
	 */
	public void paint(Graphics g){
		this.xPodioMin = 0;
		this.xPodioMax = this.getWidth();
		this.yPodioMin = 0;
		this.yPodioMax = this.getHeight();
		
		
		for(Nodo nodo: nodos){
			nodo.pintar(g,this.zoom);
		}
		ff.pintar(g, zoom);
		
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
	private void click(MouseEvent e){

		boolean clickEnLaNada = true;
		for(Nodo nodo: nodos){
			boolean coliciona =  nodo.siColiciona( e.getX(), e.getY(), zoom) ;
			clickEnLaNada &= !coliciona;
			if(coliciona) break;
		}
		if(clickEnLaNada){ 
			if(!this.controlApretado){
				for(Nodo nodo: nodos){
					nodo.deseleccionar();
				}
			}
			return;
		}
		if(this.controlApretado){
			for(Nodo nodo: nodos){
				if( nodo.siColiciona( e.getX(), e.getY(), zoom) ){
					if( nodo.estaSeleccionado() ){
						nodo.deseleccionar();
					} else {
						nodo.seleccionar();
					}
					break;
				}
			}
		} else {
			// se pone heavy
			resolvedorDeConflicto = PRENDIDO;
			for(Nodo nodo: nodos){
				if( nodo.siColiciona( e.getX(), e.getY(), zoom) ){
					nodo.seleccionar();
					break;
				}
			}
		}
	}

	private void dobleClick(MouseEvent e){
		//TODO que si coliciona en 2 nodos solo tome el primero
		for(Nodo nodo: nodos){
			if( nodo.siColiciona( e.getX(), e.getY(), zoom) ){
				nodo.dobleClick();
				break;
			}
		}
	}
	
	private void clickDerecho(MouseEvent e){
		//TODO click derecho!
		return;
	}
	
	/** 
	 * el mouse se apreto sin soltar
	 */
	public void mousePressed(MouseEvent e) {
		long clickDeAhora =  e.getWhen();
		long tolerancia = 600; // latencia maxima para el  dobleClick en milisegundos
		boolean clickIzquierdo = (e.getButton() == MouseEvent.BUTTON1);
		boolean dobleClick = (clickDeAhora -this.ultimoClick  < tolerancia);
		if ( dobleClick ){
			
			if(clickIzquierdo){
				this.dobleClick(e);
			} else {
				//TODO dobleclick derecho!
			}
			
		} else { //click simple
			if(clickIzquierdo){
				this.click(e);
			} else {
				this.clickDerecho(e);
			}
		}
		this.repaint();
		e.consume();
	}

	/**
	 *  el mouse se mueve apretando
	 */
	public void mouseDragged(MouseEvent e) {
		// parte nueva
		if(resolvedorDeConflicto == PRENDIDO){
			resolvedorDeConflicto = CALIENTE;
		}
		//fin parte nueva
		boolean clickIzquierdo = (e.getModifiersEx() == (MouseEvent.BUTTON1_DOWN_MASK & ~MouseEvent.BUTTON2_DOWN_MASK));
		if(clickIzquierdo){
			boolean noChocaConAlgo = true;	
			
			for(Nodo nodo: nodos){
				if( nodo.estaSeleccionado() ){
					if( nodo.siColicionaAlMover(e.getX(), e.getY(), this.zoom,
								nodos, this.xPodioMin, this.xPodioMax, this.yPodioMin, this.yPodioMax ) ){
						
						noChocaConAlgo = false;
					}
				}
			}
			
			if(noChocaConAlgo){	
				for(Nodo nodo: nodos){
					if( nodo.estaSeleccionado() ){
						nodo.arrastrarPorMouse( e.getX(), e.getY() );
					}
				}
			}
			this.repaint();
			e.consume();
		}
	}
	
	
	/** 
	 * el mouse se acaba de soltar
	 */
	public void mouseReleased(MouseEvent e) {
		boolean clickIzquierdo = (e.getButton() == MouseEvent.BUTTON1);
		if(clickIzquierdo){
			for(Nodo nodo: nodos){
				if(nodo.estaArrastrado() ){
					nodo.liberadoPorMouse();
				}
			}
			
			if(resolvedorDeConflicto == PRENDIDO){
				for(Nodo nodo: nodos){
					nodo.deseleccionar();
				}
				for(Nodo nodo: nodos){
					if( nodo.siColiciona( e.getX(), e.getY(), zoom) ){
						nodo.seleccionar();
						break;
					}
				}
			}
			resolvedorDeConflicto = APAGADO;
			this.repaint();
			e.consume();
		}
	}
	
	public void keyTyped(KeyEvent e) {
		this.controlApretado = e.isControlDown();
		e.consume();
	}
	
	public void keyPressed(KeyEvent e) {
		if(e.getKeyCode() == KeyEvent.VK_DOWN){
			mouevoFlechas( 0, 1);
		} 
		if(e.getKeyCode() == KeyEvent.VK_UP){
			mouevoFlechas( 0, -1);
		}
		if(e.getKeyCode() == KeyEvent.VK_LEFT){
			mouevoFlechas( -1, 0);
		}
		if(e.getKeyCode() == KeyEvent.VK_RIGHT){
			mouevoFlechas( 1, 0);
		}
		//System.out.println("Presed "+ e.paramString());
		this.controlApretado = e.isControlDown();
		e.consume();
	}
	
	private void mouevoFlechas(int horizontal, int vertical){
		boolean noChocaConAlgo = true;	
		//algun nodo choca contra la pared
		for(Nodo nodo: nodos){
			if( nodo.estaSeleccionado() ){
				if( nodo.siColicionaPorFlechitas(horizontal, vertical, this.zoom,
							nodos, this.xPodioMin, this.xPodioMax, this.yPodioMin, this.yPodioMax) ){
					noChocaConAlgo = false;
				}
			}
		}
		System.out.println(" no Choca "+ (noChocaConAlgo?"yes":"no" ));
		if(noChocaConAlgo){	
			for(Nodo nodo: nodos){
				if( nodo.estaSeleccionado() ){
					nodo.movidoPorFlechitas(horizontal, vertical);
				}
			}
		}
		this.repaint();
	}
	
	public void keyReleased(KeyEvent e) {
		this.controlApretado = e.isControlDown();
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
}
