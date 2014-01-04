package ar.com.botqueue.applet.graphic.label;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;


public class Label {
	private static final int INTER_SPACE = 5;

	/**
	 * Coloca un mensaje centrado en un rectangulo imaginario
	 * el pos x y es el vertice arriba ala izquierda del rectangulo 
	 * tambien le pasas el ancho
	 * y la fuente
	 */
	public static void putLabel(String message, Font textFont, Color textColor, Graphics g, int posX, int posY, int width){
		g.setColor(textColor);
		g.setFont(textFont);
		int textWidth = (int) g.getFontMetrics().getStringBounds(message,g).getWidth();
		int start = width/2 - textWidth/2;
		/**
		 * posY es donde termina la figura
		 * le doy un espacio de 5 pixeles entre el texto e la figura
		 */
		int espacioEntreLinea = textFont.getSize()+INTER_SPACE;
		g.drawString(message, posX+start, posY+espacioEntreLinea);
		
	}
}
