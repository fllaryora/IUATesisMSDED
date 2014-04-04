package com.example.botqueueweb;

import java.util.Arrays;
import java.util.Date;
import java.util.Map;

import org.bson.types.ObjectId;
import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.construction.JsonConstruction;
import com.example.botqueueweb.dto.input.JsonInput;
import com.example.botqueueweb.facade.Facade;
import com.example.botqueueweb.windows.CombiWindow;
import com.example.botqueueweb.windows.CounterWindow;
import com.example.botqueueweb.windows.FunctionWindow;
import com.example.botqueueweb.windows.NormalWindow;
import com.example.botqueueweb.windows.ProjectFinalWindow;
import com.example.botqueueweb.windows.QueueWindow;
import com.google.code.morphia.Morphia;
import com.mongodb.DBObject;
import com.mongodb.util.JSON;
import com.mongodb.util.JSONParseException;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.server.ThemeResource;
import com.vaadin.ui.Button;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Image;
import com.vaadin.ui.Label;
import com.vaadin.ui.Panel;
import com.vaadin.ui.VerticalLayout;

public class Precursor extends VerticalLayout implements View {

    private static final long serialVersionUID = 1L;

    Object editableId = null;

    private AppletIntegration applet;
    
    //QueueContainer data;

    ObjectId idProject;
    
