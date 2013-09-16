package ar.com.botquque.pruebas;

import java.awt.Font;
import java.awt.Graphics;

public class NodoFlecha extends NodoCircularGenerico {
	private boolean mensajeado;
	public NodoFlecha(int posX, int posY, int diametro, int orden) {
		super(posX, posY, "", 3, orden);
		mensajeado = false;
	}
	
	@Override
	public void pintar(Graphics g, double zoom){
		g.setColor(this.colorActual);
		g.drawOval(this.posX, this.posY, (int)(this.ancho * zoom),  (int)(this.alto * zoom));
		if(mensajeado){
			Font fuente;
			fuente = new Font("Arial", Font.PLAIN, (int)(this.tamanioFuente * zoom) );
			Mensaje.putLabel(this.mensaje, fuente, this.colorActual , g, this.posX, this.posY + (int)(this.alto * zoom), (int)(this.ancho * zoom));
		}
	}
	
	public void ponerMensaje(String msj){
		if(msj.length() > 0){
			this.mensajeado = true;
			this.mensaje = msj;
		}
	}
	
	public void quitarMensaje(){
		this.mensajeado = false;
	}
	
}
