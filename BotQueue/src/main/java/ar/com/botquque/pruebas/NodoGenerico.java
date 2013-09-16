package ar.com.botquque.pruebas;

import java.awt.Color;
import java.awt.Graphics;
import java.util.List;
/**
 * Define un Un cubo donde habra un simbolo y debajo tendra un mensaje
 * La clase que extiende de NodoGenerico y deba extender la funcionalidad
 * de los metodos de nodo deberá poner el override.
 * @author redes3
 *
 */
public class NodoGenerico implements Nodo, Comparable<Nodo>{
	//van a ser usadas por subclases
	protected int posX;
	protected int posY;
	protected final int ancho ;
	protected final int alto ;
	protected String mensaje;
	protected final int tamanioFuente;
	protected Color colorActual;
	
	protected final int orden;
	//auxiliares para eventos
	private boolean esSeleccionado;
	//diferencia entre el puntero y la selección
	private int diferenciaX;
	private int diferenciaY;
	private boolean esArrastrado; //esta siendo arrastrado
	
	/**
	 * 
	 * @param posX vertice arriba a la izquierda
	 * @param posY vertice arriba a la izquierda
	 * @param mensaje debajo del cubo
	 * @param ancho del cubo
	 * @param alto del cubo
	 */
	public NodoGenerico(int posX, int posY, String mensaje, int ancho, int alto, int orden) {
		this.posX = posX;
		this.posY = posY;
		this.mensaje = mensaje;
		this.ancho = ancho;
		this.alto = alto;
		this.orden = orden;
		this.colorActual = Color.BLACK;
		this.esArrastrado = false;
		this.tamanioFuente = 12;
		this.esSeleccionado = false;
		
	}

	public boolean siColiciona(int x, int y, double zoom) {
		if(x > this.posX +(int)(this.ancho * zoom)) return false;
		if(x < this.posX) return false;
		if(y > this.posY +(int)(this.alto * zoom)) return false;
		if(y < this.posY) return false;
		return true;
	}

	public void seleccionar() {
		this.esSeleccionado = true;
		this.colorActual = Color.RED;
	}

	public void deseleccionar() {
		this.esSeleccionado = false;
		this.colorActual = Color.BLACK;
	}

	public void liberadoPorMouse() {
		this.esArrastrado = false;
	}

	public boolean siColicionaAlMover(int x, int y, double zoom, List<Nodo> contexto, int desdeX, int hastaX,int desdeY, int hastaY ) {
		if(!this.esArrastrado){
			return false;
		} else{
			int nuevoX = x - this.diferenciaX; 
			int nuevoY = y - this.diferenciaY;
			if( nuevoX < desdeX){
				return true;
				}
			if( nuevoY < desdeY){
				return true;
			}
			
			if( nuevoX + (int)(this.ancho * zoom) > hastaX){
				return true;
			}
			if( nuevoY + (int)(this.alto * zoom) > hastaY){
				return true;	
			}
			///TODO
			//recibe lista de nodos que no se mueve
			//si toco algo no muevo?
			return false;	
		}
	}
	
	public void arrastrarPorMouse(int x, int y ) {
		if(!this.esArrastrado){
			this.diferenciaX = x - this.posX;
			this.diferenciaY = y - this.posY;
			this.esArrastrado = true;
			this.esSeleccionado = true;
			this.colorActual = Color.RED;
			return;
		} else{
			this.posX = x - this.diferenciaX;
			this.posY = y - this.diferenciaY;
			return;	
		}
	}

	public void movidoPorFlechitas(int movX, int movY) {
			this.posX += movX;
			this.posY += movY;
			return;	
	}
	
	public boolean siColicionaPorFlechitas(int movX, int movY, double zoom, List<Nodo> contexto, int desdeX, int hastaX,int desdeY, int hastaY ){
		int nuevoX = movX + this.posX; 
		int nuevoY = movY + this.posY;
		
		if( nuevoX < desdeX) return true;
		if( nuevoY < desdeY) return true;
		
		if( nuevoX + (int)(this.ancho * zoom) > hastaX) return true;
		if( nuevoY +(int)(this.alto * zoom) > hastaY) return true;
		//TODO
		//recibe lista de nodos que no se mueve
		//si toco algo no muevo?
		return false;
	}
	
	/**
	 * se overrrideara si o si
	 */
	public void dobleClick() {
		return;
	}
	
	/**
	 * se overrrideara si o si
	 */
	public void pintar(Graphics g, double zoom) {
		return;	
	}

	public boolean estaSeleccionado() {
		return this.esSeleccionado;
	}

	public boolean estaArrastrado() {
		return this.esArrastrado;
	}
	
	public int getXptoEnganche(double zoom, SuperficieDeEnganche pto){
		switch(pto){
			case NOR_OESTE: return this.posX;
			case NORTE: return this.posX + (int)(this.ancho * zoom)/2;
			case NOR_ESTE: return this.posX + (int)(this.ancho * zoom);
			case ESTE : return this.posX + (int)(this.ancho * zoom);
			case SUD_ESTE: return this.posX + (int)(this.ancho * zoom);
			case SUR: return this.posX + (int)(this.ancho * zoom)/2;
			case SUD_OESTE: return this.posX;
			case OESTE: return this.posX;
			case CENTRO:
			default: return this.posX + (int)(this.ancho * zoom)/2;
		}
	}
	
	public int getYptoEnganche(double zoom, SuperficieDeEnganche pto){
		switch(pto){
			case NOR_OESTE: return this.posY;
			case NORTE: return this.posY;
			case NOR_ESTE: return this.posY;
			case ESTE : return this.posY + (int)(this.alto * zoom)/2;
			case SUD_ESTE: return this.posY + (int)(this.alto * zoom);
			case SUR: return this.posY + (int)(this.alto * zoom);
			case SUD_OESTE: return this.posY + (int)(this.alto * zoom);
			case OESTE: return this.posY + (int)(this.alto * zoom)/2;
			case CENTRO:
			default: return this.posY + (int)(this.alto * zoom)/2;
		}
	}

	public int compareTo(Nodo laOtraInstancia) {
		// TODO Auto-generated method stub
		return this.orden - laOtraInstancia.getOrden();
	}

	public int getOrden(){
		return this.orden;
	}
	
}
