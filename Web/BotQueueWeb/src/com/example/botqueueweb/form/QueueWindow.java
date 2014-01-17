package com.example.botqueueweb.form;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.node.Queue;
import com.vaadin.ui.Button;
import com.vaadin.ui.Component;
import com.vaadin.ui.CssLayout;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.PasswordField;
import com.vaadin.ui.TextField;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;

public class QueueWindow extends Window {

    public QueueWindow(Queue queue, AppletIntegration applet) {
    	
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
        TextField tfName = new TextField();
        tfName.setValue(queue.getName());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);
        
        HorizontalLayout hlResource = new HorizontalLayout();
        Label lResource = new Label("Recursos: ");
        lResource.setWidth("90");
        TextField tfResource = new TextField();
        tfResource.setValue(queue.getResource());
        hlResource.addComponent(lResource);
        hlResource.addComponent(tfResource);
        subContent.addComponent(hlResource);
        
        //TODO: Implementar en forma de lista no editable
        HorizontalLayout hlPreceders = new HorizontalLayout();
        Label lPreceders = new Label("Predecesores: ");
        lPreceders.setWidth("90");
        hlPreceders.addComponent(lPreceders);
        //hlPreceders.addComponent(new TextField()); // tabla recorriendo elementos
        subContent.addComponent(hlPreceders);
        
        //TODO: Implementar en forma de lista no editable
        HorizontalLayout hlFollowers = new HorizontalLayout();
        Label lFollowers = new Label("Sucesores: ");
        lFollowers.setWidth("90");
        hlFollowers.addComponent(lFollowers);
        // hlFollowers.addComponent(new TextField()); // tabla recorriendo elementos
        subContent.addComponent(hlFollowers);
        
        HorizontalLayout hlFixedCost = new HorizontalLayout();
        Label lFixedCost = new Label("Costo Fijo: ");
        lFixedCost.setWidth("90");
        TextField tfFixedCost = new TextField();
        tfFixedCost.setValue(queue.getFixedCost());
        hlFixedCost.addComponent(lFixedCost);
        hlResource.addComponent(tfFixedCost);
        subContent.addComponent(hlFixedCost);
        
        HorizontalLayout hlVariableCost = new HorizontalLayout();
        Label lVariableCost = new Label("Costo Variable: ");
        lVariableCost.setWidth("90");
        TextField tfVariableCost = new TextField();
        tfVariableCost.setValue(queue.getVariableCost());
        hlVariableCost.addComponent(lVariableCost);
        hlVariableCost.addComponent(tfVariableCost);
        subContent.addComponent(hlVariableCost);
        
        //BOTONES
        
        HorizontalLayout hlBotones = new HorizontalLayout();
        hlBotones.setSpacing(true);
        Button bAceptar = new Button("Aceptar"); // debe llamar al metodo del aplet para guardar cambios en el mismo
        Button bCancelar = new Button("Cancelar");
        bAceptar.setWidth("100");
        bCancelar.setWidth("100");
        hlBotones.addComponent(bAceptar);
        hlBotones.addComponent(bCancelar);
        subContent.addComponent(hlBotones);
        
        // Center it in the browser window
        center();
        
        // Open it in the UI
        
        //setContent(new CssLayout());
        //setPrimaryStyleName("help-overlay");
        //setDraggable(false);
        //setResizable(false);
    }

    public void addComponent(Component c) {
        ((CssLayout) getContent()).addComponent(c);
    }

}
