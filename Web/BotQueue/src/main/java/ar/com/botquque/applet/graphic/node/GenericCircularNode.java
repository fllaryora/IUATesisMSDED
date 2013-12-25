package ar.com.botquque.applet.graphic.node;

import ar.com.botquque.applet.enums.BindSurface;


public class GenericCircularNode extends GenericNode {
	private int diameter;
	protected double radio;
	
	public GenericCircularNode(int posX, int posY, String label, int diameter, int order) {
		super(posX, posY, label, diameter, diameter, order);
		this.diameter = diameter;
		this.radio = diameter/2.0;
	}
	
	public GenericCircularNode(int posX, int posY, String label, int diameter) {
		super(posX, posY, label, diameter, diameter, 0);
		this.diameter = diameter;
		this.radio = diameter/2.0;
	}
	
	@Override
	public boolean hasHit(int x, int y, double zoom) {
		int r = (int)(this.radio * zoom);
		int x1 = this.posX + r;
		int y2 = this.posY + r ;
		int rmouse = (x-x1)*(x-x1)+(y-y2)*(y-y2)-r*r;
		//eq pinto adentro circulo=  R'ala'2 < (x-x1)ala2 + (y-y2)ala2
		if(rmouse > 0) return false;
		return true;
	}
	
	@Override
	public int getXBindSurfacePoint(double zoom, BindSurface pto){
		double westWidth =  this.radio * zoom * (1.0 - Math.cos(Math.toRadians(45)));
		double eastWidth =  this.radio * zoom * (1.0 + Math.cos(Math.toRadians(45)));
		switch(pto){
			case WEST: return this.posX;
			case NORTH_WEST: return this.posX + (int)(westWidth);
			case SOUTH_WEST: return this.posX + (int)(westWidth);
			case NORTH: return this.posX + (int)(this.radio * zoom );
			case SOUTH: return this.posX + (int)(this.radio * zoom);
			case CENTER: return this.posX + (int)(this.radio * zoom );
			case NORTH_EAST: return this.posX + (int)(eastWidth) ;
			case SOUTH_EAST: return this.posX + (int)(eastWidth) ;
			case EAST : return this.posX + (int)(this.diameter * zoom) ;
			default: return this.posY + (int)(this.radio * zoom );
		}
	}
	
	@Override
	public int getYBindSurfacePoint(double zoom, BindSurface pto){
		double northHeight =  this.radio * zoom * (1.0 - Math.sin(Math.toRadians(45)));
		double southHeight =  this.radio * zoom * (1.0 + Math.sin(Math.toRadians(45)));
		switch(pto){
			case NORTH: return this.posY;
			case SOUTH: return this.posY + (int)(this.diameter * zoom);
			case NORTH_WEST:
				return this.posY + (int)(northHeight);
			case NORTH_EAST: 
				return this.posY + (int)(northHeight);
			case SOUTH_EAST:
				return this.posY + (int)(southHeight);
			case SOUTH_WEST:
				return this.posY + (int)(southHeight);
			case WEST: return this.posY + (int)(this.radio * zoom );
			case EAST : return this.posY + (int)(this.radio * zoom );
			case CENTER: 
			default: return this.posY + (int)(this.radio * zoom );
		}
	}
	
}
