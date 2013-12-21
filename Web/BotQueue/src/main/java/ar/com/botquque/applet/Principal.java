package ar.com.botquque.applet;

import javax.swing.JApplet;
import java.awt.*;

public class Principal extends JApplet {
	
	private static final long serialVersionUID = 2L;
	
	/**
	 * Se llama despues de que se procesa el tag html
	 */
	public void init(){
		this.setFocusable(true);
		ManejadoraEvento me = new ManejadoraEvento(this);
		me.setBounds(0,0, 400,400);
		this.add(me);
		me.setVisible(true);
		//no se puede escribir sobre algo que no tiene foco.
	
		//this.requestFocusInWindow();
	}
	
	/**
	 * Se llama despues de que el usuario cierra el navegador
	 */
	public void stop(){
		
	}
	
	/**
	 * Se llama despues de init o despues de stop 
	 * si ya estaba precargada la pagina
	 */
	public void start(){
		this.setBackground(Color.WHITE);
		/* Lo que no aclaro el color se pone en rojo
		 * asi me doy cuenta que no estoy pensando bien*/
		this.setForeground(Color.RED);
	}
	
	/**
	 * Se llama cuando el browser se cerro 
	 */
	public void destroy(){
		
	}
	
}
