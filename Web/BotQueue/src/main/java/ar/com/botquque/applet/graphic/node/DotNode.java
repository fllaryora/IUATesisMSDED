package ar.com.botquque.applet.graphic.node;

import java.awt.Font;
import java.awt.Graphics;

import ar.com.botquque.applet.graphic.label.Label;

public class DotNode extends GenericCircularNode {
	private boolean labelEnabled;
	
	public DotNode(int posX, int posY,  int order) {
		super(posX, posY, "", 5, order);
		labelEnabled = false;
	}
	
	@Override
	public void paint(Graphics g, double zoom){
		g.setColor(this.currentColor);
		g.fillOval(this.posX, this.posY, (int)(this.width * zoom),  (int)(this.height * zoom));
		if(labelEnabled){
			Font font = new Font("Arial", Font.PLAIN, (int)(this.fontSize * zoom) );
			Label.putLabel(this.label, font, this.currentColor , g, this.posX, this.posY + (int)(this.height * zoom), (int)(this.width * zoom));
		}
	}
	
	public void ponerMensaje(String msg){
		if(msg.length() > 0){
			this.labelEnabled = true;
			this.label = msg;
		}
	}
	
	public void quitarMensaje(){
		this.labelEnabled = false;
	}
	
	@Override
	public boolean hasHit(int x, int y, double zoom) {
		double rMin = super.radio;
		if(rMin < 9.0) rMin = 9.0;
		double r = ( rMin * zoom);
		
		double x1 = this.posX + super.radio;
		double y2 = this.posY + super.radio;
		
		
		double rmouse = (x-x1)*(x-x1)+(y-y2)*(y-y2)-r*r;
		//eq pinto adentro circulo=  R'ala'2 < (x-x1)ala2 + (y-y2)ala2
		if(rmouse > 0.0) return false;
		return true;
	}
	
}
