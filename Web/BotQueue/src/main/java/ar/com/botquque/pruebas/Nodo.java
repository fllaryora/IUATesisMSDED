package ar.com.botquque.pruebas;

import java.awt.Graphics;
import java.util.List;

public interface Nodo{
	public boolean estaSeleccionado();
	public boolean estaArrastrado();
	//se pregunta si el punto coliciona sobre la figura del icono
	public boolean siColiciona(int x, int y, double zoom);
	//se pregunta si el punto coliciona sobre la figura del icono
	public boolean siColicionaAlMover(int x, int y, double zoom, List<Nodo> contexto, int desdeX, int hastaX,int desdeY, int hastaY );
	//TODO si coliciona una figura contra otra
	//se clickio o una parte de una flecha le esta pasando por encima
	public void seleccionar();
	//se clickio afuera o una parte de una flecha dejo de pasarle por encima
	public void deseleccionar();
	//dejo de ser arrastrado
	public void liberadoPorMouse();
	//empeso a arrastrarse o se esta arrastrando
	public void arrastrarPorMouse(int x, int y);
	//se movio los objetos con las flechitas
	public boolean siColicionaPorFlechitas(int movX, int movY, double zoom, List<Nodo> contexto, int desdeX, int hastaX,int desdeY, int hastaY );
	public void movidoPorFlechitas(int movX, int movY);
	public void dobleClick();
	public void pintar(Graphics g, double zoom);
	
	public int getXptoEnganche(double zoom, SuperficieDeEnganche pto);
	public int getYptoEnganche(double zoom, SuperficieDeEnganche pto);
	public int getOrden(); 
	public void setOrden(int orden);
}
