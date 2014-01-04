package ar.com.botqueue.applet.graphic.node;

import ar.com.botqueue.applet.enums.NodeTypes;


public  class NodeFactory {
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
