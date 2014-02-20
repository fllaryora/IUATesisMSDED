package com.example.botqueueweb.windows;

import com.example.botqueueweb.enums.WebConstantMessages;
import com.vaadin.ui.Button;
import com.vaadin.ui.Component;
import com.vaadin.ui.CssLayout;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.PasswordField;
import com.vaadin.ui.TextField;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;

public class UsuarioWindow extends Window {

    public UsuarioWindow() {
    	
    	this.setCaption(WebConstantMessages.USER_FORM);
    	this.setModal(true);
    	//subWindow.setStyleName("body-style");
    	setResizable(false);
        VerticalLayout subContent = new VerticalLayout();
        subContent.setMargin(true);
        subContent.setSpacing(true);
        setContent(subContent);
                
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label(WebConstantMessages.NAME);
        lNombre.setWidth("90");
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(new TextField());
        subContent.addComponent(hlNombre);
        
        HorizontalLayout hlApellido = new HorizontalLayout();
        Label lApellido = new Label(WebConstantMessages.LASTNAME);
        lApellido.setWidth("90");
        hlApellido.addComponent(lApellido);
        hlApellido.addComponent(new TextField());
        subContent.addComponent(hlApellido);
        
        HorizontalLayout hlUsuario = new HorizontalLayout();
        Label lUsuario = new Label(WebConstantMessages.USER);
        lUsuario.setWidth("90");
        hlUsuario.addComponent(lUsuario);
        hlUsuario.addComponent(new TextField());
        subContent.addComponent(hlUsuario);
        
        HorizontalLayout hlPass = new HorizontalLayout();
        Label lPass = new Label(WebConstantMessages.PASS);
        lPass.setWidth("90");
        hlPass.addComponent(lPass);
        PasswordField pfPass = new PasswordField();
        hlPass.addComponent(pfPass);
        subContent.addComponent(hlPass);
        
        //BOTONES
        
        HorizontalLayout hlBotones = new HorizontalLayout();
        hlBotones.setSpacing(true);
        Button bAceptar = new Button(WebConstantMessages.ACEPT);
        Button bCancelar = new Button(WebConstantMessages.CANCEL);
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

