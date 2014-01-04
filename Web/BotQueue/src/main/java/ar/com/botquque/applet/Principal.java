package ar.com.botquque.applet;

import ar.com.botquque.applet.enums.NodeTypes;
import ar.com.botquque.applet.graphic.GraphicDTO;
import java.awt.*;

public class Principal extends AbstractVaadinApplet {
	
	private static final long serialVersionUID = 2L;
	private EventHandler nodeWorld;
	private GraphicDTO graphic;
	/**
	 * Se llama despues de que se procesa el tag html
	 */
	public void init(){
		super.init();
		this.setFocusable(true);
		graphic = new GraphicDTO();
		nodeWorld = new EventHandler(this, this.graphic);
		nodeWorld.setBounds(this.getBounds());
		this.add(nodeWorld);
		nodeWorld.setVisible(true);
		//no se puede escribir sobre algo que no tiene foco.
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
		
		//TODO provisorio, sacar cuando se tenga la interface
		runExample();
	}
	
	/**
	 * Se llama cuando el browser se cerro 
	 */
	public void destroy(){
		super.destroy();
	}
	
	public boolean createNode(int node, String label){
		NodeTypes nodeType;
		switch(node){
		case 1: nodeType = NodeTypes.COMBI; return this.graphic.createNode(nodeType, label);   
		case 2: nodeType = NodeTypes.COUNTER; return this.graphic.createNode(nodeType, label);
		case 3: nodeType = NodeTypes.QUEUE; return this.graphic.createNode(nodeType, label);
		case 4: nodeType = NodeTypes.FUNCTION; return this.graphic.createNode(nodeType, label);
		case 5: nodeType = NodeTypes.NORMAL; return this.graphic.createNode(nodeType, label);
		default: return false;
		}
		
	}
	
	public void runExample(){
		this.graphic.runExample();
		return;
	} 
	
	public boolean deleteNode(){
		return this.graphic.deleteNode();
	}
	
	
	public String getNameNode(){
		return this.graphic.getNameNode();
	}
	
	public boolean renameNode(String newLabel){
		return this.graphic.renameNode(newLabel);
	}
	
	public boolean createArrow(){
		return this.graphic.createArrow( nodeWorld.getZoom() );
	}
	
	public boolean invertArrow(){
		return this.graphic.invertArrow();
	}
	
	public boolean deleteArrow(){
		return this.graphic.deleteArrow();
	}
	
	public boolean rotateHead(){
		return this.graphic.rotateHead();
	}
	
	public boolean rotateTail(){
		return this.graphic.rotateTail();
	}
	
	public boolean deleteDot(){
		return this.graphic.deleteDot();
	}
	
	public boolean createNextDot(){
		return this.graphic.createNextDot(nodeWorld.getZoom());
	}
	
	public boolean createPreviusDot(){
		return this.graphic.createPreviusDot(nodeWorld.getZoom());
	}
	
	//TODO properties
	public void setProperties(){
		this.graphic.setProperties();
		return;
	}

	@Override
	protected void doExecute(String command, Object[] params) {
		// TODO Auto-generated method stub
		System.out.print("Ejecuto comando");
		createNode(1,"Ejemplo01");
	}
	
}
