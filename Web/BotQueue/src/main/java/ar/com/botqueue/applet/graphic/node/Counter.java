package ar.com.botqueue.applet.graphic.node;
import java.awt.Font;
import java.awt.Graphics;

import ar.com.botqueue.applet.graphic.label.Label;

public class Counter extends GenericCircularNode{

	public Counter(int posX, int posY, String label) {
		super(posX, posY, label, 45, 0);
	}

	@Override
	public void paint(Graphics g, double zoom){
		g.setColor(this.currentColor);
		g.drawOval(this.posX, this.posY, (int)(this.width * zoom),  (int)(this.height * zoom));
		int[] Xs = new int[4];
		int[] Ys = new int[4];
		
		Xs[0] = this.posX +(int)(this.width * zoom/2);
		Ys[0] = this.posY;
		
		Xs[1] = this.posX +(int)(this.width * zoom/2);
		Ys[1] = this.posY - (int)(this.height * zoom/2);
		
		Xs[2] = this.posX +(int)(this.width * zoom/2)+(int)(this.width * zoom/3.5);
		Ys[2] = this.posY - (int)(this.height * zoom/3);
		
		Xs[3] = this.posX +(int)(this.width * zoom/2);
		Ys[3] = this.posY - (int)(this.height * zoom/4);
		
		g.drawPolyline(Xs, Ys, 4);
		
		Font font = new Font("Arial", Font.PLAIN, (int)(this.fontSize * zoom) );
		Label.putLabel(this.label, font, this.currentColor , g, this.posX, this.posY + (int)(this.height * zoom), (int)(this.width * zoom));
	}
}