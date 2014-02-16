package ar.com.botqueue.applet.graphic.node;
import java.awt.Font;
import java.awt.Graphics;
import java.util.List;

import ar.com.botqueue.applet.enums.NodeFields;
import ar.com.botqueue.applet.graphic.label.Label;

public class Normal extends GenericNode{
	private static final int NORMAL_ORDER = 0;
	private static final int NORMAL_HEIGHT = 45;
	private static final int NORMAL_WIDTH = 70;
	private boolean probBranch;
	private String distribution;
	private int seed;
	private double least;
	private double highest;
	private double constant;
	private double mean;
	private double variance;
	private double lambda;
	private double mode;
	private double minimun;
	private double maximun;
	private double shapeAlpha;
	private double shapeBeta;
	private double shape;
	private double escale;
	
	public Normal(int posX, int posY, String label) {
		super(posX, posY, label, NORMAL_WIDTH, NORMAL_HEIGHT, NORMAL_ORDER);
		distribution = NodeFields.DETERMINISTIC;
		seed = -1;	
		least = 0.0;
		highest = 0.0;
		constant = 0.0;
		mean = 0.0;
		variance = 0.0;
		lambda = 0.0;
		mode = 0.0;
		minimun = 0.0;
		maximun = 0.0;
		shapeAlpha = 0.0;
		shapeBeta = 0.0;
		shape = 0.0;
		escale = 0.0;
		probBranch = false;
	}
	
	
	
	public Normal(int posX, int posY, String label, boolean probBranch, String distribution, int seed,
			double least, double highest, double constant, double mean,
			double variance, double lambda, double mode, double minimun,
			double maximun, double shapeAlpha, double shapeBeta, double shape, double escale) {
		super(posX, posY, label, NORMAL_WIDTH, NORMAL_HEIGHT, NORMAL_ORDER);
		this.probBranch = probBranch;
		this.distribution = distribution;
		this.seed = seed;
		this.least = least;
		this.highest = highest;
		this.constant = constant;
		this.mean = mean;
		this.variance = variance;
		this.lambda = lambda;
		this.mode = mode;
		this.minimun = minimun;
		this.maximun = maximun;
		this.shapeAlpha = shapeAlpha;
		this.shapeBeta = shapeBeta;
		this.shape = shape;
		this.escale = escale;
	}



	@Override
	public void paint(Graphics g, double zoom){
		g.setColor(this.currentColor);
		g.drawRect(this.posX, this.posY, (int)(this.width * zoom),  (int)(this.height * zoom));
		Font font = new Font("Arial", Font.PLAIN, (int)(this.fontSize * zoom) );
		Label.putLabel(this.label, font, this.currentColor , g, this.posX, this.posY + (int)(this.height * zoom), (int)(this.width * zoom));
	}
	
