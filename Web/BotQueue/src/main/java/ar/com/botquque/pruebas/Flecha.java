package ar.com.botquque.pruebas;

import java.awt.Graphics;

public interface Flecha {
	
	public void invertirFlecha();
	public void entrarPor(SuperficieDeEnganche ptoEntrada);
	public void salirPor(SuperficieDeEnganche ptoSalida);
	public Nodo agregarPtoAntes(Nodo pto, double zoom);
	public Nodo agregarPtoDespues(Nodo pto, double zoom);
	public void quitarPto(Nodo pto);
	
	public void pintar(Graphics g, double zoom);
	
	/**
	 * Los Medio news
	 */
	public boolean entrada(Nodo nodoEntrada, double zoom);
	public boolean salida(Nodo nodoSalida, double zoom);
	
	public void quitarEntrada();
	public void quitarSalida();
	
}
