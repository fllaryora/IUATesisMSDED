
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
		//this.setBackground(Color.WHITE);
		this.setBackground(Color.BLUE);
		/* Lo que no aclaro el color se pone en rojo
		 * asi me doy cuenta que no estoy pensando bien*/
		this.setForeground(Color.RED);
		this.nodeWorld.forcePaint();
		//runSimpleTest();
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
		Object[] params = new Object[1];
		params[0]= "{ \"length\":600, \"seed\":-1, "
				+ "\"transformation\": {"
				+ "\"queues\":[  {\"idNode\":5,\"name\":\"Panadero\",\"posX\":110,\"posY\":30,\"resource\":0,\"fixedCost\":0.0,\"variableCost\":0.0,\"preceders\": [  4],\"followers\": [  4]} ],"
				+ "\"combis\":[  {\"idNode\":4,\"name\":\"Amazar\",\"posX\":96,\"posY\":132,\"probBranch\":true,\"delay\" : {\"distribution\":\"deterministic\",\"constant\":10.0},\"preceders\": [  5],\"followers\": [  3,5],\"probabilisticBranch\": [  0.9,0.1]} ],"
				+ "\"normals\":[  {\"idNode\":3,\"name\":\"Cocinar\",\"posX\":240,\"posY\":132,\"probBranch\":false,\"delay\" : {\"distribution\":\"deterministic\",\"constant\":0.0},\"preceders\": [  4],\"followers\": [  2]} ],"
				+ "\"functions\":[ {\"idNode\":2,\"name\":\"Cortar\",\"posX\":384,\"posY\":132,\"probBranch\":false,\"input\":0,\"output\":0,\"preceders\": [  3],\"followers\": [  1]} ],"
				+ "\"counters\":[  {\"idNode\":1,\"name\":\"Servir\",\"posX\":528,\"posY\":132,\"quantity\":0,\"cycle\":0,\"preceders\": [  2],\"followers\": [ ]} ]}, "
				+ "\"arrows\": [{\"tail\":2,\"head\":1,\"tailSurface\":2,\"headSurface\":6,\"enableProb\":false,\"probabilisticBranch\":0.0,\"middlePoints\": [{\"posX\":478,\"posY\":154,\"order\":0} ]} ,"
				+ "{\"tail\":3,\"head\":2,\"tailSurface\":2,\"headSurface\":6,\"enableProb\":false,\"probabilisticBranch\":0.0,\"middlePoints\": [{\"posX\":347,\"posY\":154,\"order\":0} ]} ,"
				+ "{\"tail\":4,\"head\":3,\"tailSurface\":2,\"headSurface\":6,\"enableProb\":true,\"probabilisticBranch\":0.9,\"middlePoints\": [{\"posX\":203,\"posY\":154,\"order\":0} ]} ,"
				+ "{\"tail\":5,\"head\":4,\"tailSurface\":5,\"headSurface\":7,\"enableProb\":false,\"probabilisticBranch\":0.0,\"middlePoints\": [{\"posX\":125,\"posY\":103,\"order\":0} ]} ,"
				+ "{\"tail\":4,\"head\":5,\"tailSurface\":1,\"headSurface\":3,\"enableProb\":true,\"probabilisticBranch\":0.1,\"middlePoints\": [{\"posX\":138,\"posY\":103,\"order\":0} ]} ] }";
				
		
		/*params[0]= "{\"length\":600,\"seed\":-1,\"transformation\":{"
					+ "\"queues\":[{\"idNode\":1,\"name\":\"Panadero\",\"posX\":110,\"posY\":30,\"resource\":0,\"fixedCost\":0.0,\"variableCost\":0.0,\"preceders\":[2],\"followers\":[2]}],"
					+ "\"combis\":[{\"idNode\":2,\"name\":\"Amazar\",\"posX\":96,\"posY\":132,\"probBranch\":false,\"delay\":{\"distribution\":\"deterministic\",\"constant\":0.0},\"preceders\":[1],\"followers\":[1,3]}],"
					+ "\"normals\":[{\"idNode\":3,\"name\":\"Cocinar\",\"posX\":240,\"posY\":132,\"probBranch\":false,\"delay\":{\"distribution\":\"deterministic\",\"constant\":0.0},\"preceders\":[2],\"followers\":[5]}],"
					+ "\"functions\":[{\"idNode\":5,\"name\":\"Cortar\",\"posX\":384,\"posY\":132,\"probBranch\":false,\"input\":0,\"output\":0,\"preceders\":[3],\"followers\":[4]}],"
					+ "\"counters\":[{\"idNode\":4,\"name\":\"Servir\",\"posX\":528,\"posY\":132,\"quantity\":0,\"cycle\":0,\"preceders\":[5],\"followers\":[]}]},"
				+ "\"arrows\":["
					+ "{\"tail\":1,\"head\":2,\"tailSurface\":5,\"headSurface\":7,\"enableProb\":false,\"probabilisticBranch\":0.0,\"middlePoints\":[{\"posX\":125,\"posY\":103,\"order\":0}]},"
					+ "{\"tail\":2,\"head\":1,\"tailSurface\":1,\"headSurface\":3,\"enableProb\":false,\"probabilisticBranch\":0.0,\"middlePoints\":[{\"posX\":138,\"posY\":103,\"order\":0}]},"
					+ "{\"tail\":2,\"head\":3,\"tailSurface\":2,\"headSurface\":6,\"enableProb\":false,\"probabilisticBranch\":0.0,\"middlePoints\":[{\"posX\":203,\"posY\":154,\"order\":0}]},"
					+ "{\"tail\":5,\"head\":4,\"tailSurface\":2,\"headSurface\":6,\"enableProb\":false,\"probabilisticBranch\":0.0,\"middlePoints\":[{\"posX\":478,\"posY\":154,\"order\":0}]},"
					+ "{\"tail\":3,\"head\":5,\"tailSurface\":2,\"headSurface\":6,\"enableProb\":false,\"probabilisticBranch\":0.0,\"middlePoints\":[{\"posX\":347,\"posY\":154,\"order\":0}]}]}";
		*/
		doExecute("setAllModelFile", params);
		
		this.graphic.testFastExample();
		/*Object[] params = new Object[18]; 
		String probabilistics = "3,0.9,5,0.1";
		params[17]= probabilistics;
		params[0]= "deterministic";
		params[4]= "10.0";
		params[15]= "yes";
		params[16]= "Amazar";
		doExecute("editNode", params);*/
		
		doExecute("createArrow", params);
		
		//System.out.println( this.graphic.getModelInfo(this, 600 , -1) );
		//System.out.println( this.graphic.getAllModelFile(this, 600 , -1) );
	}
	
}