    @Override
    public void enter(final ViewChangeEvent event) {
    	
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
	        
	        @SuppressWarnings("unchecked")
			@Override
            public void changeVariables(Object source, @SuppressWarnings("rawtypes") Map variables) {
                super.changeVariables(source, variables);
                
                if (variables.containsKey("editQueue")) { 
                    String jsonNode = (String) variables.get("editQueue");            
        			DBObject dbObject = null;
        			try{	
        				dbObject = (DBObject) JSON.parse(jsonNode);
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
                    QueueWindow qWindow = new QueueWindow(dbObject,applet,true);
                    qWindow.setHeight("256px");//("610px"); -344
                    qWindow.setWidth("450px");
                	getUI().addWindow(qWindow);
                }
                
                if (variables.containsKey("editCombi")) {                    
                    String jsonNode = (String) variables.get("editCombi");
        			DBObject dbObject = null;
        			DBObject dbObject2 = null;
        			try{
        				String jsons[];
        				jsons = jsonNode.split("\\*\\*\\*");
        				if (jsons.length>1) {
	        				dbObject = (DBObject) JSON.parse(jsons[0]);
	        				dbObject2 = (DBObject) JSON.parse(jsons[1]);
        				}
        				else {
        					dbObject = (DBObject) JSON.parse("{}");
	        				dbObject2 = (DBObject) JSON.parse(jsons[0]);
        				}
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
        			CombiWindow cWindow = new CombiWindow(dbObject2,dbObject,applet,true);
        			cWindow.setHeight("610px");
    				cWindow.setWidth("458px");
                	getUI().addWindow(cWindow);
                }
                
                if (variables.containsKey("editFunction")) {                    
                    String jsonNode = (String) variables.get("editFunction");            
        			DBObject dbObject = null;
        			DBObject dbObject2 = null;
        			try{
        				String jsons[];
        				jsons = jsonNode.split("\\*\\*\\*");
        				if (jsons.length>1) {
	        				dbObject = (DBObject) JSON.parse(jsons[0]);
	        				dbObject2 = (DBObject) JSON.parse(jsons[1]);
        				}
        				else {
        					dbObject = (DBObject) JSON.parse("{}");
	        				dbObject2 = (DBObject) JSON.parse(jsons[0]);
        				}
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
        			FunctionWindow fWindow = new FunctionWindow(dbObject2,dbObject,applet,true);
        			fWindow.setHeight("610px");
        			fWindow.setWidth("458px");
                	getUI().addWindow(fWindow);
                }
                
                if (variables.containsKey("editNormal")) {                    
                    String jsonNode = (String) variables.get("editNormal");            
         			DBObject dbObject = null;
        			DBObject dbObject2 = null;
        			try{
        				String jsons[];
        				jsons = jsonNode.split("\\*\\*\\*");
        				if (jsons.length>1) {
	        				dbObject = (DBObject) JSON.parse(jsons[0]);
	        				dbObject2 = (DBObject) JSON.parse(jsons[1]);
        				}
        				else {
        					dbObject = (DBObject) JSON.parse("{}");
	        				dbObject2 = (DBObject) JSON.parse(jsons[0]);
        				}
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
        			NormalWindow nWindow = new NormalWindow(dbObject2,dbObject,applet,true);
        			nWindow.setHeight("610px");
        			nWindow.setWidth("458px");
                	getUI().addWindow(nWindow);
                }
                
                if (variables.containsKey("editCounter")) {                    
                    String jsonNode = (String) variables.get("editCounter");            
        			DBObject dbObject = null;
        			try{	
        				dbObject = (DBObject) JSON.parse(jsonNode);
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
        			CounterWindow cWindow = new CounterWindow(dbObject,applet,true);
        			cWindow.setHeight("216px");
        			cWindow.setWidth("450px");
                	getUI().addWindow(cWindow);
                }
                
                if (variables.containsKey("editModel")) {
                	String jsonNode = (String) variables.get("editModel");            
                	DBObject dbObject = null, dbObject2 = null;
                	int nroNodos = 0;
        			try{
        				String jsons[];
        				jsons = jsonNode.split("\\*\\*\\*");
        				dbObject = (DBObject) JSON.parse(jsons[0]);  //construction
        				dbObject2 = (DBObject) JSON.parse(jsons[1]); //pending
        				nroNodos = Integer.parseInt(jsons[2]);
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
        			Morphia morphia = new Morphia();
        			morphia.map(JsonConstruction.class);
        			JsonConstruction jsonConstruction = morphia.fromDBObject(JsonConstruction.class, dbObject);
        			
        			morphia = new Morphia();
        			morphia.map(JsonInput.class);
        		    JsonInput jsonInput = morphia.fromDBObject(JsonInput.class, dbObject2);
        		    
        		    ObjectId idProject = (ObjectId) event.getNavigator().getUI().getData();
        	    	Project project = Facade.getInstance().getProject(idProject);
        	    	project.setConstruction(jsonConstruction);
        	    	project.setInput(jsonInput);
        	    	project.setState("P");
        	    	project.setPendingStamp((new Long((new Date()).getTime())).toString());
        	    	project.setLastUpdatedStamp((new Long((new Date()).getTime())).toString());
        	    	project.setNroProcs(nroNodos);
        	    	Facade.getInstance().saveProject(project);
                }

                if (variables.containsKey("editModelConstruct")) {
                	String jsonNode = (String) variables.get("editModelConstruct");            
        			DBObject dbObject = null;
        			try{	
        				dbObject = (DBObject) JSON.parse(jsonNode);
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
        			Morphia morphia = new Morphia();
        			morphia.map(JsonConstruction.class);
        			JsonConstruction jsonConstruction = morphia.fromDBObject(JsonConstruction.class, dbObject);
        		    
        		    ObjectId idProject = (ObjectId) event.getNavigator().getUI().getData();
        	    	Project project = Facade.getInstance().getProject(idProject);
        	    	project.setConstruction(jsonConstruction);
        	    	project.setState("C");
        	    	project.setPendingStamp((new Long((new Date()).getTime())).toString());
        	    	project.setLastUpdatedStamp((new Long((new Date()).getTime())).toString());
        	    	
        	    	Facade.getInstance().saveProject(project);
                }
            }
	        
	    };
	    
    	setMargin(true);
    	
    	Panel bodyPanel = new Panel();
    	bodyPanel.setWidth("100%");
    	bodyPanel.setHeight("100%");
    	VerticalLayout vlPanel = new VerticalLayout();
    	
        setSizeFull();
        addStyleName("transactions");

    	idProject = (ObjectId) event.getNavigator().getUI().getData();
    	Project project = Facade.getInstance().getProject(idProject);
    	
    	showTop(project,vlPanel);
    	showBotonera(vlPanel);
	    vlPanel.addComponent(applet);
    	vlPanel.setComponentAlignment(applet, Alignment.MIDDLE_LEFT);
    	showBotoneraDown(vlPanel);
	    
	    bodyPanel.setContent(vlPanel);
        addComponent(bodyPanel);
        
        if(project.getConstruction()!=null)
        {
	        String[] arrayParams = new String[1];
	        Morphia morphia = new Morphia();
			morphia.map(JsonConstruction.class);
	    	arrayParams[0] = morphia.toDBObject(project.getConstruction()).toString();
		    applet.executeCommand("setAllModelFile",arrayParams);
        }
    }
    
    void showTop(Project project, VerticalLayout vlPanel)
    {
    	HorizontalLayout top = new HorizontalLayout();
    	top.setWidth("100%");
    	top.setSpacing(true);
    	top.addStyleName("toolbar");
    	
    	Label lTicket =  new Label("Proyecto: ");
        lTicket.addStyleName("ticket");
        lTicket.setSizeUndefined();
        Label lValue = new Label(project.getName());
        lValue.setSizeUndefined();
        
        ThemeResource imgBot = new ThemeResource("img/bot.png");
    	Image image = new Image(null,imgBot);
    	
        top.addComponent(image);
        top.addComponent(lTicket);
        top.addComponent(lValue);
        top.addComponent(new Label(" "));
        top.setSizeUndefined();
        
    	top.setComponentAlignment(lTicket, Alignment.MIDDLE_CENTER);
    	top.setComponentAlignment(lValue, Alignment.MIDDLE_CENTER);
    	vlPanel.addComponent(top);
    	vlPanel.setComponentAlignment(top, Alignment.MIDDLE_RIGHT);
    }
    
    void showBotoneraDown(VerticalLayout vlPanel)
    {
    	HorizontalLayout hlBotonera = new HorizontalLayout();
        hlBotonera.setSpacing(true);
        hlBotonera.setHeight("40px");
        hlBotonera.setMargin(true);
	    
        Button button = new Button("Guardar");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
				String[] arrayParams = new String[2];
            	arrayParams[0] = "0";
            	arrayParams[1] = "0";	            	
			    applet.executeCommand("getAllModelFile",arrayParams);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Guardar y Ejecutar");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
				ProjectFinalWindow pfWindow = new ProjectFinalWindow(applet);
				pfWindow.setHeight("200px");
				pfWindow.setWidth("450px");
            	getUI().addWindow(pfWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        //addComponent(hlBotonera);
        vlPanel.addComponent(hlBotonera);
    	vlPanel.setComponentAlignment(hlBotonera, Alignment.MIDDLE_RIGHT);
    }
    
    void showBotonera(VerticalLayout vlPanel) //VerticalLayout vlPanel)
    {
        HorizontalLayout hlBotonera = new HorizontalLayout();
        hlBotonera.setSpacing(true);
        hlBotonera.setHeight("40px");
        hlBotonera.setMargin(true);
        
		Button button = new Button("Cola");
		//ThemeResource imgCombi = new ThemeResource("img/queue.png");
    	//Image image = new Image(null,imgCombi);
		//button.setIcon(imgCombi);
		button.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
			@Override
			public void buttonClick(ClickEvent event) {
            	QueueWindow qWindow = new QueueWindow((DBObject) JSON.parse("{}"),applet,false);
            	qWindow.setHeight("136px"); //("150px"); -14
            	qWindow.setWidth("450px");
            	getUI().addWindow(qWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Combi");
		//button.setIcon(new ThemeResource("img/queue.png"));
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
				CombiWindow cWindow = new CombiWindow((DBObject) JSON.parse("{}"),null,applet,false);
				cWindow.setHeight("136px");//.setHeight("145px"); -9
				cWindow.setWidth("458px");
            	getUI().addWindow(cWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Función");
		//button.setIcon(new ThemeResource("img/queue.png"));
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
				FunctionWindow fWindow = new FunctionWindow((DBObject) JSON.parse("{}"),null,applet,false);
				fWindow.setHeight("136px");//("140px"); -4
				fWindow.setWidth("458px");
            	getUI().addWindow(fWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Normal");
		//button.setIcon(new ThemeResource("img/queue.png"));
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
				NormalWindow nWindow = new NormalWindow((DBObject) JSON.parse("{}"),null,applet,false);
				nWindow.setHeight("136px");//("140px"); -4
				nWindow.setWidth("458px");
            	getUI().addWindow(nWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Counter");
		//button.setIcon(new ThemeResource("img/queue.png"));
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
				CounterWindow cWindow = new CounterWindow((DBObject) JSON.parse("{}"),applet,false);
				cWindow.setHeight("136px");//("140px"); -4
				cWindow.setWidth("450px");
            	getUI().addWindow(cWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Delete");
		button.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteNode");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createArrow");
		button.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("invertArrow");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("invertArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("deleteArrow");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);

        button = new Button("rotateHead");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("rotateHead");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("rotateTail");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("rotateTail");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("deleteDot");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createNextDot");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createNextDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createPreviusDot");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createPreviusDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("editNode");
		button.addClickListener(new Button.ClickListener() {
			private static final long serialVersionUID = 1L;
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("getNodeInfo");
			    System.out.println("getNodeInfo");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        //addComponent(hlBotonera);
        vlPanel.addComponent(hlBotonera);
    	vlPanel.setComponentAlignment(hlBotonera, Alignment.MIDDLE_LEFT);
    }

}