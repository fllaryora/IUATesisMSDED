package com.example.botqueueweb;

import java.util.ArrayList;
import java.util.List;

import org.bson.types.ObjectId;

import com.example.botqueueweb.business.ProjectBussines;
import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.output.QueueFinal;
import com.example.botqueueweb.windows.FunctionWindow;
import com.example.botqueueweb.windows.ProjectWindow;
import com.mongodb.DBObject;
import com.mongodb.util.JSON;
import com.vaadin.data.Item;
import com.vaadin.event.ItemClickEvent;
import com.vaadin.event.ItemClickEvent.ItemClickListener;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.server.ThemeResource;
import com.vaadin.ui.Button;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Image;
import com.vaadin.ui.Label;
import com.vaadin.ui.Panel;
import com.vaadin.ui.Table;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;

public class Home extends VerticalLayout implements View, Button.ClickListener{

    private static final long serialVersionUID = 1L;

    Object editableId = null;

    @Override
    public void enter(final ViewChangeEvent event) {
        
        ProjectBussines projectBussines = new ProjectBussines(); //TODO: hacer singleton
    	List<Project> projects = projectBussines.getProjects();
    	setMargin(true);
    	
    	Panel bodyPanel = new Panel();
    	bodyPanel.setWidth("100%");
    	bodyPanel.setHeight("100%");
    	VerticalLayout vlPanel = new VerticalLayout();
    	vlPanel.setSpacing(true);
    	
        setSizeFull();
        addStyleName("transactions");
        
        ThemeResource imgLogo = new ThemeResource("img/logo.png");
    	Image image = new Image(null,imgLogo);
        //Label title = new Label("Bienvenido a Bot Queue");
        //title.addStyleName("h1");
        vlPanel.addComponent(image);
        //vlPanel.addComponent(title);

        Label subtitle = new Label("Proyectos");
        subtitle.addStyleName("ticket");
        vlPanel.addComponent(subtitle);
        
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
			
			@Override
			public void itemClick(ItemClickEvent event) {
				Item item = t.getItem(event.getItemId());
				Object myObjectProperty = item.getItemProperty("Id").getValue();
				t.setData(myObjectProperty);
			}
		});
    	
    	t.setVisibleColumns(new Object[]{"Nombre", "Estado", "Nro de Procesos"});
    	
    	vlPanel.addComponent(t);
    	
    	HorizontalLayout hlPanel = new HorizontalLayout();
    	Button btnSeleccion = new Button("Seleccionar");
    	btnSeleccion.addClickListener(new ClickListener() {
            @Override
            public void buttonClick(ClickEvent event2) {
                event2.getButton().addStyleName("selected");
                //getUI().getUI().setData(t.getData());// TODO:ver cual manda al UI
                event.getNavigator().getUI().setData(t.getData());// TODO:ver cual manda al UI 
            }
        });
    	hlPanel.addComponent(btnSeleccion);
    	btnSeleccion = new Button("Nuevo ...");
    	btnSeleccion.addClickListener(new ClickListener() {
            @Override
            public void buttonClick(ClickEvent event2) {
                event2.getButton().addStyleName("selected");
                //getUI().getUI().setData(t.getData());// TODO:ver cual manda al UI
                
                ProjectWindow fWindow = new ProjectWindow(event.getNavigator().getUI());
				fWindow.setHeight("140px");
				fWindow.setWidth("450px");
            	getUI().addWindow(fWindow);
            	//fWindow.addlistener
                //event.getNavigator().getUI().setData(t.getData());// TODO:ver cual manda al UI 
            }
        });
    	hlPanel.addComponent(btnSeleccion);
    	//hlPanel.addComponent(new Button("Nuevo ..."));
    	hlPanel.setSpacing(true);
    	vlPanel.addComponent(hlPanel);
    	
        /*HorizontalLayout hlTupla;
        Label lTicket;
        
        lTicket =  new Label("Costo Total: ");
        lTicket.addStyleName("ticket");
        hlTupla = new HorizontalLayout();
        hlTupla.setSpacing(true);
        hlTupla.addComponent(lTicket);
        hlTupla.addComponent(new Label("$ "+project.getOutput().getSummaryReport().getTotalCost()));
        vlPanel.addComponent(hlTupla);
        
        lTicket =  new Label("Tiempo Total: ");
        lTicket.addStyleName("ticket");
        hlTupla = new HorizontalLayout();
        hlTupla.setSpacing(true);
        hlTupla.addComponent(lTicket);
        hlTupla.addComponent(new Label(project.getOutput().getSummaryReport().getTotalTime().toString()));
    	vlPanel.addComponent(hlTupla);*/
        
        bodyPanel.setContent(vlPanel);
        addComponent(bodyPanel);
    }

	@Override
	public void buttonClick(ClickEvent event) {
		System.out.println("implement click");
		//no me funciono para ProjectWindow
	}

}
