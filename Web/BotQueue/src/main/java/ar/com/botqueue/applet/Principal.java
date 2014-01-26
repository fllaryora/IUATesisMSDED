
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
		//this.setSize(800, 600);
		
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
		this.graphic.testFastExample(this);
		this.nodeWorld.forcePaint();
	}
	
	/**
	 * Se llama cuando el browser se cerro 
	 */
	public void destroy(){
		super.destroy();
	}
	
	/**
	 * Crea un nodo en el modelo 
	 */
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
	
	
	/**
	 * Elimina un nodo seleccionado en el modelo 
	 */
	public boolean deleteNode(){
		boolean ret; 
		ret = this.graphic.deleteNode();
		this.nodeWorld.forcePaint();
		return ret ;
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

	@Override
	protected void doExecute(String command, Object[] params) {
		
		if (command.equalsIgnoreCase("createNode"))
        	createNode(Integer.parseInt((String)params[0]), (String)params[1]);
		else if (command.equalsIgnoreCase("deleteNode"))
            deleteNode();
        else if (command.equalsIgnoreCase("createArrow")) 	
   		 	createArrow();
        else if (command.equalsIgnoreCase("invertArrow"))
	   		invertArrow();
        else if (command.equalsIgnoreCase("deleteArrow"))
        	deleteArrow();
        else if (command.equalsIgnoreCase("rotateHead"))
   		 	rotateHead();
        else if (command.equalsIgnoreCase("rotateTail"))
   		 	rotateTail();
        else if (command.equalsIgnoreCase("deleteDot"))
   		 	deleteDot();
        else if (command.equalsIgnoreCase("createNextDot"))
   		 	createNextDot();
        else if (command.equalsIgnoreCase("createPreviusDot"))
   		 	createPreviusDot();
        else if (command.equalsIgnoreCase("getNodeInfo"))
        	getNodeInfo();
        else if (command.equalsIgnoreCase("editNode"))
        	editNode(params);
        else if (command.equalsIgnoreCase("getModelInfo"))//length y modelSeed
        	getModelInfo(params);
        else if (command.equalsIgnoreCase("getModelConstructInfo"))//length y modelSeed
		getModelConstructInfo( params);
	}
	
	/** 
	 * get all data of some node
	 * */
	public void getNodeInfo(){
		this.graphic.getNodeInfo(this);
		return ;
		
	}
	
	/** 
	 * set all data of some node
	 * */
	public void editNode(Object[] params){
		this.graphic.editNode(params);
		return ;
		
	}
	
	public void getModelInfo(Object[] params){
		this.graphic.getModelInfo(this, params);
		return ;
	}
	
	public void getModelConstructInfo(Object[] params){
		this.graphic.getModelConstructInfo(this, params);
		return ;
	}
	
}
