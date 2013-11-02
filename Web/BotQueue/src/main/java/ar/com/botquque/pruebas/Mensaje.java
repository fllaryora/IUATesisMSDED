package ar.com.botquque.pruebas;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;


public class Mensaje {
	/**
	 * Coloca un mensaje centrado en un rectangulo imaginario
	 * el pos x y es el vertice arriba ala izquierda del rectangulo 
	 * tambien le pasas el ancho
	 * y la fuente
	 */
	public static void putLabel(String mensaje, Font fuenteTexto, Color colorTexto,Graphics g, int posX, int posY, int ancho){
		g.setColor(colorTexto);
		g.setFont(fuenteTexto);
		int anchoTexto = (int) g.getFontMetrics().getStringBounds(mensaje,g).getWidth();
		int start = ancho/2 - anchoTexto/2;
		/**
		 * posY es donde termina la figura
		 * le doy un espacio de 5 pixeles entre el texto e la figura
		 */
		int espacioEntreLinea = fuenteTexto.getSize()+5;
		g.drawString(mensaje, posX+start, posY+espacioEntreLinea);
		
	}
}