	@Override
	public String getJson(int idNode, List<Integer> preceders, List<Integer> followers ,  List<Double> probabilisticBranch){
		String ret = "{"+
		super.putValue(NodeFields.ID_NODE , idNode)+","+
		super.putString(NodeFields.NAME, this.label)+",";
		
		ret += "\"delay\" : {";
		if (this.distribution.equalsIgnoreCase(NodeFields.DETERMINISTIC)){
			ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.DETERMINISTIC)+","+
			super.putValue(NodeFields.CONSTANT, this.constant);
		}
		else if (this.distribution.equalsIgnoreCase(NodeFields.UNIFORM)){
			ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.UNIFORM)+","+
					super.putValue(NodeFields.HIGHEST , this.highest)+","+
					super.putValue(NodeFields.LEAST, this.least)+","+
					super.putValue(NodeFields.SEED, this.seed);
		}
		else if (this.distribution.equalsIgnoreCase(NodeFields.NORMAL)){
			ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.NORMAL)+","+
					super.putValue(NodeFields.MEAN , this.mean)+","+
					super.putValue(NodeFields.VARIANCE, this.variance)+","+
					super.putValue(NodeFields.SEED, this.seed);
		}
		else if (this.distribution.equalsIgnoreCase(NodeFields.TRIANGULAR)){
			ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.TRIANGULAR)+","+
					super.putValue(NodeFields.HIGHEST , this.highest)+","+
					super.putValue(NodeFields.LEAST, this.least)+","+
					super.putValue(NodeFields.MODE, this.mode)+","+
					super.putValue(NodeFields.SEED, this.seed);
		
		}
		else if (this.distribution.equalsIgnoreCase(NodeFields.EXPONENTIAL)){
			ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.EXPONENTIAL)+","+
					super.putValue(NodeFields.LAMBDA , this.lambda)+","+
					super.putValue(NodeFields.SEED, this.seed);
		}
		else if (this.distribution.equalsIgnoreCase(NodeFields.BETA)){
			ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.BETA)+","+
					super.putValue(NodeFields.MAXIMUN , this.maximun)+","+
					super.putValue(NodeFields.MINIMUN, this.minimun)+","+
					super.putValue(NodeFields.SHAPE_ALPHA, this.shapeAlpha)+","+
					super.putValue(NodeFields.SHAPE_BETA, this.shapeBeta)+","+
					super.putValue(NodeFields.SEED, this.seed);
		}
		else if (this.distribution.equalsIgnoreCase(NodeFields.LOG_NORMAL)){
			ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.LOG_NORMAL)+","+
					super.putValue(NodeFields.SHAPE , this.shape)+","+
					super.putValue(NodeFields.MINIMUN, this.minimun)+","+
					super.putValue(NodeFields.ESCALE, this.escale)+","+
					super.putValue(NodeFields.SEED, this.seed);
		}
		ret += "},";
		ret += super.putArray(NodeFields.PROCEDERS, preceders)+","+
		super.putArray(NodeFields.FOLLOWERS, followers);
		if( probBranch ){
			ret += ","+super.putArrayDouble(NodeFields.PROBABILISTIC_BRANCH, probabilisticBranch);
		}
		
		ret += "}";
		return ret;
	}
	
	public void editNormal(String distribution, int seed, double least, double highest, double constant,
			double mean, double variance, double lambda, double mode, double minimun, double maximun,
			double shapeAlpha, double shapeBeta, double shape, double escale, boolean probBranch, String label){
		 this.distribution = distribution;
		 this.seed = seed;
		 this.least = least;
		 this.highest = highest;
		 this.constant = constant;
		 this.mean = mean;
		 this.variance = variance;
		 this.lambda = lambda;
		 this.mode = mode;
		 this.minimun = minimun;
		 this.maximun = maximun;
		 this.shapeAlpha = shapeAlpha;
		 this.shapeBeta = shapeBeta;
		 this.shape = shape;
		 this.escale = escale;
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
				super.putValue(NodeFields.PROBAB_BRANCH , this.probBranch)+",";
				ret += "\"delay\" : {";
				if (this.distribution.equalsIgnoreCase(NodeFields.DETERMINISTIC)){
					ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.DETERMINISTIC)+","+
					super.putValue(NodeFields.CONSTANT, this.constant);
				}
				else if (this.distribution.equalsIgnoreCase(NodeFields.UNIFORM)){
					ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.UNIFORM)+","+
							super.putValue(NodeFields.HIGHEST , this.highest)+","+
							super.putValue(NodeFields.LEAST, this.least)+","+
							super.putValue(NodeFields.SEED, this.seed);
				}
				else if (this.distribution.equalsIgnoreCase(NodeFields.NORMAL)){
					ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.NORMAL)+","+
							super.putValue(NodeFields.MEAN , this.mean)+","+
							super.putValue(NodeFields.VARIANCE, this.variance)+","+
							super.putValue(NodeFields.SEED, this.seed);
				}
				else if (this.distribution.equalsIgnoreCase(NodeFields.TRIANGULAR)){
					ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.TRIANGULAR)+","+
							super.putValue(NodeFields.HIGHEST , this.highest)+","+
							super.putValue(NodeFields.LEAST, this.least)+","+
							super.putValue(NodeFields.MODE, this.mode)+","+
							super.putValue(NodeFields.SEED, this.seed);
				
				}
				else if (this.distribution.equalsIgnoreCase(NodeFields.EXPONENTIAL)){
					ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.EXPONENTIAL)+","+
							super.putValue(NodeFields.LAMBDA , this.lambda)+","+
							super.putValue(NodeFields.SEED, this.seed);
				}
				else if (this.distribution.equalsIgnoreCase(NodeFields.BETA)){
					ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.BETA)+","+
							super.putValue(NodeFields.MAXIMUN , this.maximun)+","+
							super.putValue(NodeFields.MINIMUN, this.minimun)+","+
							super.putValue(NodeFields.SHAPE_ALPHA, this.shapeAlpha)+","+
							super.putValue(NodeFields.SHAPE_BETA, this.shapeBeta)+","+
							super.putValue(NodeFields.SEED, this.seed);
				}
				else if (this.distribution.equalsIgnoreCase(NodeFields.LOG_NORMAL)){
					ret += super.putString(NodeFields.DISTRIBUTION, NodeFields.LOG_NORMAL)+","+
							super.putValue(NodeFields.SHAPE , this.shape)+","+
							super.putValue(NodeFields.MINIMUN, this.minimun)+","+
							super.putValue(NodeFields.ESCALE, this.escale)+","+
							super.putValue(NodeFields.SEED, this.seed);
				}
				ret += "},";
				ret += super.putArray(NodeFields.PROCEDERS, preceders)+","+
				super.putArray(NodeFields.FOLLOWERS, followers);
				if( probBranch ){
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

