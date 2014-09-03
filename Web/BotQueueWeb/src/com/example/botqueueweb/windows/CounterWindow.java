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

public class CounterWindow extends Window {

	private static final long serialVersionUID = 1L;

	public CounterWindow(final DBObject counter, final AppletIntegration applet, boolean isFull, Project project) {
    	
		//CONFIGURACION INICIAL
    	this.setCaption("Contador");
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
        
        //ELEMENTOS
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label("Nombre: ");
        if (isFull)	lNombre.setWidth("75");
        else		lNombre.setWidth("90");
        final TextField tfName = new TextField();
        if (isFull)	tfName.setWidth("345");
        else		tfName.setWidth("330");
        if (counter.get("name")!=null)
        	tfName.setValue(counter.get("name").toString());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);
        
        HorizontalLayout hlQuantity = new HorizontalLayout();
        Label lQuantity = new Label("Quantity: ");
        lQuantity.setWidth("75");
        final TextField tfQuantity = new TextField();
        tfQuantity.setWidth("345");
        if (counter.get("quantity")!=null)
        	tfQuantity.setValue(counter.get("quantity").toString());
        hlQuantity.addComponent(lQuantity);
	    hlQuantity.addComponent(tfQuantity);
	    if (isFull)
	        subContent.addComponent(hlQuantity);

        
        HorizontalLayout hlCycle = new HorizontalLayout();
        Label lCycle = new Label("Cycle: ");
        lCycle.setWidth("75");
        final TextField tfCycle = new TextField();
        tfCycle.setWidth("345");
        if (counter.get("cycle")!=null)
        	tfCycle.setValue(counter.get("cycle").toString());
        hlCycle.addComponent(lCycle);
        hlCycle.addComponent(tfCycle);
	    if (isFull)
	    	subContent.addComponent(hlCycle);
        
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
	            	String[] arrayParams = new String[3];
		        	arrayParams[0] = tfQuantity.getValue();
		        	arrayParams[1] = tfCycle.getValue();
	            	arrayParams[2] = tfName.getValue();	        	    
	        	    applet.executeCommand("editNode", arrayParams);
	        	    applet.setStyleName("v-AppletVisible");
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
	        	    arrayParams[0] = "2";
	        	    arrayParams[1] = tfName.getValue();		
	        	    applet.executeCommand("createNode", arrayParams);
	        	    applet.setStyleName("v-AppletVisible");
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
				applet.setStyleName("v-AppletVisible");
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
