package ar.com.botquque.applet.nodos;

import ar.com.botquque.applet.enums.SuperficieDeEnganche;


public class NodoCircularGenerico extends NodoGenerico {
	private int diametro;
	private double radio;
	
	public NodoCircularGenerico(int posX, int posY, String mensaje, int diametro, int orden) {
		super(posX, posY, mensaje, diametro, diametro, orden);
		this.diametro = diametro;
		this.radio = diametro/2.0;
	}
	
	public NodoCircularGenerico(int posX, int posY, String mensaje, int diametro) {
		super(posX, posY, mensaje, diametro, diametro, 0);
		this.diametro = diametro;
		this.radio = diametro/2.0;
	}
	
	@Override
	public boolean siColiciona(int x, int y, double zoom) {
		int r = (int)(this.radio * zoom);
		int x1 = this.posX + r;
		int y2 = this.posY + r ;
		int rmouse = (x-x1)*(x-x1)+(y-y2)*(y-y2)-r*r;
		//eq pinto adentro circulo=  R'ala'2 < (x-x1)ala2 + (y-y2)ala2
		if(rmouse > 0) return false;
		return true;
	}
	
	@Override
	public int getXptoEnganche(double zoom, SuperficieDeEnganche pto){
		double anchoOeste =  this.radio * zoom * (1.0 - Math.cos(Math.toRadians(45)));
		double anchoEste =  this.radio * zoom * (1.0 + Math.cos(Math.toRadians(45)));
		switch(pto){
			case OESTE: return this.posX;
			case NOR_OESTE: return this.posX + (int)(anchoOeste);
			case SUD_OESTE: return this.posX + (int)(anchoOeste);
			case NORTE: return this.posX + (int)(this.radio * zoom );
			case SUR: return this.posX + (int)(this.radio * zoom);
			case CENTRO: return this.posX + (int)(this.radio * zoom );
			case NOR_ESTE: return this.posX + (int)(anchoEste) ;
			case SUD_ESTE: return this.posX + (int)(anchoEste) ;
			case ESTE : return this.posX + (int)(this.diametro * zoom) ;
			default: return this.posY + (int)(this.radio * zoom );
		}
	}
	
	@Override
	public int getYptoEnganche(double zoom, SuperficieDeEnganche pto){
		double altoNorte =  this.radio * zoom * (1.0 - Math.sin(Math.toRadians(45)));
		double altoSur =  this.radio * zoom * (1.0 + Math.sin(Math.toRadians(45)));
		switch(pto){
			case NORTE: return this.posY;
			case SUR: return this.posY + (int)(this.diametro * zoom);
			case NOR_OESTE:
				return this.posY + (int)(altoNorte);
			case NOR_ESTE: 
				return this.posY + (int)(altoNorte);
			case SUD_ESTE:
				return this.posY + (int)(altoSur);
			case SUD_OESTE:
				return this.posY + (int)(altoSur);
			case OESTE: return this.posY + (int)(this.radio * zoom );
			case ESTE : return this.posY + (int)(this.radio * zoom );
			case CENTRO: 
			default: return this.posY + (int)(this.radio * zoom );
		}
	}
	
}
