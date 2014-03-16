package com.example.botqueueweb.windows;

import java.util.List;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.dto.input.Combi;
import com.mongodb.BasicDBObject;
import com.mongodb.DBObject;
import com.mongodb.util.JSON;
import com.vaadin.data.Property;
import com.vaadin.data.Property.ValueChangeEvent;
import com.vaadin.ui.AbsoluteLayout;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
import com.vaadin.ui.ComboBox;
import com.vaadin.ui.Component;
import com.vaadin.ui.CssLayout;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.Panel;
import com.vaadin.ui.TextField;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;

public class FunctionWindow extends Window {

    public FunctionWindow(final DBObject function,DBObject dbProbBranch, final AppletIntegration applet, boolean isFull) {
    	
    	this.setCaption("Función");
    	this.setModal(true);
    	
    	Panel bodyPanel = new Panel();
    	bodyPanel.setWidth("100%");
    	bodyPanel.setHeight("100%");
    	
    	setResizable(false);
        VerticalLayout subContent = new VerticalLayout();
        subContent.setMargin(true);
        subContent.setSpacing(true);
        
        bodyPanel.setContent(subContent);
        setContent(bodyPanel);
        
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label("Nombre: ");
        if (isFull)	lNombre.setWidth("110");
        else		lNombre.setWidth("90");
        final TextField tfName = new TextField();
        if (isFull)	tfName.setWidth("310");
        else		tfName.setWidth("330");
        if (function.get("name")!=null)
        	tfName.setValue(function.get("name").toString());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);

     // Elementos Modificacion //
    	
    	 
    	
    	final HorizontalLayout hlInput = new HorizontalLayout();
        Label lMinimun = new Label("Input: ");
        lMinimun.setWidth("110");
        final TextField tfInput = new TextField();
        if (function.get("input")!=null)
        	tfInput.setValue(function.get("input").toString());
        hlInput.addComponent(lMinimun);
        hlInput.addComponent(tfInput);
        //hlInput.setVisible(false);
        
        final HorizontalLayout hlOutput = new HorizontalLayout();
        Label lMaximun = new Label("Output: ");
        lMaximun.setWidth("110");
        final TextField tfOutput = new TextField();
        if (function.get("output")!=null)
        	tfOutput.setValue(function.get("output").toString());
        hlOutput.addComponent(lMaximun);
        hlOutput.addComponent(tfOutput);
        //hlOutput.setVisible(false);
        
        //TODO: private List<Double> probabilisticBranch;
        
        if (isFull)
        {
	        final HorizontalLayout hlSpace = new HorizontalLayout();
	        hlSpace.setHeight("370px");
	        
	        subContent.addComponent(hlInput);
	        subContent.addComponent(hlOutput);
	        //subContent.addComponent(hlSeed);
	        subContent.addComponent(hlSpace);
        }
        
        
        //BOTONES
        
	    //AbsoluteLayout alBotones = new AbsoluteLayout();
	    HorizontalLayout hlBotones = new HorizontalLayout();
	    hlBotones.setSpacing(true);
	    hlBotones.setHeight("100%");
	    
        Button bAceptar = new Button("Aceptar");
        
        if(isFull)
        {
        	//TODO: applet.executeCommand("editNode", arrayParams);
        	//con json completo de Queue
        	bAceptar.addClickListener(new ClickListener() {
	            @Override
				public void buttonClick(ClickEvent event) {
	            	String[] arrayParams = new String[5];
	            	arrayParams[0] = tfInput.getValue().toString();
	            	arrayParams[1] = tfOutput.getValue();
	            	arrayParams[2] = "no"; //"yes" //prob bhanch
	            	arrayParams[3] = tfName.getValue();	            	
	            	arrayParams[4] = ""; 
	            	
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
	        	    arrayParams[0] = "4";
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
        subContent.setComponentAlignment(hlBotones, Alignment.BOTTOM_RIGHT);
        center();
    }

    public void addComponent(Component c) {
        ((CssLayout) getContent()).addComponent(c);
    }

}
