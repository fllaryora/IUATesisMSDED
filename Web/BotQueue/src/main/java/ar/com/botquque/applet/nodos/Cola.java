package ar.com.botquque.applet.nodos;
import java.awt.Font;
import java.awt.Graphics;

import ar.com.botquque.applet.etiquetas.Mensaje;

public class Cola extends NodoCircularGenerico{

	public Cola(int posX, int posY, String mensaje) {
		super(posX, posY, mensaje, 45, 0);
	}

	public Cola(int posX, int posY, String mensaje, int orden) {
		super(posX, posY, mensaje, 45, orden);
	}
	
	@Override
	public void pintar(Graphics g, double zoom){
		g.setColor(this.colorActual);
		g.drawOval(this.posX, this.posY, (int)(this.ancho * zoom),  (int)(this.alto * zoom));
		int x1 = this.posX +(int)(this.ancho * zoom/1.5);
		int y1 = this.posY +(int)(this.alto * zoom/1.5);
		int x2 = this.posX +(int)(this.ancho * zoom);
		int y2 = this.posY +(int)(this.alto * zoom);
		g.drawLine(x1, y1,x2, y2);
		Font fuente;
		fuente = new Font("Arial", Font.PLAIN, (int)(this.tamanioFuente * zoom) );
		Mensaje.putLabel(this.mensaje, fuente, this.colorActual , g, this.posX, this.posY + (int)(this.alto * zoom), (int)(this.ancho * zoom));
	}
	
}