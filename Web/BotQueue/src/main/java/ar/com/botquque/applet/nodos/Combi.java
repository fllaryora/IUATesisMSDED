package ar.com.botquque.applet.nodos;
import java.awt.Font;
import java.awt.Graphics;

import ar.com.botquque.applet.etiquetas.Mensaje;

public class Combi extends NodoGenerico{

	public Combi(int posX, int posY, String mensaje, int orden) {
		
		super(posX, posY, mensaje, 70, 45, orden);
	}
	
	public Combi(int posX, int posY, String mensaje) {
		
		super(posX, posY, mensaje, 70, 45, 0);
	}
	@Override
	public void pintar(Graphics g, double zoom){
		g.setColor(this.colorActual);
		g.drawRect(this.posX, this.posY, (int)(this.ancho * zoom),  (int)(this.alto * zoom));
		g.drawLine(this.posX, this.posY + (int)(this.alto*zoom)/2, this.posX + (int)(this.alto*zoom)/2, this.posY);
		Font fuente;
		fuente = new Font("Arial", Font.PLAIN, (int)(this.tamanioFuente * zoom) );
		Mensaje.putLabel(this.mensaje, fuente, this.colorActual , g, this.posX, this.posY + (int)(this.alto * zoom), (int)(this.ancho * zoom));
	}
}
