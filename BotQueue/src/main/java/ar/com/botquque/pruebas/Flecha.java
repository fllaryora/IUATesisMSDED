package ar.com.botquque.pruebas;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;

public class Flecha {
	public Nodo empieza; //despues tiene que ser el ultimo punto
	public Nodo termina;
 
	public double phi = Math.toRadians(20) ; //un codo
	public int anchoFlecha = 20;
	
	public void invertirFlecha(){
		Nodo aux = this.empieza;
		this.empieza = this.termina;
		this.termina = aux;
	}
 
	public void pintar(Graphics g, double zoom){
		int x1 = this.empieza.getXCentral(zoom);
		int y1 = this.empieza.getYCentral(zoom);
		int x2 = this.termina.getXCentral(zoom);
		int y2 = this.termina.getYCentral(zoom);
		g.drawLine(x1, y1, x2, y2);
		pintarCabezaFlecha(g, zoom, x1, y1, x2, y2);
	}

	private void pintarCabezaFlecha(Graphics g, double zoom, int x1, int y1, int x2, int y2) {
		double magnitud = (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
		double x3, y3, x4, y4;
		double dx, dy;
		double theta, rho;
		magnitud = Math.sqrt(magnitud);
		if(magnitud > anchoFlecha * zoom * 2.0 ){
			dx = x2 - x1;
			dy = y2 - y1;
			theta = Math.atan2(dy, dx);
			rho = theta + this.phi;
			x3 = x2 - this.anchoFlecha * zoom * Math.cos(rho);
			y3 = y2 - this.anchoFlecha * zoom * Math.sin(rho);
			rho = theta - this.phi;
			x4 = x2 - this.anchoFlecha * zoom * Math.cos(rho);
			y4 = y2 - this.anchoFlecha * zoom * Math.sin(rho);
			Graphics2D g2 =(Graphics2D) g;
			g2.draw(new Line2D.Double(x2, y2, x3, y3));
			g2.draw(new Line2D.Double(x2, y2, x4, y4));
		}
	}
	
	
	
}
