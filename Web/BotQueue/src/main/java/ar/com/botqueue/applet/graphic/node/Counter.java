package ar.com.botqueue.applet.graphic.node;
import java.awt.Font;
import java.awt.Graphics;
import java.util.List;

import ar.com.botqueue.applet.enums.NodeFields;
import ar.com.botqueue.applet.graphic.label.Label;

public class Counter extends GenericCircularNode{

	private static final int COUNTER_ORDER = 0;
	private static final int COUNTER_DIAMETER = 45;
	private int quantity;
	private int cycle;
	
	public Counter(int posX, int posY, String label) {
		super(posX, posY, label, COUNTER_DIAMETER, COUNTER_ORDER);
	}
	
	public Counter(int posX, int posY, String label, int quantity, int cycle) {
		super(posX, posY, label, COUNTER_DIAMETER, COUNTER_ORDER);
		this.quantity = quantity;
		this.cycle = cycle;
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
	
	@Override
	public String getJson(int idNode, List<Integer> preceders, List<Integer> followers,  List<Double> probabilisticBranch) {
		return "{"+
				super.putValue(NodeFields.ID_NODE , idNode)+","+
				super.putString(NodeFields.NAME, this.label)+","+
				super.putValue(NodeFields.QUANTITY,this.quantity)+","+
				super.putValue(NodeFields.CYCLE,this.cycle)+","+
				super.putArray(NodeFields.PROCEDERS, preceders)+","+
				super.putArray(NodeFields.FOLLOWERS, followers)
				+"}";
	}
	
	public void editCounter(int quantity, int cycle, String label){
		this.quantity = quantity;
	    this.cycle = cycle;
	    this.label = label;
	}
	@Override
	public String getJsonSaveFile(int idNode, List<Integer> preceders, List<Integer> followers,  List<Double> probabilisticBranch) {
		return "{"+
				super.putValue(NodeFields.ID_NODE , idNode)+","+
				super.putString(NodeFields.NAME, this.label)+","+
				super.putValue(NodeFields.POS_X , this.posX)+","+
				super.putValue(NodeFields.POS_Y , this.posY)+","+
				super.putValue(NodeFields.QUANTITY,this.quantity)+","+
				super.putValue(NodeFields.CYCLE,this.cycle)+","+
				super.putArray(NodeFields.PROCEDERS, preceders)+","+
				super.putArray(NodeFields.FOLLOWERS, followers)
				+"}";
	}
}