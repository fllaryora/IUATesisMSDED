package com.example.botqueueweb;

import java.util.List;

import org.bson.types.ObjectId;

import com.example.botqueueweb.business.ProjectBussines;
import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.windows.ProjectWindow;
import com.vaadin.data.Container;
import com.vaadin.data.Item;
import com.vaadin.data.Property;
import com.vaadin.data.util.IndexedContainer;
import com.vaadin.event.ItemClickEvent;
import com.vaadin.event.ItemClickEvent.ItemClickListener;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.server.ThemeResource;
import com.vaadin.ui.Button;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Image;
import com.vaadin.ui.Label;
import com.vaadin.ui.Notification;
import com.vaadin.ui.Panel;
import com.vaadin.ui.Table;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;

public class Home extends VerticalLayout implements View{

    private static final long serialVersionUID = 1L;

    Object editableId = null;

    @SuppressWarnings("deprecation")
	@Override
    public void enter(final ViewChangeEvent event) {
        
        ProjectBussines projectBussines = new ProjectBussines(); //TODO: HACER SINGLETON
    	List<Project> projects = projectBussines.getProjects();
    	
    	//CONFIGURACION INICIAL
    	setMargin(true);
    	Panel bodyPanel = new Panel();
    	bodyPanel.setWidth("100%");
    	bodyPanel.setHeight("100%");
    	VerticalLayout vlPanel = new VerticalLayout();
    	vlPanel.setSpacing(true);
        setSizeFull();
        addStyleName("transactions");
        
        //LOGO
        ThemeResource imgLogo = new ThemeResource("img/logo.png");
    	Image image = new Image(null,imgLogo);
        vlPanel.addComponent(image);

        //PROYECTOS
        Label subtitle = new Label("Proyectos");
        subtitle.addStyleName("ticket");
        vlPanel.addComponent(subtitle);
        
        //SELECCCIONADO
        //TODO:Agregar proyecto seleccionado (label + nombre)
        
        //TABLA DE PROYECTOS
        //TODO: Ancho de columna fijo, ver tamaños maximos
        final Table t = new Table();
        t.setSelectable(true);
        t.setImmediate(true);
        t.setColumnCollapsingAllowed(true);
        t.setColumnReorderingAllowed(true);
        
        t.addContainerProperty("Id", ObjectId.class, null);
        t.addContainerProperty("Nombre", String.class, null);
        t.addContainerProperty("Estado", String.class , null);
        t.addContainerProperty("Nro de Procesos", Integer.class , null);
        
        int i=0;
        String estadoName;
    	for (Project project : projects) {
    		i++;
    		estadoName = "";
    		if (project.getState().equalsIgnoreCase("C"))
    			estadoName = "Construccion"; 
    		else if (project.getState().equalsIgnoreCase("P"))
    			estadoName = "Pendiente";
    		else if (project.getState().equalsIgnoreCase("E"))
		    	estadoName = "Error";
		    else if (project.getState().equalsIgnoreCase("X"))
		    	estadoName = "Ejecución";
		    else if (project.getState().equalsIgnoreCase("F"))
		    	estadoName = "Finalizado";
    		t.addItem(new Object[]{project.getId(),project.getName(),estadoName,project.getNroProcs()},i);
    	}
    	t.addListener(new ItemClickListener() {
			private static final long serialVersionUID = 1L;
			@Override
			public void itemClick(ItemClickEvent event) {
				Item item = t.getItem(event.getItemId());
				Object myObjectProperty = item.getItemProperty("Id").getValue();
				t.setData(myObjectProperty);
			}
		});
    	t.setVisibleColumns(new Object[]{"Nombre", "Estado", "Nro de Procesos"});
    	vlPanel.addComponent(t);
    	
    	//BOTONES
    	//TODO: botones aliniados a la detecha, borde derecho de la tabla
    	HorizontalLayout hlPanel = new HorizontalLayout();
    	Button btnGeneric = new Button("Refrescar");
    	btnGeneric.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
			@SuppressWarnings({ "rawtypes", "unchecked" })
			@Override
            public void buttonClick(ClickEvent event2) {
				//GET PROYECTOS
				ProjectBussines projectBussines = new ProjectBussines(); //TODO: HACER SINGLETON
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
            }
        });
    	hlPanel.addComponent(btnGeneric);
    	
    	btnGeneric = new Button("Seleccionar");
    	btnGeneric.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;

			@Override
            public void buttonClick(ClickEvent event2) {
                event2.getButton().addStyleName("selected");
                event.getNavigator().getUI().setData(t.getData()); 
            }
        });
    	hlPanel.addComponent(btnGeneric);
    	
    	btnGeneric = new Button("Eliminar");
    	btnGeneric.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;

			@SuppressWarnings({ "rawtypes", "unchecked" })
			@Override
            public void buttonClick(ClickEvent event2) {
                event2.getButton().addStyleName("selected");
                
                //VALIDACIONES
                if(t.getData()==null){
                	Notification.show("Seleccione un proyecto de la grilla", Notification.Type.WARNING_MESSAGE);
            		return;
                }
                
                //ELIMINAR PROYECTO
                ObjectId idProject = (ObjectId) t.getData();
                ProjectBussines projectBussines = new ProjectBussines(); //TODO: HACER SINGLETON
    	    	projectBussines.deleteProject(idProject);
    	    	t.setData(null);
    	    	event.getNavigator().getUI().setData(null); 
    	    	
    	    	//GET PROYECTOS
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
            }
        });
    	hlPanel.addComponent(btnGeneric);
    	
    	btnGeneric = new Button("Nuevo ...");
    	btnGeneric.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;

			@Override
            public void buttonClick(ClickEvent event2) {
                event2.getButton().addStyleName("selected");
                
                //ABRIR NUEVO PROYECTO
                ProjectWindow fWindow = new ProjectWindow(t);
				fWindow.setHeight("140px");
				fWindow.setWidth("450px");
            	getUI().addWindow(fWindow);
            }
        });
    	hlPanel.addComponent(btnGeneric);
    	hlPanel.setSpacing(true);
    	vlPanel.addComponent(hlPanel);
        
        bodyPanel.setContent(vlPanel);
        addComponent(bodyPanel);
    }

}
