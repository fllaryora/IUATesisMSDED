package com.example.botqueueweb.windows;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.dto.input.Combi;
import com.vaadin.data.Property;
import com.vaadin.data.Property.ValueChangeEvent;
import com.vaadin.ui.Button;
import com.vaadin.ui.ComboBox;
import com.vaadin.ui.Component;
import com.vaadin.ui.CssLayout;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.TextField;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;

public class CombiWindow extends Window {

    public CombiWindow(Combi combi, final AppletIntegration applet, boolean isFull) {
    	
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
		
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label("Nombre: ");
        lNombre.setWidth("90");
        final TextField tfName = new TextField();
        if (combi.getName()!=null)
        	tfName.setValue(combi.getName());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);
        
        if (isFull)
        {
        	/*HorizontalLayout hlResource = new HorizontalLayout();
	        Label lResource = new Label("Recursos: ");
	        lResource.setWidth("90");
	        TextField tfResource = new TextField();
	        if (queue.getResource()!=null)
	        	tfResource.setValue(queue.getResource().toString());
	        hlResource.addComponent(lResource);
	        hlResource.addComponent(tfResource);
	        subContent.addComponent(hlResource);*/
	        
        	/*private Double minimun;
        	private Double maximun;
        	private Double shapeAlpha;
        	private Double shapeBeta;
        	private Double constant;
        	private Double lambda;
        	private Double escale;
        	private Double shape;
        	private Double mean;
        	private Double variance;
        	private Double least;
        	private Double highest;
        	private Double mode;*/
        	
        	final ComboBox cbDelay = new ComboBox();
        	cbDelay.addItem("---");
        	cbDelay.addItem("Deterministica");
        	cbDelay.addItem("Uniforme");
        	cbDelay.addItem("Exponencial");
        	cbDelay.addItem("Normal");
        	cbDelay.addItem("Beta");
        	cbDelay.addItem("Triangular");
        	
        	cbDelay.addListener(new Property.ValueChangeListener() {
				
				@Override
				public void valueChange(ValueChangeEvent event) {
					if (cbDelay.getValue()!=null)
					{
						if (cbDelay.getValue().toString().equalsIgnoreCase("Deterministica"))
						{
							
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Uniforme"))
						{
							
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Exponencial"))
						{
							
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Normal"))
						{
							
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Beta"))
						{
							
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Triangular"))
						{
							
						}
					}
				}
			});
        	
	        /*HorizontalLayout hlResource = new HorizontalLayout();
	        Label lResource = new Label("Recursos: ");
	        lResource.setWidth("90");
	        TextField tfResource = new TextField();
	        if (queue.getResource()!=null)
	        	tfResource.setValue(queue.getResource().toString());
	        hlResource.addComponent(lResource);
	        hlResource.addComponent(tfResource);
	        subContent.addComponent(hlResource);
	        
	        HorizontalLayout hlFixedCost = new HorizontalLayout();
	        Label lFixedCost = new Label("Costo Fijo: ");
	        lFixedCost.setWidth("90");
	        TextField tfFixedCost = new TextField();
	        if (queue.getFixedCost()!=null)
	        	tfFixedCost.setValue(queue.getFixedCost().toString());
	        hlFixedCost.addComponent(lFixedCost);
	        hlFixedCost.addComponent(tfFixedCost);
	        subContent.addComponent(hlFixedCost);
	        
	        HorizontalLayout hlVariableCost = new HorizontalLayout();
	        Label lVariableCost = new Label("Costo Variable: ");
	        lVariableCost.setWidth("90");
	        TextField tfVariableCost = new TextField();
	        if (queue.getVariableCost()!=null)
	        	tfVariableCost.setValue(queue.getVariableCost().toString());
	        hlVariableCost.addComponent(lVariableCost);
	        hlVariableCost.addComponent(tfVariableCost);
	        subContent.addComponent(hlVariableCost);*/
        }
        
        //BOTONES
        
        HorizontalLayout hlBotones = new HorizontalLayout();
        hlBotones.setSpacing(true);
        
        Button bAceptar = new Button("Aceptar");
        
        if(isFull)
        {
        	//TODO: applet.executeCommand("editNode", arrayParams);
        	//con json completo de Queue
        }
        else
        {
	        bAceptar.addClickListener(new ClickListener() {
	            @Override
				public void buttonClick(ClickEvent event) {
	            	String[] arrayParams = new String[2];
	        	    arrayParams[0] = "1";
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
