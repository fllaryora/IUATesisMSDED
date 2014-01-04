package ar.com.botqueue.applet;

import ar.com.botqueue.applet.enums.NodeTypes;
import ar.com.botqueue.applet.graphic.GraphicDTO;

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
		this.nodeWorld.forcePaint();
	}
	
	/**
	 * Se llama cuando el browser se cerro 
	 */
	public void destroy(){
		super.destroy();
	}
	
	public boolean createNode(int node, String label){
		NodeTypes nodeType;
		boolean ret; 
		switch(node){
		case 1:
			nodeType = NodeTypes.COMBI;
			ret = this.graphic.createNode(nodeType, label);
			this.nodeWorld.forcePaint();
			return ret ; 
		case 2:
			nodeType = NodeTypes.COUNTER;
			ret = this.graphic.createNode(nodeType, label);
			this.nodeWorld.forcePaint();
			return ret ;
		case 3:
			nodeType = NodeTypes.QUEUE;
			ret = this.graphic.createNode(nodeType, label);
			this.nodeWorld.forcePaint();
			return ret ;
		case 4: 
			nodeType = NodeTypes.FUNCTION;
			ret = this.graphic.createNode(nodeType, label);
			this.nodeWorld.forcePaint();
			return ret ;
		case 5: 
			nodeType = NodeTypes.NORMAL;
			ret = this.graphic.createNode(nodeType, label);
			this.nodeWorld.forcePaint();
			return ret ;
		default: return false;
		}
		
	}
	
	public void runExample(){
		this.graphic.runExample();
		return;
	} 
	
	public boolean deleteNode(){
		boolean ret; 
		ret = this.graphic.deleteNode();
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	
	public String getNameNode(){
		return this.graphic.getNameNode();
	}
	
	public boolean renameNode(String newLabel){
		boolean ret; 
		ret = this.graphic.renameNode(newLabel);
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	public boolean createArrow(){
		boolean ret; 
		ret = this.graphic.createArrow( nodeWorld.getZoom() );
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	public boolean invertArrow(){
		boolean ret; 
		ret = this.graphic.invertArrow();
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	public boolean deleteArrow(){
		boolean ret; 
		ret = this.graphic.deleteArrow();
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	public boolean rotateHead(){
		boolean ret; 
		ret = this.graphic.rotateHead();
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	public boolean rotateTail(){
		boolean ret; 
		ret = this.graphic.rotateTail();
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	public boolean deleteDot(){
		boolean ret; 
		ret = this.graphic.deleteDot();
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	public boolean createNextDot(){
		boolean ret; 
		ret = this.graphic.createNextDot(nodeWorld.getZoom());
		this.nodeWorld.forcePaint();
		return ret ;
	}
	
	public boolean createPreviusDot(){
		boolean ret; 
		ret = this.graphic.createPreviusDot(nodeWorld.getZoom());
		this.nodeWorld.forcePaint();
		return ret ;
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
		vaadinUpdateVariable("mydata", "somevalue", true);
	}
	
}
