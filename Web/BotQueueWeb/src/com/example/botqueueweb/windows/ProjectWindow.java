package com.example.botqueueweb.windows;

import java.util.Date;
import java.util.List;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.business.ProjectBussines;
import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.input.JsonInput;
import com.example.botqueueweb.dto.output.JsonOutput;
import com.mongodb.DBObject;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
import com.vaadin.ui.Component;
import com.vaadin.ui.CssLayout;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.Panel;
import com.vaadin.ui.TextField;
import com.vaadin.ui.UI;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;

public class ProjectWindow extends Window {

    public ProjectWindow(UI uiPpal) {
    	
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
        
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label("Nombre: ");
        lNombre.setWidth("90");
        final TextField tfName = new TextField();
        tfName.setWidth("330");

        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);
        
        //BOTONES
        
        HorizontalLayout hlBotones = new HorizontalLayout();
        hlBotones.setSpacing(true);
        
        Button bAceptar = new Button("Aceptar");
        
        bAceptar.addClickListener(new ClickListener() {
            @Override
			public void buttonClick(ClickEvent event) {
            	//TODO: validaciones
            	//TODO: insert en base con el nombre
            	ProjectBussines projectBussines = new ProjectBussines(); //TODO: hacer singleton
            	Project project = new Project();
            	project.setName(tfName.getValue());
            	project.setState("C");
            	project.setLastUpdatedStamp((new Date()).toString());
            	//project.setUsr(new ArrayList<String>());
            	projectBussines.insertProject(project);
            	//TODO: poner como seleccionado
            	//TODO: en ventana deberia mostrarse projecto seleccionado
        	    close();
			}
		});
        
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
        
        //setContent(new CssLayout());
        //setPrimaryStyleName("help-overlay");
        //setDraggable(false);
        //setResizable(false);
    }

    public void addComponent(Component c) {
        ((CssLayout) getContent()).addComponent(c);
    }

}