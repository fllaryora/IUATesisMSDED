package ar.com.botquque.pruebas;
import java.awt.Font;
import java.awt.Graphics;

public class Cola extends NodoGenerico{

public Cola(int posX, int posY, String mensaje) {
		
		super(posX, posY, mensaje, 45, 45);
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
	
	@Override
	public boolean siColiciona(int x, int y, double zoom) {
		int x1 = this.posX +(int)(this.ancho * zoom)/2;
		int y2 = this.posY +(int)(this.alto * zoom)/2;
		int r = (int)(this.alto * zoom)/2;
		int rmouse = (x-x1)*(x-x1)+(y-y2)*(y-y2)-r*r;
		//eq pinto adentro circulo=  R'ala'2 < (x-x1)ala2 + (y-y2)ala2
		if(rmouse > 0) return false;
		return true;
	}
}