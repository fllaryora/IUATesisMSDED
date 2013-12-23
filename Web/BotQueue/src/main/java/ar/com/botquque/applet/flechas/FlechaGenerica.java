package ar.com.botquque.applet.flechas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import ar.com.botquque.applet.enums.SuperficieDeEnganche;
import ar.com.botquque.applet.nodos.Nodo;
import ar.com.botquque.applet.nodos.NodoBuilder;
import ar.com.botquque.applet.nodos.NodoGenerico;

public class FlechaGenerica implements Flecha{
	//los puntos en la flecha
	private List<NodoGenerico> nodosFlecha;
	private boolean hayNodoCabeza;
	private boolean hayNodoCola;
	private Nodo empieza; 
	// la flecha va del ultimo punto a este nodo
	private Nodo termina;
	
	private SuperficieDeEnganche empiezaPto;
	private SuperficieDeEnganche terminaPto;
	
	private final double phi = Math.toRadians(20);
	private final int anchoFlecha = 10;
	private Color colorActual;
	private List<Nodo> nodosExternos;
	
	public FlechaGenerica(List<Nodo> nodosPanel ){
		this.nodosFlecha = new ArrayList<NodoGenerico>();
		this.hayNodoCabeza = false;
		this.hayNodoCola = false;
		this.nodosExternos = nodosPanel; 
		this.empieza = null;
		this.termina = null;
		this.colorActual = Color.BLACK;
		this.empiezaPto = SuperficieDeEnganche.CENTRO;
		this.terminaPto = SuperficieDeEnganche.CENTRO;
		
	}
	
	/**************los verdaderos constructores*****************************/
	public boolean entrada(Nodo nodoEntrada, double zoom) {
		this.empieza = nodoEntrada;
		this.hayNodoCola = true;
		this.empiezaPto = SuperficieDeEnganche.ESTE;
		if(this.hayNodoCabeza && this.hayNodoCola){
			crearPtoMedio(zoom);
			return true;
		}
		return false;
	}

	public boolean salida(Nodo nodoSalida, double zoom) {
		this.termina = nodoSalida;
		this.hayNodoCabeza = true;
		this.terminaPto = SuperficieDeEnganche.OESTE;
		if(this.hayNodoCabeza && this.hayNodoCola){
			crearPtoMedio(zoom);
			return true;
		}
		return false;
	}
	
	public void quitarEntrada() {
		this.empieza = null;
		this.hayNodoCola = false;
		this.empiezaPto = SuperficieDeEnganche.CENTRO;
		nodosFlecha.clear();
	}

	public void quitarSalida() {
		this.termina = null;
		this.hayNodoCabeza = false;
		this.terminaPto = SuperficieDeEnganche.CENTRO;
		nodosFlecha.clear();
	}
	
	public void crearPtoMedio(double zoom){
		int x = (this.termina.getXptoEnganche(zoom, this.terminaPto) + this.empieza.getXptoEnganche(zoom, this.empiezaPto))/2 ;
		int y = (this.termina.getYptoEnganche(zoom, this.terminaPto) + this.empieza.getYptoEnganche(zoom, this.empiezaPto))/2 ;
		Nodo e = NodoBuilder.createNodo(NodoBuilder.NODOFLECHA, x, y, "", 0);
		nodosFlecha.add((NodoGenerico)e);
	}
	/**************los verdaderos constructores*****************************/
	
	public void invertirFlecha(){
		Nodo aux = this.empieza;
		this.empieza = this.termina;
		this.termina = aux;
	}

	public void quitarPto(Nodo pto) {
		if(this.hayNodoCabeza && this.hayNodoCola){
			if(nodosFlecha.size() > 1){
				nodosFlecha.remove(pto);
			}
		}
	}
	
	public void entrarPor(SuperficieDeEnganche ptoEntrada) {
		this.empiezaPto = ptoEntrada;
	}

	public void salirPor(SuperficieDeEnganche ptoSalida) {
		this.terminaPto = ptoSalida;
	}

