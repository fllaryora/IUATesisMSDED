package com.example.botqueueweb;

import java.util.Arrays;
import java.util.Map;

import org.vaadin.applet.AppletIntegration;

import com.vaadin.data.Item;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.ui.Button;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;
//import com.vaadin.ui.Button.ClickListener;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.VerticalLayout;

public class Precursor extends VerticalLayout implements View {

    private static final long serialVersionUID = 1L;

    Object editableId = null;

    private AppletIntegration applet;
    
    QueueContainer data;

    @Override
    public void enter(ViewChangeEvent event) {
    	
		applet = new AppletIntegration() {

	        private static final long serialVersionUID = 1L;
	
	        @Override
	        public void attach() {
	        	super.attach();
	            //setAppletArchives(Arrays.asList(new String[]{"BotQueue.jar"}));  
	            //setCodebase("/TestApplet2/VAADIN/applet/");     
	            //setAppletClass("ar/com/botqueue/applet/Principal.class");
	            
	            setAppletArchives(Arrays.asList(new String[]{"BotQueue.jar"}));  
	            setCodebase("/BotQueueWeb/VAADIN/applet/");     
	            setAppletClass("ar/com/botqueue/applet/Principal.class");  
	            
	            //executeCommand("renameNode"); // breaks here
	            
	            //setWidth("500px");  
	            //setHeight("500px");
	            setWidth("1000px");  
	            setHeight("1000px");
	        }
	        
	        @Override
            public void changeVariables(Object source, Map variables) {
	        	
                super.changeVariables(source, variables);
                
                if (variables.containsKey("mydata")) {                    
                    String strEstadoOk = (String) variables.get("mydata");            
                    if (strEstadoOk.equals("somevalue")){;
                        System.out.println("OK");                    
                    } else {
                    	System.out.println("error");
                    	//applet.executeCommand("exit");    
                    }
                }    
            }
	        
	    };
	    
    	setSpacing(false);
    	
    	
        //setSizeFull();
        //addStyleName("transactions");

        HorizontalLayout hlBotonera = new HorizontalLayout();
        hlBotonera.setHeight("40px");
        hlBotonera.setMargin(true);
        
		Button button = new Button("Cola");
		button.addClickListener(new ClickListener() {
            @Override
			public void buttonClick(ClickEvent event) {
				addComponent(new Label("Thank you for clicking"));
				String[] arrayParams = new String[2];
			    arrayParams[0] = "1";
			    arrayParams[1] = "Nuevo";		
			    applet.executeCommand("createNode", arrayParams);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Combi");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				String[] arrayParams = new String[2];
			    arrayParams[0] = "1";
			    arrayParams[1] = "Nuevo";		
			    applet.executeCommand("createNode", arrayParams);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("Funcion");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
				String[] arrayParams = new String[2];
			    arrayParams[0] = "2";
			    arrayParams[1] = "Nuevo";		
			    applet.executeCommand("createNode", arrayParams);
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);

        button = new Button("Delete");
		button.addClickListener(new ClickListener() {
            @Override
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteNode");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createArrow");
		button.addClickListener(new ClickListener() {
            @Override
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("invertArrow");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("invertArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("deleteArrow");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteArrow");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);

        button = new Button("rotateHead");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("rotateHead");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("rotateTail");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("rotateTail");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("deleteDot");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("deleteDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createNextDot");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createNextDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        button = new Button("createPreviusDot");
		button.addClickListener(new Button.ClickListener() {
			public void buttonClick(ClickEvent event) {
			    applet.executeCommand("createPreviusDot");
			}
		});
		button.addStyleName("small");
        hlBotonera.addComponent(button);
        
        addComponent(hlBotonera);
        
	    addComponent(applet);
	    
        /*Flot  grafico = new Flot();
        double points[]= { 0.6, 1.8, 2.8, 1.2};
        grafico.addSeries(points);
        addComponent(grafico);*/
       
        /* GAUGE */
        
        /*HorizontalLayout gaugeLayout = new HorizontalLayout();
        final Gauge gauge = new Gauge("gauge");
        gaugeLayout.addComponent(gauge);

        final Gauge gauge2 = new Gauge("gauge2");
        gaugeLayout.addComponent(gauge2);

        addComponent(gaugeLayout);

        Button b = new Button("Randomize!", new Button.ClickListener() {
            @Override
            public void buttonClick(ClickEvent event) {
                Random ran = new Random();
                int rand1 = ran.nextInt(100);
                gauge.setValue(rand1);

                int rand2 = ran.nextInt(100);
                gauge2.setValue(rand2);

            }
        });
        addComponent(b);*/
    }

    private boolean filterByProperty(String prop, Item item, String text) {
        if (item == null || item.getItemProperty(prop) == null
                || item.getItemProperty(prop).getValue() == null)
            return false;
        String val = item.getItemProperty(prop).getValue().toString().trim()
                .toLowerCase();
        if (val.startsWith(text.toLowerCase().trim()))
            return true;
        return false;
    }

    void createNewReportFromSelection() {
       // ((BotqueuewebUI) getUI()).openReports(t);
    }

}