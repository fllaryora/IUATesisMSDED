package ar.com.botquque.pruebas;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.MouseEvent;

public class Normal {
	private int posX;
	private int posY;
	private final int ANCHO = 70;
	private final int ALTO = 45;
	private String mensaje;
	private final int TAMANIO_FUENTE = 12;
	private Color colorNormal;
	private int diferenciaX;
	private int diferenciaY;
	private boolean samarrado; //esta siendo arrastrado
	
	public Normal(int posX, int posY, String mensaje) {
		super();
		this.posX = posX;
		this.posY = posY;
		this.mensaje = mensaje;
		this.colorNormal = Color.BLACK;
		this.samarrado = false;
	}

	public int getPosX() {
		return posX;
	}

//	public void setPosX(int posX) {
//		this.posX = posX;
//	}

	public int getPosY() {
		return posY;
	}

//	public void setPosY(int posY) {
//		this.posY = posY;
//	}

	public String getMensaje() {
		return mensaje;
	}

	public void setMensaje(String mensaje) {
		this.mensaje = mensaje;
	}
	
	public int getXcentral(){
		return this.posX+ (this.ANCHO/2);
	}
	
	public int getYcentral(){
		return this.posY+ (this.ALTO/2);
	}
	
	public void pintar(Graphics g, double zoom){
		g.setColor(this.colorNormal);
		g.drawRect(this.posX, this.posY, (int)(this.ANCHO * zoom),  (int)(this.ALTO * zoom));
		Font fuente;
		fuente = new Font("Arial", Font.PLAIN, (int)(this.TAMANIO_FUENTE * zoom) );
		Mensaje.putLabel(this.mensaje, fuente, this.colorNormal , g, this.posX, this.posY + (int)(this.ALTO * zoom), (int)(this.ANCHO * zoom));
	}
	
	
	public Color getColorNormal() {
		return colorNormal;
	}

	public void setColorNormal(Color colorNormal) {
		this.colorNormal = colorNormal;
	}

	public boolean isHit(MouseEvent e, double zoom){
		if (e.getButton() != MouseEvent.BUTTON1) return false;
		int x = e.getX(); 
		if(x > this.posX +(int)(this.ANCHO * zoom)) return false;
		if(x < this.posX) return false;
		int y = e.getY();
		if(y > this.posY +(int)(this.ALTO * zoom)) return false;
		if(y < this.posY) return false;
		return true;
	}
	
	public void agarrar(MouseEvent e){
		this.samarrado = true;
		int x = e.getX(); 
		int y = e.getY();
		this.diferenciaX = x - this.posX;
		this.diferenciaY = y - this.posY;
	}
	
	public boolean arrastrar(MouseEvent e, double zoom, int desdeX, int hastaX,int desdeY, int hastaY ){
		if(this.samarrado){
			System.out.println("desdeX  "+ desdeX +"hastaX "+hastaX);
			int nuevoX = e.getX()-this.diferenciaX; 
			int nuevoY = e.getY()-this.diferenciaY;
			
			if( nuevoX < desdeX) return false;
			if( nuevoY < desdeY) return false;
			
			if( nuevoX + (int)(this.ANCHO * zoom) > hastaX) return false;
			if( nuevoY +(int)(this.ALTO * zoom) > hastaY) return false;
			//recibe lista de nodos que no se mueve
			this.posX = nuevoX;
			this.posY = nuevoY;
			return true;
			
		}
		return false;
	}
	
	public void soltado(){
		this.samarrado = false;
	}
}

