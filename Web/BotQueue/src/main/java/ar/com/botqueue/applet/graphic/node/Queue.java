package ar.com.botqueue.applet.graphic.node;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.util.List;

import ar.com.botqueue.applet.enums.NodeFields;
import ar.com.botqueue.applet.graphic.label.Label;

public class Queue extends GenericCircularNode{
	
	
    private static final int QUEUE_ORDER = 0;
	private static final int QUEUE_DIAMETER = 45;
	private int resource;
    private double fixedCost;
    private double variableCost;
    
	public Queue(int posX, int posY, String label) {
		super(posX, posY, label, 45, 0);
		resource = 0;
		fixedCost = 0.0;
		variableCost = 0.0;
	}
	
	public Queue(int posX, int posY, String label, int resource, double fixedCost, double variableCost) {
		super(posX, posY, label, QUEUE_DIAMETER, QUEUE_ORDER);
		this.resource = resource;
		this.fixedCost = fixedCost;
		this.variableCost = variableCost;
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
	public String getJson(int idNode, List<Integer> preceders, List<Integer> followers,  List<Double> probabilisticBranch) {
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
	
	@Override
	public String getJsonSaveFile(int idNode, List<Integer> preceders, List<Integer> followers,  List<Double> probabilisticBranch) {
		return "{"+
				super.putValue(NodeFields.ID_NODE , idNode)+","+
				super.putString(NodeFields.NAME, this.label)+","+
				super.putValue(NodeFields.POS_X , this.posX)+","+
				super.putValue(NodeFields.POS_Y , this.posY)+","+
				super.putValue(NodeFields.RESOURCE,this.resource)+","+
				super.putValue(NodeFields.FIXED_COST,this.fixedCost)+","+
				super.putValue(NodeFields.VARIABLE_COST,this.variableCost)+","+
				super.putArray(NodeFields.PROCEDERS, preceders)+","+
				super.putArray(NodeFields.FOLLOWERS, followers)
				+"}";
	}
	
	public void editQueue(int resource, double fixedCost, double variableCost , String label){
		this.label = label;
		this.resource = resource;
	    this.fixedCost = fixedCost;
	    this.variableCost = variableCost;
	}
	
}