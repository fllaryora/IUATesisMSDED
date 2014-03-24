package com.example.botqueueweb.windows;

import java.util.Date;
import java.util.List;

import org.bson.types.ObjectId;

import com.example.botqueueweb.business.ProjectBussines;
import com.example.botqueueweb.dto.Project;
import com.vaadin.data.Container;
import com.vaadin.data.Item;
import com.vaadin.data.Property;
import com.vaadin.data.util.IndexedContainer;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
import com.vaadin.ui.Component;
import com.vaadin.ui.CssLayout;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.Notification;
import com.vaadin.ui.Panel;
import com.vaadin.ui.Table;
import com.vaadin.ui.TextField;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;

public class ProjectWindow extends Window {

	private static final long serialVersionUID = 1L;

	public ProjectWindow(final Table t) {
    	
		//CONFIGURACION
    	this.setCaption("Proyecto");
    	this.setModal(true);
    	this.setResizable(false);
    	
    	//PANEL
    	Panel bodyPanel = new Panel();
    	bodyPanel.setWidth("100%");
    	bodyPanel.setHeight("100%");
    	setContent(bodyPanel);
        VerticalLayout subContent = new VerticalLayout();
        subContent.setMargin(true);
        subContent.setSpacing(true);
        bodyPanel.setContent(subContent);
        
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label("Nombre: ");
        lNombre.setWidth("90");
        final TextField tfName = new TextField();
        tfName.setMaxLength(40);
        tfName.setWidth("330");

        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);
        
        //BOTONES
        HorizontalLayout hlBotones = new HorizontalLayout();
        hlBotones.setSpacing(true);
        
        Button bAceptar = new Button("Aceptar");
        bAceptar.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
			@SuppressWarnings("unchecked")
			@Override
			public void buttonClick(ClickEvent event) {
            	//VALIDACIONES
				if (tfName.getValue()==null || tfName.getValue().length()==0){
					Notification.show("Ingrese nombre de proyecto", Notification.Type.WARNING_MESSAGE);
					return;
				}
            	
            	//INSERT PROYECTO
            	ProjectBussines projectBussines = new ProjectBussines(); //TODO: Hacer singleton
            	Project project = new Project();
            	project.setName(tfName.getValue());
            	project.setState("C");
            	project.setConstructionStamp((new Long((new Date()).getTime())).toString());
            	project.setLastUpdatedStamp((new Long((new Date()).getTime())).toString());
            	//project.setUsr(new ArrayList<String>()); //TODO: Asignar Usuario con referencia
            	projectBussines.insertProject(project);
            	
            	// GET PROYECTOS
             	List<Project> projects = projectBussines.getProjects();
            	
            	//ACTUALIZACION DE TABLA (INTERFACE)
                String estadoName;
                Container container = new IndexedContainer();
                container.addContainerProperty("Id", ObjectId.class, null);
                container.addContainerProperty("Nombre", String.class, null);
                container.addContainerProperty("Estado", String.class , null);
                container.addContainerProperty("Nro de Procesos", Integer.class , null);
                for (Project project1 : projects) {
                	container.addItem(project1.getId());
                	Item item = container.getItem(project1.getId());
                	
            		estadoName = "";
            		if (project1.getState().equalsIgnoreCase("C"))
            			estadoName = "Construccion"; 
            		else if (project1.getState().equalsIgnoreCase("P"))
            			estadoName = "Pendiente";
            		else if (project1.getState().equalsIgnoreCase("E"))
        		    	estadoName = "Error";
        		    else if (project1.getState().equalsIgnoreCase("X"))
        		    	estadoName = "Ejecución";
        		    else if (project1.getState().equalsIgnoreCase("F"))
        		    	estadoName = "Finalizado";
            		
            		@SuppressWarnings("rawtypes")
					Property property = item.getItemProperty("Id");
                	property.setValue(project1.getId());
                	property = item.getItemProperty("Nombre");
                	property.setValue(project1.getName());
            		property = item.getItemProperty("Estado");
                	property.setValue(estadoName);
            		property = item.getItemProperty("Nro de Procesos");
                	property.setValue(project1.getNroProcs());
            	}
                t.setContainerDataSource(container);
                t.setVisibleColumns(new Object[]{"Nombre", "Estado", "Nro de Procesos"});
            	
        	    close();
			}
		});
        
        final Button bCancelar = new Button("Cancelar");
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