	public Nodo agregarPtoDespues(Nodo pto, double zoom) {
		int xInicial =0, yInicial=0;
		int xFinal = pto.getXptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
		int yFinal = pto.getYptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
		int currentOrder = pto.getOrden();
		if(this.hayNodoCabeza && this.hayNodoCola){
			int size = this.nodosFlecha.size();
			if( currentOrder + 1 == size){//debo usar el punto extremo?
				xInicial = this.termina.getXptoEnganche(zoom, this.terminaPto) ;
				yInicial = this.termina.getYptoEnganche(zoom, this.terminaPto) ;
			} else{
				for(Nodo nodo: nodosFlecha){
					if (nodo.getOrden() == pto.getOrden() + 1){
						xInicial = nodo.getXptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
						yInicial = nodo.getYptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
						break;
					}
				}
				
			}
			
			int x = (xFinal + xInicial)/2;
			int y = (yFinal + yInicial)/2;;
			Nodo e = NodoBuilder.createNodo(NodoBuilder.NODOFLECHA, x, y, "", currentOrder+1);
			for(Nodo nodo: nodosFlecha){
				if (nodo.getOrden() > pto.getOrden()){
					nodo.setOrden(nodo.getOrden() +1);
				}
			}
			nodosFlecha.add((NodoGenerico)e);
			Collections.sort(nodosFlecha);
			return e;
		}
		return null;
	}

	public Nodo agregarPtoAntes(Nodo pto, double zoom){
		int xInicial =0, yInicial=0;
		int xFinal = pto.getXptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
		int yFinal = pto.getYptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
		int currentOrder = pto.getOrden();
		if(this.hayNodoCabeza && this.hayNodoCola){
			if( currentOrder == 0){//debo usar el punto extremo?
				xInicial = this.empieza.getXptoEnganche(zoom, this.empiezaPto) ;
				yInicial = this.empieza.getYptoEnganche(zoom, this.empiezaPto) ;
			} else{
				for(Nodo nodo: nodosFlecha){
					if (nodo.getOrden() == pto.getOrden() - 1){
						xInicial = nodo.getXptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
						yInicial = nodo.getYptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
						break;
					}
				}
				
			}
			
			int x = (xFinal + xInicial)/2;
			int y = (yFinal + yInicial)/2;;
			Nodo e = NodoBuilder.createNodo(NodoBuilder.NODOFLECHA, x, y, "", currentOrder-1);
			for(Nodo nodo: nodosFlecha){
				if (nodo.getOrden() < pto.getOrden()){
					nodo.setOrden(nodo.getOrden() -1);
				}
			}
			nodosFlecha.add((NodoGenerico)e);
			Collections.sort(nodosFlecha);
			return e;
		}
		return null;
	}
	
	public void pintar(Graphics g, double zoom){
		int x1, x2, y1, y2; 
		if(this.hayNodoCabeza && this.hayNodoCola){
			g.setColor(this.colorActual);
			
			x1 = this.empieza.getXptoEnganche(zoom, empiezaPto);
			y1 = this.empieza.getYptoEnganche(zoom, empiezaPto);
			Graphics2D g2 =(Graphics2D) g;
			for(Nodo nodo: nodosFlecha){
				nodo.pintar(g, zoom);
				x2 = nodo.getXptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
				y2 = nodo.getYptoEnganche(zoom, SuperficieDeEnganche.CENTRO);
				g.setColor(this.colorActual);
				g2.draw(new Line2D.Double(x1, y1, x2, y2));
				//g.drawLine(x1, y1, x2, y2);
				x1 = x2;
				y1 = y2;
			}
			
			x2 = this.termina.getXptoEnganche(zoom, terminaPto);
			y2 = this.termina.getYptoEnganche(zoom, terminaPto);
			
			g2.draw(new Line2D.Double(x1, y1, x2, y2));
			//g.drawLine(x1, y1, x2, y2);
			
			pintarCabezaFlecha(g, zoom, x1, y1, x2, y2);
		}
	}

	private void pintarCabezaFlecha(Graphics g, double zoom, int x1, int y1, int x2, int y2) {
		double magnitud = (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
		double x3, y3, x4, y4;
		double dx, dy;
		double theta, rho;
		magnitud = Math.sqrt(magnitud);
		if(magnitud > anchoFlecha * zoom * 2.0 ){
			dx = x2 - x1;
			dy = y2 - y1;
			theta = Math.atan2(dy, dx);
			rho = theta + this.phi;
			x3 = x2 - this.anchoFlecha * zoom * Math.cos(rho);
			y3 = y2 - this.anchoFlecha * zoom * Math.sin(rho);
			rho = theta - this.phi;
			x4 = x2 - this.anchoFlecha * zoom * Math.cos(rho);
			y4 = y2 - this.anchoFlecha * zoom * Math.sin(rho);
			Graphics2D g2 =(Graphics2D) g;
			g2.draw(new Line2D.Double(x2, y2, x3, y3));
			g2.draw(new Line2D.Double(x2, y2, x4, y4));
		}
	}
	
	
	
}
