package com.example.botqueueweb.windows;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.dto.Project;
import com.mongodb.DBObject;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
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

public class QueueWindow extends Window {

	private static final long serialVersionUID = 1L;

	public QueueWindow(final DBObject queue, final AppletIntegration applet, boolean isFull, Project project) {
    	
		//CONFIGURACION INICIAL
    	this.setCaption("Cola");
    	this.setModal(true);
    	this.setResizable(false);
    	
    	//PANEL
    	Panel bodyPanel = new Panel();
    	bodyPanel.setWidth("100%");
    	bodyPanel.setHeight("100%");
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
        if (queue.get("name")!=null)
        	tfName.setValue(queue.get("name").toString());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);
        
        HorizontalLayout hlResource = new HorizontalLayout();
        Label lResource = new Label("Recursos: ");
        lResource.setWidth("110");
        final TextField tfResource = new TextField();
        tfResource.setWidth("310");
        if (queue.get("resource")!=null)
        	tfResource.setValue(queue.get("resource").toString());
        hlResource.addComponent(lResource);
	    hlResource.addComponent(tfResource);
	    if (isFull)
	        subContent.addComponent(hlResource);

        
        HorizontalLayout hlFixedCost = new HorizontalLayout();
        Label lFixedCost = new Label("Costo Fijo: ");
        lFixedCost.setWidth("110");
        final TextField tfFixedCost = new TextField();
        tfFixedCost.setWidth("310");
        if (queue.get("fixedCost")!=null)
        	tfFixedCost.setValue(queue.get("fixedCost").toString());
        hlFixedCost.addComponent(lFixedCost);
        hlFixedCost.addComponent(tfFixedCost);
	    if (isFull)
	    	subContent.addComponent(hlFixedCost);
        
        HorizontalLayout hlVariableCost = new HorizontalLayout();
        Label lVariableCost = new Label("Costo Variable: ");
        lVariableCost.setWidth("110");
        final TextField tfVariableCost = new TextField();
        tfVariableCost.setWidth("310");
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
        	bAceptar.addClickListener(new ClickListener() {
				private static final long serialVersionUID = 1L;
				@Override
				public void buttonClick(ClickEvent event) {
	            	String[] arrayParams = new String[4];
		        	arrayParams[0] = tfResource.getValue();
		        	arrayParams[1] = tfFixedCost.getValue();
		        	arrayParams[2] = tfVariableCost.getValue();
	            	arrayParams[3] = tfName.getValue();	        	    
	        	    applet.executeCommand("editNode", arrayParams);
	        	    close();
				}
			});
        }
        else
        {
	        bAceptar.addClickListener(new ClickListener() {
				private static final long serialVersionUID = 1L;
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
        
        Button bCancelar = null;
        if (!project.getState().equalsIgnoreCase("C"))
        	bCancelar = new Button("Cerrar");
        else
        	bCancelar = new Button("Cancelar");		
        bCancelar.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
			@Override
			public void buttonClick(ClickEvent event) {
        	    close();
			}
		});
        
        bAceptar.setWidth("100");
        bCancelar.setWidth("100");
        if (project.getState().equalsIgnoreCase("C"))
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
