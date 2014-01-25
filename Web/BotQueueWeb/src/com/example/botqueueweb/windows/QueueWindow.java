package com.example.botqueueweb.windows;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.dto.input.Queue;
import com.mongodb.DBObject;
import com.vaadin.ui.Button;
import com.vaadin.ui.Component;
import com.vaadin.ui.CssLayout;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.TextField;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;

public class QueueWindow extends Window {

    public QueueWindow(final DBObject queue, final AppletIntegration applet, boolean isFull) {
    	
    	this.setCaption("Queue");
    	this.setModal(true);
    	//subWindow.setStyleName("body-style");
    	setResizable(false);
        VerticalLayout subContent = new VerticalLayout();
        subContent.setMargin(true);
        subContent.setSpacing(true);
        setContent(subContent);
        
        // idNode:5
		//resource: 5
		//"preceders": [5,8,11,13,15,17],
		//"followers": [5,8,11,13,15,17],
		//fixedCost": 0.0,
		//variableCost": 0.0
		
        //dbObject.get("idNode");
	    //dbObject.get("name");
	    //dbObject.get("resource");
		//private List<Integer> preceders;
		//private List<Integer> followers;
		//dbObject.get("fixedCost");
		//dbObject.get("variableCost");
        
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label("Nombre: ");
        lNombre.setWidth("90");
        final TextField tfName = new TextField();
        if (queue.get("name")!=null)
        	tfName.setValue(queue.get("name").toString());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);
        
        HorizontalLayout hlResource = new HorizontalLayout();
        Label lResource = new Label("Recursos: ");
        lResource.setWidth("90");
        TextField tfResource = new TextField();
        if (queue.get("resource")!=null)
        	tfResource.setValue(queue.get("resource").toString());
        hlResource.addComponent(lResource);
	    hlResource.addComponent(tfResource);
	    if (isFull)
	        subContent.addComponent(hlResource);

        
        HorizontalLayout hlFixedCost = new HorizontalLayout();
        Label lFixedCost = new Label("Costo Fijo: ");
        lFixedCost.setWidth("90");
        TextField tfFixedCost = new TextField();
        if (queue.get("fixedCost")!=null)
        	tfFixedCost.setValue(queue.get("fixedCost").toString());
        hlFixedCost.addComponent(lFixedCost);
        hlFixedCost.addComponent(tfFixedCost);
	    if (isFull)
	    	subContent.addComponent(hlFixedCost);
        
        HorizontalLayout hlVariableCost = new HorizontalLayout();
        Label lVariableCost = new Label("Costo Variable: ");
        lVariableCost.setWidth("90");
        TextField tfVariableCost = new TextField();
        if (queue.get("variableCost")!=null)
        	tfVariableCost.setValue(queue.get("variableCost").toString());
        hlVariableCost.addComponent(lVariableCost);
        hlVariableCost.addComponent(tfVariableCost);
	    if (isFull)
	    	subContent.addComponent(hlVariableCost);

        
        //BOTONES
        
        HorizontalLayout hlBotones = new HorizontalLayout();
        hlBotones.setSpacing(true);
        
        Button bAceptar = new Button("Aceptar");
        
        if(isFull)
        {
        	//applet.executeCommand("editNode", arrayParams);
        	//TODO: con json completo de Queue, ver si lo pasa con BSON
        	bAceptar.addClickListener(new ClickListener() {
	            @Override
				public void buttonClick(ClickEvent event) {
	            	String[] arrayParams = new String[2];
	            	queue.put("name", tfName.getValue());
	            	queue.put("resource", Integer.parseInt(tfName.getValue()));
	    	        queue.put("fixedCost", Double.parseDouble(tfName.getValue()));
	    	        queue.put("variableCost", Double.parseDouble(tfName.getValue()));
	        	    arrayParams[0] = queue.toString();
	        	    
	        	    /*String[] arrayParams = new String[4];
	        	    arrayParams[0] = tfName.getValue();
	        	    arrayParams[1] = tfResource.getValue();
	        	    arrayParams[2] = tfFixedCost.getValue();
	        	    arrayParams[3] = tfVariableCost.getValue();*/
	        	    
	        	    applet.executeCommand("editNode", arrayParams);
	        	    close();
				}
			});
        }
        else
        {
	        bAceptar.addClickListener(new ClickListener() {
	            @Override
				public void buttonClick(ClickEvent event) {
	            	String[] arrayParams = new String[2];
	        	    arrayParams[0] = "3";
	        	    arrayParams[1] = tfName.getValue();		
	        	    applet.executeCommand("createNode", arrayParams);
	        	    close();
				}
			});
        }
        
        Button bCancelar = new Button("Cancelar");
        bCancelar.addClickListener(new ClickListener() {
            @Override
			public void buttonClick(ClickEvent event) {
        	    close();
			}
		});
        
        bAceptar.setWidth("100");
        bCancelar.setWidth("100");
        hlBotones.addComponent(bAceptar);
        hlBotones.addComponent(bCancelar);
        subContent.addComponent(hlBotones);
        
        center();
        
        //setContent(new CssLayout());
        //setPrimaryStyleName("help-overlay");
        //setDraggable(false);
        //setResizable(false);
    }

    public void addComponent(Component c) {
        ((CssLayout) getContent()).addComponent(c);
    }

}