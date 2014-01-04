package ar.com.botqueue.applet.graphic.node;
import java.awt.Font;
import java.awt.Graphics;

import ar.com.botqueue.applet.graphic.label.Label;
public class Function extends GenericCircularNode{

	public Function(int posX, int posY, String label) {
		super(posX, posY, label, 45, 0);
	}

	@Override
	public void paint(Graphics g, double zoom){
		g.setColor(this.currentColor);
		g.drawOval(this.posX, this.posY, (int)(this.width * zoom),  (int)(this.height * zoom));
		Font font = new Font("Arial", Font.PLAIN, (int)(this.fontSize * zoom) );
		Label.putLabel(this.label, font, this.currentColor , g, this.posX, this.posY + (int)(this.height * zoom), (int)(this.width * zoom));
	}
	
}
