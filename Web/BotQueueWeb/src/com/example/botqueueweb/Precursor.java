package com.example.botqueueweb;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.Map;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.dto.input.Combi;
import com.example.botqueueweb.dto.input.Function;
import com.example.botqueueweb.dto.input.Normal;
import com.example.botqueueweb.dto.input.Queue;
import com.example.botqueueweb.windows.QueueWindow;
import com.mongodb.DBObject;
import com.mongodb.util.JSON;
import com.mongodb.util.JSONParseException;
import com.vaadin.data.Item;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.ui.Button;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;
//import com.vaadin.ui.Button.ClickListener;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.VerticalLayout;

public class Precursor extends VerticalLayout implements View {

    private static final long serialVersionUID = 1L;

    Object editableId = null;

    private AppletIntegration applet;
    
    //QueueContainer data;

    @Override
    public void enter(ViewChangeEvent event) {
    	
		applet = new AppletIntegration() {

	        private static final long serialVersionUID = 1L;
	
	        @Override
	        public void attach() {
	        	super.attach();
	            
	            setAppletArchives(Arrays.asList(new String[]{"BotQueue.jar"}));  
	            setCodebase("/BotQueueWeb/VAADIN/applet/");     
	            setAppletClass("ar/com/botqueue/applet/Principal.class");  
	            
	            setWidth("1000px");  
	            setHeight("1000px");
	        }
	        
	        @Override
            public void changeVariables(Object source, Map variables) {
                super.changeVariables(source, variables);
                
                System.out.println("changeVariables");
                
                if (variables.containsKey("mydata")) {                    
                    String strEstadoOk = (String) variables.get("mydata");            
                    if (strEstadoOk.equals("somevalue")){;
                        System.out.println("OK");                    
                    } else {
                    	System.out.println("error");
                    	//applet.executeCommand("exit");    
                    }
                }
                
                if (variables.containsKey("editQueue")) { 
                	System.out.println("editQueue");
                    String jsonNode = (String) variables.get("editQueue");            
                    //strEstadoOk // contiene JSON
                    //TODO: pasar json a QUEUE, eliminar new Queue()
        			DBObject dbObject = null;
        			try{	
        				dbObject = (DBObject) JSON.parse(jsonNode); //TODO: Linea de applet
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
       				//queueEdit = new Queue();
        		    
        		    //Project pr = morphia.fromDBObject(Project.class, dbObject);
        		    //queue = morphia.fromDBObject(Queue.class, dbObject);
        		    
        		    //dbObject.get(key);
        		    //dbObject.get("idNode");
        		    //dbObject.get("name");
        		    //dbObject.get("resource");
        			//private List<Integer> preceders;
        			//private List<Integer> followers;
        			//dbObject.get("fixedCost");
        			//dbObject.get("variableCost");
        			
        			//System.out.println(dbObject.toString());
        			//dbObject.put("fixedCost",5.6);
        			//System.out.println(dbObject.toString());
        			
                    QueueWindow qWindow = new QueueWindow(dbObject,applet,true);
                	getUI().addWindow(qWindow);
                }
                
                /*if (variables.containsKey("editCombi")) {                    
                    String jsonNode = (String) variables.get("editCombi");            
                    //strEstadoOk // contiene JSON
                    //TODO: pasar json a QUEUE, eliminar new Queue()
                    CombiWindow cWindow = new CombiWindow(new Combi(),applet,true);
                	getUI().addWindow(cWindow);
                }
                
                if (variables.containsKey("editNormal")) {                    
                    String jsonNode = (String) variables.get("editQueue");            
                    //strEstadoOk // contiene JSON
                    //TODO: pasar json a QUEUE, eliminar new Queue()
                    NormalWindow nWindow = new NormalWindow(new Normal(),applet,true);
                	getUI().addWindow(nWindow);
                }
                
                if (variables.containsKey("editFunction")) {                    
                    String jsonNode = (String) variables.get("editFunction");            
                    //strEstadoOk // contiene JSON
                    //TODO: pasar json a QUEUE, eliminar new Queue()
                    FunctionWindow fWindow = new FunctionWindow(new Function(),applet,true);
                	getUI().addWindow(fWindow);
                }
                
                if (variables.containsKey("editCounter")) {                    
                    String jsonNode = (String) variables.get("editCounter");            
                    //strEstadoOk // contiene JSON
                    //TODO: pasar json a QUEUE, eliminar new Queue()
                    CounterWindow cWindow = new CounterWindow(new Queue(),applet,true);
                	getUI().addWindow(qWindow);
                }*/
            }
	        
	    };
	    
    	setSpacing(false);
    	
    	
        //setSizeFull();
        //addStyleName("transactions");

        HorizontalLayout hlBotonera = new HorizontalLayout();
        hlBotonera.setSpacing(true);
        hlBotonera.setHeight("40px");
        hlBotonera.setMargin(true);
        hlBotonera.setCaption("Nodos");
        
		Button button = new Button("Cola");
		button.addClickListener(new ClickListener() {
            @Override
			public void buttonClick(ClickEvent event) {
            	QueueWindow qWindow = new QueueWindow((DBObject) JSON.parse("{}"),applet,false);
            	getUI().addWindow(qWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        /*button = new Button("Combi");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				CombiWindow qWindow = new CombiWindow(new Combi(),applet,true);
            	getUI().addWindow(qWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Normal");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				NormalWindow qWindow = new NormalWindow(new Normal(),applet,true);
            	getUI().addWindow(qWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Function");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				FunctionWindow qWindow = new FunctionWindow(new Function(),applet,true);
            	getUI().addWindow(qWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Counter");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				CounterWindow qWindow = new CounterWindow(new Counter(),applet,true);
            	getUI().addWindow(qWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);*/
        
        button = new Button("Funcion");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				String[] arrayParams = new String[2];
			    arrayParams[0] = "2";
			    arrayParams[1] = "Nuevo";		
			    applet.executeCommand("createNode", arrayParams);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);

        button = new Button("Delete");
		button.addClickListener(new ClickListener() {
            @Override
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteNode");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createArrow");
		button.addClickListener(new ClickListener() {
            @Override
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("invertArrow");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("invertArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("deleteArrow");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);

        button = new Button("rotateHead");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("rotateHead");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("rotateTail");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("rotateTail");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("deleteDot");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createNextDot");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createNextDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createPreviusDot");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createPreviusDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("editNode");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("getNodeInfo");
			    System.out.println("getNodeInfo");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        addComponent(hlBotonera);
        
	    addComponent(applet);
	    
        /*Flot  grafico = new Flot();
        double points[]= { 0.6, 1.8, 2.8, 1.2};
        grafico.addSeries(points);
        addComponent(grafico);*/
       
        /* GAUGE */
        
        /*HorizontalLayout gaugeLayout = new HorizontalLayout();
        final Gauge gauge = new Gauge("gauge");
        gaugeLayout.addComponent(gauge);

        final Gauge gauge2 = new Gauge("gauge2");
        gaugeLayout.addComponent(gauge2);

        addComponent(gaugeLayout);

        Button b = new Button("Randomize!", new Button.ClickListener() {
            @Override
            public void buttonClick(ClickEvent event) {
                Random ran = new Random();
                int rand1 = ran.nextInt(100);
                gauge.setValue(rand1);

                int rand2 = ran.nextInt(100);
                gauge2.setValue(rand2);

            }
        });
        addComponent(b);*/
    }

}