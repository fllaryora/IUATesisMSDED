package com.example.botqueueweb.windows;

import java.util.HashMap;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.dto.Project;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
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

public class ProjectFinalWindow extends Window {

	private static final long serialVersionUID = 1L;

	public ProjectFinalWindow(final AppletIntegration applet,final ViewChangeEvent eventPpal, Project project) {
    	
    	this.setCaption("Proyecto");
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
        
        HorizontalLayout hlName = new HorizontalLayout();
        Label lName = new Label("Nombre: ");
        lName.setWidth("210");
        final TextField tfName = new TextField();
        tfName.setWidth("330");
        tfName.setValue(project.getName());
        hlName.addComponent(lName);
        hlName.addComponent(tfName);
        subContent.addComponent(hlName);
        
        HorizontalLayout hlLength = new HorizontalLayout();
        Label lLength = new Label("Tiempo de Simulación: ");
        lLength.setWidth("210");
        final TextField tfLength = new TextField();
        tfLength.setWidth("330");
        hlLength.addComponent(lLength);
        hlLength.addComponent(tfLength);
        subContent.addComponent(hlLength);
        
        HorizontalLayout hlDeltaT = new HorizontalLayout();
        Label lDeltaT = new Label("Paso de Simulación [ 0.01 , 1 ]: ");
        lDeltaT.setWidth("210");
        final TextField tfDeltaT = new TextField();
        tfDeltaT.setWidth("330");
        hlDeltaT.addComponent(lDeltaT);
        hlDeltaT.addComponent(tfDeltaT);
        subContent.addComponent(hlDeltaT);
        
        HorizontalLayout hlSeed = new HorizontalLayout();
        Label lSeed = new Label("Semilla: ");
        lSeed.setWidth("210");
        final TextField tfSeed = new TextField();
        tfSeed.setWidth("330");
        hlSeed.addComponent(lSeed);
        hlSeed.addComponent(tfSeed);
        subContent.addComponent(hlSeed);
        
        //BOTONES
        
        HorizontalLayout hlBotones = new HorizontalLayout();
        hlBotones.setSpacing(true);
        
        Button bAceptar = new Button("Aceptar");
        
        bAceptar.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
			@Override
			public void buttonClick(ClickEvent event) {
				((HashMap<String,Object>) eventPpal.getNavigator().getUI().getData()).put("nameProjectPending", tfName.getValue());
				((HashMap<String,Object>) eventPpal.getNavigator().getUI().getData()).put("simTime", Double.parseDouble(tfLength.getValue()));
				((HashMap<String,Object>) eventPpal.getNavigator().getUI().getData()).put("deltaT", Double.parseDouble(tfDeltaT.getValue()));
            	String[] arrayParams = new String[2];
            	arrayParams[0] = (int)Math.ceil(Double.parseDouble(tfLength.getValue())/( Double.parseDouble(tfDeltaT.getValue())))+"";
            	arrayParams[1] = tfSeed.getValue();        	
			    applet.executeCommand("getModelInfo",arrayParams);
        	    close();
			}
		});
        
        Button bCancelar = new Button("Cancelar");
        bCancelar.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
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
