package ar.com.botqueue.applet.graphic.node;

import ar.com.botqueue.applet.enums.NodeTypes;


public  class NodeFactory {
	
	public static Node createCompleteNode(NodeTypes nodeType, int posX, int posY, String label,
				boolean probBranch, String distribution, int seed, double least, double highest,
				double constant, double mean,double variance, double lambda, double mode,
				double minimun, double maximun, double shapeAlpha, double shapeBeta, double shape,
				double escale, int quantity, int cycle,int resource, double fixedCost,
				double variableCost, int input, int output){
		switch(nodeType){
		case QUEUE:
		return new Queue( posX,posY,label, resource,  fixedCost, variableCost);
		case NORMAL:
			return new Normal( posX, posY, label, probBranch, distribution, seed, 
					 least,  highest,  constant,  mean, variance, lambda,
					 mode,  minimun, maximun,  shapeAlpha,  shapeBeta,  shape, escale);
		case COUNTER:
			return new Counter( posX,posY,label, quantity, cycle);
		case FUNCTION:
			return new Function( posX,posY,label,  probBranch,  input,  output);
		case COMBI:
			return new Combi(posX, posY, label, probBranch,distribution, seed,
					least, highest, constant, mean, variance, lambda, mode, minimun,
					maximun, shapeAlpha, shapeBeta, shape, escale);
		default:
		break;
		}
		return null;
	}
	
	public static Node createNode(NodeTypes nodeType, int posX, int posY, String label){
		switch(nodeType){
		case QUEUE:
		return new Queue( posX,posY,label);
		case NORMAL:
			return new Normal( posX,posY,label);
		case COUNTER:
			return new Counter( posX,posY,label);
		case FUNCTION:
			return new Function( posX,posY,label);
		case COMBI:
			return new Combi( posX,posY,label);
		default:
		break;
		}
		return null;
	}
	
	public static Node createNode(NodeTypes nodeType, int posX, int posY, String label, int order){
		switch(nodeType){
		case ARROW_DOT: return new DotNode( posX,posY,order);
		default:
		break;
		}
		return null;
	}
	
}
