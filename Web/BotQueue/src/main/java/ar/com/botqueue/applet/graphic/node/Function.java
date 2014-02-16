package ar.com.botqueue.applet.graphic.node;
import java.awt.Font;
import java.awt.Graphics;
import java.util.List;

import ar.com.botqueue.applet.enums.NodeFields;
import ar.com.botqueue.applet.graphic.label.Label;
public class Function extends GenericCircularNode{
	private static final int FUNCTION_ORDER = 0;
	private static final int FUNCTION_DIAMETER = 45;
	private boolean probBranch;
	private int input;
	private int output;
	
	public Function(int posX, int posY, String label) {
		super(posX, posY, label, FUNCTION_DIAMETER, FUNCTION_ORDER);
		input = 0;
		output = 0;
		probBranch = false;
	}

	
	public Function(int posX, int posY, String label, boolean probBranch, int input, int output) {
		super(posX, posY, label, FUNCTION_DIAMETER, FUNCTION_ORDER);
		this.probBranch = probBranch;
		this.input = input;
		this.output = output;
	}


	@Override
	public void paint(Graphics g, double zoom){
		g.setColor(this.currentColor);
		g.drawOval(this.posX, this.posY, (int)(this.width * zoom),  (int)(this.height * zoom));
		Font font = new Font("Arial", Font.PLAIN, (int)(this.fontSize * zoom) );
		Label.putLabel(this.label, font, this.currentColor , g, this.posX, this.posY + (int)(this.height * zoom), (int)(this.width * zoom));
	}
	
	@Override
	public String getJson(int idNode, List<Integer> preceders, List<Integer> followers,  List<Double> probabilisticBranch) {
		String ret = "{"+
		super.putValue(NodeFields.ID_NODE , idNode)+","+
		super.putString(NodeFields.NAME, this.label)+","+
		super.putValue(NodeFields.INPUT,this.input)+","+
		super.putValue(NodeFields.OUT_PUT,this.output)+","+
		super.putArray(NodeFields.PROCEDERS, preceders)+","+
		super.putArray(NodeFields.FOLLOWERS, followers);
		if(probBranch){
			ret += ","+super.putArrayDouble(NodeFields.PROBABILISTIC_BRANCH, probabilisticBranch);
		}
		
		ret += "}";
		return ret;
	}
	
	public void editFunction(int input, int output, boolean probBranch, String label){
		this.input = input;
	    this.output = output;
	    this.probBranch = probBranch;
	    this.label = label;
	}
	@Override
	public String getJsonSaveFile(int idNode, List<Integer> preceders, List<Integer> followers,  List<Double> probabilisticBranch) {
			String ret = "{"+
			super.putValue(NodeFields.ID_NODE , idNode)+","+
			super.putString(NodeFields.NAME, this.label)+","+
			super.putValue(NodeFields.POS_X , this.posX)+","+
			super.putValue(NodeFields.POS_Y , this.posY)+","+
			super.putValue(NodeFields.PROBAB_BRANCH , this.probBranch)+","+
			super.putValue(NodeFields.INPUT,this.input)+","+
			super.putValue(NodeFields.OUT_PUT,this.output)+","+
			super.putArray(NodeFields.PROCEDERS, preceders)+","+
			super.putArray(NodeFields.FOLLOWERS, followers);
			if(probBranch){
				ret += ","+super.putArrayDouble(NodeFields.PROBABILISTIC_BRANCH, probabilisticBranch);
			}
			
			ret += "}";
			return ret;
	}
	
	@Override
	public boolean isProbBranch() {
		return this.probBranch;
	}
	
}
