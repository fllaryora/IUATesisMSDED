package ar.com.botquque.pruebas;

import java.awt.event.MouseEvent;

public interface Nodo {
	public boolean isHit(MouseEvent e, double zoom);
	public void seleccionar();
	public void deseleccionar();
	public void agarradoPorMouse(MouseEvent e);
	public boolean movidoPorMouse(MouseEvent e);
	public boolean movidoPorFlechitas(int movX, int movY);
	public void dobleClick();
}
