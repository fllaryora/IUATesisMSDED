package com.example.botqueueweb;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.Map;

import org.bson.types.ObjectId;
import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.business.ProjectBussines;
import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.input.Combi;
import com.example.botqueueweb.dto.input.Function;
import com.example.botqueueweb.dto.input.Normal;
import com.example.botqueueweb.dto.input.Queue;
import com.example.botqueueweb.windows.CombiWindow;
import com.example.botqueueweb.windows.CounterWindow;
import com.example.botqueueweb.windows.FunctionWindow;
import com.example.botqueueweb.windows.NormalWindow;
import com.example.botqueueweb.windows.QueueWindow;
import com.mongodb.DBObject;
import com.mongodb.util.JSON;
import com.mongodb.util.JSONParseException;
import com.vaadin.data.Item;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.server.ThemeResource;
import com.vaadin.ui.Button;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;
import com.vaadin.ui.Alignment;
//import com.vaadin.ui.Button.ClickListener;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Image;
import com.vaadin.ui.Label;
import com.vaadin.ui.VerticalLayout;

public class Precursor extends VerticalLayout implements View {

    private static final long serialVersionUID = 1L;

    Object editableId = null;

    private AppletIntegration applet;
    
    //QueueContainer data;

    ObjectId idProject;
    
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
                
                if (variables.containsKey("editQueue")) { 
                    String jsonNode = (String) variables.get("editQueue");            
        			DBObject dbObject = null;
        			try{	
        				dbObject = (DBObject) JSON.parse(jsonNode);
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
                    QueueWindow qWindow = new QueueWindow(dbObject,applet,true);
                    qWindow.setHeight("610px");
                    qWindow.setWidth("450px");
                	getUI().addWindow(qWindow);
                }
                
                if (variables.containsKey("editCombi")) {                    
                    String jsonNode = (String) variables.get("editCombi");
                    //jsonNode = jsonNode.replace('*', '+');
                    //jsonNode = jsonNode.replaceAll("***", "+");
        			DBObject dbObject = null;
        			DBObject dbObject2 = null;
        			try{
        				String jsons[];
        				//jsons = jsonNode.split("+");
        				jsons = jsonNode.split("\\*\\*\\*");
        				if (jsons.length>1)
        				{
	        				dbObject = (DBObject) JSON.parse(jsons[0]);
	        				dbObject2 = (DBObject) JSON.parse(jsons[1]);
        				}
        				else
        				{
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
        			try{	
        				dbObject = (DBObject) JSON.parse(jsonNode);
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
        			FunctionWindow fWindow = new FunctionWindow(dbObject,applet,true);
        			fWindow.setHeight("610px");
        			fWindow.setWidth("450px");
                	getUI().addWindow(fWindow);
                }
                
                if (variables.containsKey("editNormal")) {                    
                    String jsonNode = (String) variables.get("editNormal");            
        			DBObject dbObject = null;
        			try{	
        				dbObject = (DBObject) JSON.parse(jsonNode); //TODO: parsear segundo json de prob branch
        			}catch(JSONParseException exception){
        				exception.printStackTrace();
        			}
        			
        			NormalWindow nWindow = new NormalWindow(dbObject,applet,true);
        			nWindow.setHeight("610px");
        			nWindow.setWidth("450px");
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
        			cWindow.setHeight("610px");
        			cWindow.setWidth("450px");
                	getUI().addWindow(cWindow);
                }
            }
	        
	    };
	    
    	setSpacing(false);
    	
        //setSizeFull();
        //addStyleName("transactions");
    	
    	idProject = (ObjectId) event.getNavigator().getUI().getData();
    	ProjectBussines projectBussines = new ProjectBussines(); //TODO: hacer singleton
    	Project project = projectBussines.getProject(idProject);
    	
    	showTop(project);
    	showBotonera();
	    addComponent(applet);
	    
    }
    
    void showTop(Project project) //, VerticalLayout vlPanel)
    {
    	HorizontalLayout top = new HorizontalLayout();
    	top.setWidth("100%");
    	//top.setHeight("30px");
    	top.setSpacing(true);
    	top.addStyleName("toolbar");
    	//addComponent(top);
    	
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
        //top.setHeight("25px");
        
    	top.setComponentAlignment(lTicket, Alignment.MIDDLE_CENTER);
    	top.setComponentAlignment(lValue, Alignment.MIDDLE_CENTER);
    	//vlPanel.addComponent(top);
    	//vlPanel.setComponentAlignment(top, Alignment.MIDDLE_RIGHT);
    	addComponent(top);
    	setComponentAlignment(top, Alignment.MIDDLE_RIGHT);
    }
    
    void showBotonera() //VerticalLayout vlPanel)
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
            @Override
			public void buttonClick(ClickEvent event) {
            	QueueWindow qWindow = new QueueWindow((DBObject) JSON.parse("{}"),applet,false);
            	qWindow.setHeight("150px");
            	qWindow.setWidth("450px");
            	getUI().addWindow(qWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Combi");
		//button.setIcon(new ThemeResource("img/queue.png"));
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				CombiWindow cWindow = new CombiWindow((DBObject) JSON.parse("{}"),null,applet,false);
				cWindow.setHeight("145px");
				cWindow.setWidth("450px");
            	getUI().addWindow(cWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Función");
		//button.setIcon(new ThemeResource("img/queue.png"));
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				FunctionWindow fWindow = new FunctionWindow((DBObject) JSON.parse("{}"),applet,false);
				fWindow.setHeight("140px");
				fWindow.setWidth("450px");
            	getUI().addWindow(fWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Normal");
		//button.setIcon(new ThemeResource("img/queue.png"));
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				NormalWindow nWindow = new NormalWindow((DBObject) JSON.parse("{}"),applet,false);
				nWindow.setHeight("140px");
				nWindow.setWidth("450px");
            	getUI().addWindow(nWindow);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Counter");
		//button.setIcon(new ThemeResource("img/queue.png"));
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				CounterWindow cWindow = new CounterWindow((DBObject) JSON.parse("{}"),applet,false);
				cWindow.setHeight("140px");
				cWindow.setWidth("450px");
            	getUI().addWindow(cWindow);
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
    }

}