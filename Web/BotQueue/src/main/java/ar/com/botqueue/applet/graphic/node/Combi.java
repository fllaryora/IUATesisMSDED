package ar.com.botqueue.applet.graphic.node;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;

import ar.com.botqueue.applet.graphic.label.Label;

public class Combi extends GenericNode{

	public Combi(int posX, int posY, String label) {
		
		super(posX, posY, label, 70, 45, 0);
	}
	
	@Override
	public void paint(Graphics g, double zoom){
		g.setColor(this.currentColor);
		g.drawRect(this.posX, this.posY, (int)(this.width * zoom),  (int)(this.height * zoom));
		Graphics2D g2 =(Graphics2D) g;
		g2.draw(new Line2D.Double(this.posX, this.posY + (this.height*zoom)/2.0, this.posX +(this.height*zoom)/2.0, this.posY));
		//g.drawLine(this.posX, this.posY + (int)(this.alto*zoom)/2, this.posX + (int)(this.alto*zoom)/2, this.posY);
		Font font = new Font("Arial", Font.PLAIN, (int)(this.fontSize * zoom) );
		Label.putLabel(this.label, font, this.currentColor , g, this.posX, this.posY + (int)(this.height * zoom), (int)(this.width * zoom));
	}
}
