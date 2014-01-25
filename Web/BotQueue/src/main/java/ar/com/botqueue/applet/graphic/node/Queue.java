package ar.com.botqueue.applet.graphic.node;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.util.List;

import ar.com.botqueue.applet.graphic.label.Label;
import ar.com.botqueue.applet.graphic.node.fields.NodeFields;

public class Queue extends GenericCircularNode{
	
	
    private int resource;
    
    private double fixedCost;
    private double variableCost;
    
	public Queue(int posX, int posY, String label) {
		super(posX, posY, label, 45, 0);
		resource = 0;
		fixedCost = 0.0;
	}
	
	@Override
	public void paint(Graphics g, double zoom){
		g.setColor(this.currentColor);
		g.drawOval(this.posX, this.posY, (int)(this.width * zoom),  (int)(this.height * zoom));
		Graphics2D g2 =(Graphics2D) g;
		g2.draw(new Line2D.Double(this.posX +(this.width * zoom/1.5),this.posY +(this.height * zoom/1.5), this.posX +(this.width * zoom), this.posY +(this.height * zoom)));
		Font font = new Font("Arial", Font.PLAIN, (int)(this.fontSize * zoom) );
		Label.putLabel(this.label, font, this.currentColor , g, this.posX, this.posY + (int)(this.height * zoom), (int)(this.width * zoom));
	}
	
	@Override
	public String getJson(int idNode, List<Integer> preceders, List<Integer> followers) {
		return "{"+
				super.putValue(NodeFields.ID_NODE , idNode)+","+
				super.putString(NodeFields.NAME, this.label)+","+
				super.putValue(NodeFields.RESOURCE,this.resource)+","+
				super.putValue(NodeFields.FIXED_COST,this.fixedCost)+","+
				super.putValue(NodeFields.VARIABLE_COST,this.variableCost)+","+
				super.putArray(NodeFields.PROCEDERS, preceders)+","+
				super.putArray(NodeFields.FOLLOWERS, followers)
				+"}";
	}
	
}