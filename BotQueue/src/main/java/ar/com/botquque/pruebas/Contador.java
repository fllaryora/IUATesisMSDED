package ar.com.botquque.pruebas;
import java.awt.Font;
import java.awt.Graphics;

public class Contador extends NodoGenerico{

public Contador(int posX, int posY, String mensaje) {
		
		super(posX, posY, mensaje, 45, 45);
	}

	@Override
	public void pintar(Graphics g, double zoom){
		g.setColor(this.colorActual);
		g.drawOval(this.posX, this.posY, (int)(this.ancho * zoom),  (int)(this.alto * zoom));
		int[] Xs = new int[4];
		int[] Ys = new int[4];
		
		Xs[0] = this.posX +(int)(this.ancho * zoom/2);
		Ys[0] = this.posY;
		
		Xs[1] = this.posX +(int)(this.ancho * zoom/2);
		Ys[1] = this.posY - (int)(this.alto * zoom/2);
		
		Xs[2] = this.posX +(int)(this.ancho * zoom/2)+(int)(this.ancho * zoom/3.5);
		Ys[2] = this.posY - (int)(this.alto * zoom/3);
		
		Xs[3] = this.posX +(int)(this.ancho * zoom/2);
		Ys[3] = this.posY - (int)(this.alto * zoom/4);
		
		g.drawPolyline(Xs, Ys, 4);
		
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