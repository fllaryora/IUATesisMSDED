
package ar.com.botqueue.applet;

import ar.com.botqueue.applet.graphic.GraphicDTO;

import java.awt.*;

public class Principal extends AbstractVaadinApplet {
	
	private static final long serialVersionUID = 2L;
	private EventHandler nodeWorld;
	private GraphicDTO graphic;
	private VaadinFacade facade;
	/**
	 * Se llama despues de que se procesa el tag html
	 */
	public void init(){
		super.init();
		this.setFocusable(true);
		
		graphic = new GraphicDTO();
		nodeWorld = new EventHandler(this, this.graphic);
		nodeWorld.setBounds(this.getBounds());
		//this.setSize(800, 600);
		
		this.add(nodeWorld);
		nodeWorld.setVisible(true);
		//no se puede escribir sobre algo que no tiene foco.
		
		facade = new VaadinFacade(graphic, nodeWorld, this);
	}
	
	/**
	 * Se llama despues de que el usuario cierra el navegador
	 */
	public void stop(){
		super.stop();
	}
	
	/**
	 * Se llama despues de init o despues de stop 
	 * si ya estaba precargada la pagina
	 */
	public void start(){
		super.start();
		this.setBackground(Color.WHITE);
		/* Lo que no aclaro el color se pone en rojo
		 * asi me doy cuenta que no estoy pensando bien*/
		this.setForeground(Color.RED);
		this.nodeWorld.forcePaint();
		runSimpleTest();
	}
	
	/**
	 * Se llama cuando el browser se cerro 
	 */
	public void destroy(){
		super.destroy();
	}
	
	@Override
	protected void doExecute(String command, Object[] params) {
		this.facade.doExecute(command, params);
	}
	
	@Deprecated
	private void runSimpleTest(){
		this.graphic.testFastExample();
		Object[] params = new Object[18]; 
		String probabilistics = "3,0.9,5,0.1";
		params[17]= probabilistics;
		params[0]= "deterministic";
		params[4]= "10.0";
		params[15]= "yes";
		params[16]= "Amazar";
		doExecute("editNode", params);
		System.out.println( this.graphic.getModelInfo(this, 600 , -1) );
		System.out.println( this.graphic.getAllModelFile(this, 600 , -1) );
	}
	
}
