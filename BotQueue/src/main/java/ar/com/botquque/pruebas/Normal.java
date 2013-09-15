package ar.com.botquque.pruebas;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.MouseEvent;

public class Normal extends NodoGenerico{

	public Normal(int posX, int posY, String mensaje) {
		
		super(posX, posY, mensaje, 70, 45);
	}
	
	@Override
	public void pintar(Graphics g, double zoom){
		g.setColor(this.colorActual);
		g.drawRect(this.posX, this.posY, (int)(this.ancho * zoom),  (int)(this.alto * zoom));
		Font fuente;
		fuente = new Font("Arial", Font.PLAIN, (int)(this.tamanioFuente * zoom) );
		Mensaje.putLabel(this.mensaje, fuente, this.colorActual , g, this.posX, this.posY + (int)(this.alto * zoom), (int)(this.ancho * zoom));
	}
	
}

