package com.example.botqueueweb;

import java.util.ArrayList;
import java.util.List;

import org.bson.types.ObjectId;

import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.output.Combi;
import com.example.botqueueweb.dto.output.Counter;
import com.example.botqueueweb.dto.output.CounterFinal;
import com.example.botqueueweb.dto.output.Function;
import com.example.botqueueweb.dto.output.NodesStatus;
import com.example.botqueueweb.dto.output.Normal;
import com.example.botqueueweb.dto.output.Queue;
import com.example.botqueueweb.dto.output.QueueFinal;
import com.example.botqueueweb.dto.output.TimeLine;
import com.example.botqueueweb.facade.Facade;
import com.example.botqueueweb.js.Chart;


//import com.google.gwt.dev.util.collect.HashMap;
import java.util.HashMap;

import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.server.ThemeResource;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Image;
import com.vaadin.ui.Label;
import com.vaadin.ui.Panel;
import com.vaadin.ui.Table;
import com.vaadin.ui.VerticalLayout;

public class Reporte extends VerticalLayout implements View {

    private static final long serialVersionUID = 1L;

    Object editableId = null;

    ObjectId idProject;
    
    @Override
    public void enter(ViewChangeEvent event) {
    	
    	/* CONFIGURACION INICIAL */
    	setMargin(true);
        setSizeFull();
        addStyleName("transactions");
        
    	idProject = (ObjectId) ((HashMap<String,Object>)event.getNavigator().getUI().getData()).get("idProjectSelected");
    	//TODO: idProject==null no se selecciono proyecto
    	
    	/* PANEL */
    	Project project = Facade.getInstance().getProject(idProject);
    	Panel bodyPanel = new Panel();
    	bodyPanel.setWidth("100%");
    	bodyPanel.setHeight("100%");
    	VerticalLayout vlPanel = new VerticalLayout();


        /* Pie Chart*/
        /*final Chart chart2 = new Chart("Chart2");
        chart2.setType("Pie");
        chart2.addColor("#F38630");
        chart2.addPercent(25.0);
        chart2.addColor("#E0E4CC");
        chart2.addPercent(25.0);
        chart2.addColor("#69D2E7");
        chart2.addPercent(50.0);
        addComponent(chart2);*/
        
    	/* COMPONENTS */
    	
        showTop(project,vlPanel);
        List<TimeLine> deltaTs= project.getOutput().getTimeLines();
        if( deltaTs != null && !deltaTs.isEmpty() )
        {
        	NodesStatus status = deltaTs.get(0).getNodesStatus();
            if ( status.getCounters() != null && !status.getCounters().isEmpty() && status.getQueues() != null && !status.getQueues().isEmpty() )
            {
            	showSummaryReport(project,vlPanel);
            }
            if ( status.getCounters() != null && !status.getCounters().isEmpty() )
            {
            	showCounterFull(project,vlPanel);
            }
            if ( status.getCombis() != null && !status.getCombis().isEmpty() )
            {
            	showCombisFull(project,vlPanel);
            }
            if ( status.getQueues() != null && !status.getQueues().isEmpty() )
            {
            	showQueuesFull(project,vlPanel);
            }
            if ( status.getNormals() != null && !status.getNormals().isEmpty() )
            {
            	showNormalFull(project,vlPanel);
            }
            if ( status.getFunctions() != null && !status.getFunctions().isEmpty() )
            {
            	showFunctionFull(project,vlPanel);
            }	
        } else{
        	String errorMesage = project.getOutput().getError();
        	errorMesage = (errorMesage != null && !errorMesage.isEmpty() )?errorMesage:"No se encontro archivo de salida. Contacte a soporte";
        	vlPanel.setSpacing(true);
        	
        	Label title = new Label("Reporte de Error de botqueue");
            title.addStyleName("h1");
            vlPanel.addComponent(title);

            HorizontalLayout hlTupla;
            Label lTicket;
            
            lTicket =  new Label("Error: ");
            lTicket.addStyleName("ticket");
            hlTupla = new HorizontalLayout();
            hlTupla.setSpacing(true);
            hlTupla.addComponent(lTicket);
            hlTupla.addComponent(new Label(errorMesage) );
            vlPanel.addComponent(hlTupla);
        	
        }
        
        bodyPanel.setContent(vlPanel);
        addComponent(bodyPanel);
    }

    void showTop(Project project, VerticalLayout vlPanel)
    {
    	HorizontalLayout top = new HorizontalLayout();
    	top.setWidth("100%");
    	top.setSpacing(true);
    	top.addStyleName("toolbar");
    	
    	Label lTicket =  new Label("Proyecto: ");
        lTicket.addStyleName("ticket");
        lTicket.setSizeUndefined();
        Label lValue = new Label(project.getName());
        lValue.setSizeUndefined();
        
        ThemeResource imgBot = new ThemeResource("img/bot.png");
    	Image image = new Image(null,imgBot);
    	
        top.addComponent(image);
        top.addComponent(lTicket);
        top.addComponent(lValue);
        top.addComponent(new Label(" "));
        top.setSizeUndefined();
        
    	top.setComponentAlignment(lTicket, Alignment.MIDDLE_CENTER);
    	top.setComponentAlignment(lValue, Alignment.MIDDLE_CENTER);
    	vlPanel.addComponent(top);
    	vlPanel.setComponentAlignment(top, Alignment.MIDDLE_RIGHT);
    }
    
    void showSummaryReport(Project project, VerticalLayout vlPanel)
    {
    	vlPanel.setSpacing(true);
    	
    	Label title = new Label("Reporte Final");
        title.addStyleName("h1");
        vlPanel.addComponent(title);

        HorizontalLayout hlTupla;
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
    	vlPanel.addComponent(hlTupla);
    	
    	Table t = new Table();
        t.setSelectable(true);
        t.setImmediate(true);
        t.setColumnCollapsingAllowed(true);
        t.setColumnReorderingAllowed(true);
        
        t.addContainerProperty("Counter", Integer.class, null);
        t.addContainerProperty("Productividad Total", Integer.class , null);
        
        int i=0;
    	for (CounterFinal counterFinal : project.getOutput().getSummaryReport().getCounters()) {
    		i++;
    		t.addItem(new Object[]{counterFinal.getIdNode(),counterFinal.getTotalProductivity()},i);
		}
    	
    	vlPanel.addComponent(t);
    	
    	t = new Table();
        t.setSelectable(true);
        t.setImmediate(true);
        t.setColumnCollapsingAllowed(true);
        t.setColumnReorderingAllowed(true);
        
        t.addContainerProperty("Queue", Integer.class, null);
        t.addContainerProperty("Costo Fijo", String.class , null);
        t.addContainerProperty("Costo Variable", String.class , null);
        
        i=0;
    	for (QueueFinal queueFinal : project.getOutput().getSummaryReport().getQueues()) {
    		i++;
    		t.addItem(new Object[]{queueFinal.getIdNode(),"$ "+queueFinal.getFixCost(),"$ "+queueFinal.getVariableCost()},i);
    	}
    	
    	vlPanel.addComponent(t);
    	project.getOutput().getSummaryReport().getCounters();
    }
    
    void showCombisFull(Project project, VerticalLayout vlPanel)
    {
    	HorizontalLayout hlTupla;
        Label lTicket,lValue;
         
        List<Integer> combisIds = new ArrayList<Integer>();
	    for (Combi combiLocal : project.getOutput().getTimeLines().get(0).getNodesStatus().getCombis())
	    {
	    	combisIds.add(combiLocal.getIdNode());
		}
	    int timeLinesCount = project.getOutput().getTimeLines().size();
	    
	    
	    for (Integer idCombi : combisIds)
	    {
	    	ThemeResource imgCombi = new ThemeResource("img/combi.png");
	    	Image image = new Image(null,imgCombi);
	    	lTicket =  new Label("Combi: ");
	        lTicket.addStyleName("ticket");
	        lValue = new Label(idCombi.toString());
	        hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(image);
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	        
	        //hlTupla.set
	        vlPanel.addComponent(hlTupla);
	        
	    	//vlPanel.addComponent(image);	
	    	
	    	//BAR - COUNTER IMPUT
	    	
	    	vlPanel.addComponent(new Label("CounterImput"));	
	    	final Chart chart = new Chart("ChartCombi");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	chart.setWidthJS("750");
	    	chart.setHeightJS("300"); 
	    	chart.setPointStrokeColor("#fff");
	    	chart.addFillColor("rgba(151,187,205,0.5)");
	    	chart.addStrokeColor("rgba(151,187,205,1)");
	    	chart.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) { //60
	            for (Combi combi : project.getOutput().getTimeLines().get(i).getNodesStatus().getCombis()) {
	            	if (combi.getIdNode().equals(idCombi))
	            	{
	            		if (combi.getCounterInput() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) combi.getCounterInput();
	            	}
				}	
			}
	    	
	    	chart.addSeries(points);
	        
	    	vlPanel.addComponent(chart);
	    	
	    	// CHART - AMOUNT DELAY
	    	
	    	vlPanel.addComponent(new Label("AmountDelay "));	
	    	final Chart chartAmountDelay = new Chart("ChartCombi");
	    	chartAmountDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	double points2[] = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAmountDelay.setLabels(labels);
	    	
	    	chartAmountDelay.setWidthJS("750");
	    	chartAmountDelay.setHeightJS("300"); 
	    	chartAmountDelay.setPointStrokeColor("#fff");
	    	chartAmountDelay.addFillColor("rgba(151,187,205,0.5)");
	    	chartAmountDelay.addStrokeColor("rgba(151,187,205,1)");
	    	chartAmountDelay.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Combi combi : project.getOutput().getTimeLines().get(i).getNodesStatus().getCombis()) {
	            	if (combi.getIdNode().equals(idCombi))
	            	{
	            		if (combi.getAmountDelay() == null)
	            			points2[i] = 0;
	        		    else
	        		    	points2[i] = combi.getAmountDelay();
	            	}
				}	
			}
	    	
	    	chartAmountDelay.addPoints(points2);
	        
	    	vlPanel.addComponent(chartAmountDelay);
	    	
	    	// MINIMA Y MINIMA DURACION SORTEADA
	    	
	    	vlPanel.addComponent(new Label("Minimo / Maximo  "));	
	    	final Chart chartMM = new Chart("ChartCombi");
	    	chartMM.setType("Bar");
	    	
	    	labels = new String[timeLinesCount];
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartMM.setLabels(labels);
	    	
	    	chartMM.setWidthJS("750");
	    	chartMM.setHeightJS("300"); 
	    	chartMM.setPointStrokeColor("#fff");
	    	chartMM.addFillColor("rgba(151,187,205,0.5)");
	    	chartMM.addStrokeColor("rgba(151,187,205,1)");
	    	chartMM.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Combi combi : project.getOutput().getTimeLines().get(i).getNodesStatus().getCombis()) {
	            	if (combi.getIdNode().equals(idCombi))
	            	{
	            		if (combi.getMinimunDrawn() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) combi.getMinimunDrawn();
	            	}
				}	
			}
	    	
	    	chartMM.addSeries(points);
	        
	    	chartMM.addFillColor("rgba(220,220,220,0.5)");
	    	chartMM.addStrokeColor("rgba(220,220,220,1)");
	    	chartMM.addPointColor("rgba(220,220,220,1)");
	    	
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Combi combi : project.getOutput().getTimeLines().get(i).getNodesStatus().getCombis()) {
	            	if (combi.getIdNode().equals(idCombi))
	            	{
	            		if (combi.getMaximunDrawn() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) combi.getMaximunDrawn();
	            	}
				}	
			}
	    	chartMM.addSeries(points);
	    	
	    	vlPanel.addComponent(chartMM);
		}
    }
    
    void showQueuesFull(Project project, VerticalLayout vlPanel)
    {
    	HorizontalLayout hlTupla;
        Label lTicket,lValue;
         
        List<Integer> queuesIds = new ArrayList<Integer>();
	    for (Queue queueLocal : project.getOutput().getTimeLines().get(0).getNodesStatus().getQueues())
	    {
	    	queuesIds.add(queueLocal.getIdNode());
		}
	    int timeLinesCount = project.getOutput().getTimeLines().size();
	    
	    
	    for (Integer idQueue : queuesIds)
	    {
	    	ThemeResource imgQueue = new ThemeResource("img/queue.png");
	    	Image image = new Image(null,imgQueue);
	    	lTicket =  new Label("Queue: ");
	        lTicket.addStyleName("ticket");
	        lValue = new Label(idQueue.toString());
	        hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(image);
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	        
	        vlPanel.addComponent(hlTupla);
	        
	    	//vlPanel.addComponent(image);	
	    	
	    	//BAR - AMAUNT
	    	
	    	vlPanel.addComponent(new Label("Amaunt"));	
	    	final Chart chart = new Chart("ChartQueue");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	chart.setWidthJS("750");
	    	chart.setHeightJS("300"); 
	    	chart.setPointStrokeColor("#fff");
	    	chart.addFillColor("rgba(151,187,205,0.5)");
	    	chart.addStrokeColor("rgba(151,187,205,1)");
	    	chart.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Queue queue : project.getOutput().getTimeLines().get(i).getNodesStatus().getQueues()) {
	            	if (queue.getIdNode().equals(idQueue))
	            	{
	            		if (queue.getAmount() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) queue.getAmount();
	            	}
				}	
			}
	    	
	    	chart.addSeries(points);
	        
	    	vlPanel.addComponent(chart);
	    	
	    	// CHART - AMOUNT DELAY
	    	
	    	vlPanel.addComponent(new Label("Average"));	
	    	final Chart chartAmountDelay = new Chart("ChartCombi");
	    	chartAmountDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	double points2[] = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAmountDelay.setLabels(labels);
	    	
	    	chartAmountDelay.setWidthJS("750");
	    	chartAmountDelay.setHeightJS("300"); 
	    	chartAmountDelay.setPointStrokeColor("#fff");
	    	chartAmountDelay.addFillColor("rgba(151,187,205,0.5)");
	    	chartAmountDelay.addStrokeColor("rgba(151,187,205,1)");
	    	chartAmountDelay.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Queue queue : project.getOutput().getTimeLines().get(i).getNodesStatus().getQueues()) {
	            	if (queue.getIdNode().equals(idQueue))
	            	{
	            		if (queue.getAverage() == null)
	            			points2[i] = 0;
	        		    else
	        		    	points2[i] = queue.getAverage();
	            	}
				}	
			}
	    	
	    	chartAmountDelay.addPoints(points2);
	        
	    	vlPanel.addComponent(chartAmountDelay);
	    	
	    	//  DURACION SORTEADA
	    	
	    	vlPanel.addComponent(new Label("Counter Input/Output"));	
	    	final Chart chartIO = new Chart("ChartCombi");
	    	chartIO.setType("Bar");
	    	
	    	labels = new String[timeLinesCount];
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartIO.setLabels(labels);
	    	
	    	chartIO.setWidthJS("750");
	    	chartIO.setHeightJS("300"); 
	    	chartIO.setPointStrokeColor("#fff");
	    	chartIO.addFillColor("rgba(151,187,205,0.5)");
	    	chartIO.addStrokeColor("rgba(151,187,205,1)");
	    	chartIO.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Queue queue : project.getOutput().getTimeLines().get(i).getNodesStatus().getQueues()) {
	            	if (queue.getIdNode().equals(idQueue))
	            	{
	            		if (queue.getCounterInput() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) queue.getCounterInput();
	            	}
				}	
			}
	    	
	    	chartIO.addSeries(points);
	        
	    	chartIO.addFillColor("rgba(220,220,220,0.5)");
	    	chartIO.addStrokeColor("rgba(220,220,220,1)");
	    	chartIO.addPointColor("rgba(220,220,220,1)");
	    	
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Queue queue : project.getOutput().getTimeLines().get(i).getNodesStatus().getQueues()) {
	            	if (queue.getIdNode().equals(idQueue))
	            	{
	            		if (queue.getCounterOutput() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) queue.getCounterOutput();
	            	}
				}	
			}
	    	chartIO.addSeries(points);
	    	
	    	vlPanel.addComponent(chartIO);
	    	
	    	// MINIMA Y MINIMA DURACION SORTEADA
	    	
	    	vlPanel.addComponent(new Label("Minimo / Maximo  "));	
	    	final Chart chartMM = new Chart("ChartCombi");
	    	chartMM.setType("Bar");
	    	
	    	labels = new String[timeLinesCount];
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartMM.setLabels(labels);
	    	
	    	chartMM.setWidthJS("750");
	    	chartMM.setHeightJS("300"); 
	    	chartMM.setPointStrokeColor("#fff");
	    	chartMM.addFillColor("rgba(151,187,205,0.5)");
	    	chartMM.addStrokeColor("rgba(151,187,205,1)");
	    	chartMM.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Queue queue : project.getOutput().getTimeLines().get(i).getNodesStatus().getQueues()) {
	            	if (queue.getIdNode().equals(idQueue))
	            	{
	            		if (queue.getMinimun() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) queue.getMinimun();
	            	}
				}	
			}
	    	
	    	chartMM.addSeries(points);
	        
	    	chartMM.addFillColor("rgba(220,220,220,0.5)");
	    	chartMM.addStrokeColor("rgba(220,220,220,1)");
	    	chartMM.addPointColor("rgba(220,220,220,1)");
	    	
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Queue queue : project.getOutput().getTimeLines().get(i).getNodesStatus().getQueues()) {
	            	if (queue.getIdNode().equals(idQueue))
	            	{
	            		if (queue.getMaximun() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) queue.getMaximun();
	            	}
				}	
			}
	    	chartMM.addSeries(points);
	    	
	    	vlPanel.addComponent(chartMM);
		}
    }
    
    void showNormalFull(Project project, VerticalLayout vlPanel)
    {
    	HorizontalLayout hlTupla;
        Label lTicket,lValue;
         
        List<Integer> normalsIds = new ArrayList<Integer>();
	    for (Normal normalLocal : project.getOutput().getTimeLines().get(0).getNodesStatus().getNormals())
	    {
	    	normalsIds.add(normalLocal.getIdNode());
		}
	    int timeLinesCount = project.getOutput().getTimeLines().size();
	    
	    
	    for (Integer idNormal : normalsIds)
	    {
	    	ThemeResource imgCombi = new ThemeResource("img/normal.png");
	    	Image image = new Image(null,imgCombi);
	    	lTicket =  new Label("Normal: ");
	        lTicket.addStyleName("ticket");
	        lValue = new Label(idNormal.toString());
	        hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(image);
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	        
	        //hlTupla.set
	        vlPanel.addComponent(hlTupla);
	        
	    	//vlPanel.addComponent(image);	
	    	
	    	//BAR - COUNTER IMPUT
	    	
	    	vlPanel.addComponent(new Label("CounterImput"));	
	    	final Chart chart = new Chart("ChartNormal");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	chart.setWidthJS("750");
	    	chart.setHeightJS("300"); 
	    	chart.setPointStrokeColor("#fff");
	    	chart.addFillColor("rgba(151,187,205,0.5)");
	    	chart.addStrokeColor("rgba(151,187,205,1)");
	    	chart.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) { //60
	            for (Normal normal : project.getOutput().getTimeLines().get(i).getNodesStatus().getNormals()) {
	            	if (normal.getIdNode().equals(idNormal))
	            	{
	            		if (normal.getCounterInput() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) normal.getCounterInput();
	            	}
				}	
			}
	    	
	    	chart.addSeries(points);
	        
	    	vlPanel.addComponent(chart);
	    	
	    	// CHART - AMOUNT DELAY
	    	
	    	vlPanel.addComponent(new Label("AmountDelay "));	
	    	final Chart chartAmountDelay = new Chart("ChartCombi");
	    	chartAmountDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	double points2[] = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAmountDelay.setLabels(labels);
	    	
	    	chartAmountDelay.setWidthJS("750");
	    	chartAmountDelay.setHeightJS("300"); 
	    	chartAmountDelay.setPointStrokeColor("#fff");
	    	chartAmountDelay.addFillColor("rgba(151,187,205,0.5)");
	    	chartAmountDelay.addStrokeColor("rgba(151,187,205,1)");
	    	chartAmountDelay.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Normal normal : project.getOutput().getTimeLines().get(i).getNodesStatus().getNormals()) {
	            	if (normal.getIdNode().equals(idNormal))
	            	{
	            		if (normal.getAmountDelay() == null)
	            			points2[i] = 0;
	        		    else
	        		    	points2[i] = normal.getAmountDelay();
	            	}
				}	
			}
	    	
	    	chartAmountDelay.addPoints(points2);
	        
	    	vlPanel.addComponent(chartAmountDelay);
	    	
	    	// MINIMA Y MINIMA DURACION SORTEADA
	    	
	    	vlPanel.addComponent(new Label("Minimo / Maximo  "));	
	    	final Chart chartMM = new Chart("ChartCombi");
	    	chartMM.setType("Bar");
	    	
	    	labels = new String[timeLinesCount];
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartMM.setLabels(labels);
	    	
	    	chartMM.setWidthJS("750");
	    	chartMM.setHeightJS("300"); 
	    	chartMM.setPointStrokeColor("#fff");
	    	chartMM.addFillColor("rgba(151,187,205,0.5)");
	    	chartMM.addStrokeColor("rgba(151,187,205,1)");
	    	chartMM.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Normal normal : project.getOutput().getTimeLines().get(i).getNodesStatus().getNormals()) {
	            	if (normal.getIdNode().equals(idNormal))
	            	{
	            		if (normal.getMinimunDrawn() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) normal.getMinimunDrawn();
	            	}
				}	
			}
	    	
	    	chartMM.addSeries(points);
	        
	    	chartMM.addFillColor("rgba(220,220,220,0.5)");
	    	chartMM.addStrokeColor("rgba(220,220,220,1)");
	    	chartMM.addPointColor("rgba(220,220,220,1)");
	    	
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Normal normal : project.getOutput().getTimeLines().get(i).getNodesStatus().getNormals()) {
	            	if (normal.getIdNode().equals(idNormal))
	            	{
	            		if (normal.getMaximunDrawn() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) normal.getMaximunDrawn();
	            	}
				}	
			}
	    	chartMM.addSeries(points);
	    	
	    	vlPanel.addComponent(chartMM);
		}
    }
    
    void showFunctionFull(Project project, VerticalLayout vlPanel)
    {
    	HorizontalLayout hlTupla;
        Label lTicket,lValue;
         
        List<Integer> functionsIds = new ArrayList<Integer>();
	    for (Function functionLocal : project.getOutput().getTimeLines().get(0).getNodesStatus().getFunctions())
	    {
	    	functionsIds.add(functionLocal.getIdNode());
		}
	    int timeLinesCount = project.getOutput().getTimeLines().size();
	    
	    
	    for (Integer idFunction : functionsIds)
	    {
	    	ThemeResource imgFunction = new ThemeResource("img/function.png");
	    	Image image = new Image(null,imgFunction);
	    	lTicket =  new Label("Function: ");
	        lTicket.addStyleName("ticket");
	        lValue = new Label(idFunction.toString());
	        hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(image);
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	        
	        //hlTupla.set
	        vlPanel.addComponent(hlTupla);
	        
	    	//vlPanel.addComponent(image);	
	    	
	    	//BAR - COUNTER IMPUT
	    	
	    	vlPanel.addComponent(new Label("Amount"));	
	    	final Chart chart = new Chart("ChartFunction");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	chart.setWidthJS("750");
	    	chart.setHeightJS("300"); 
	    	chart.setPointStrokeColor("#fff");
	    	chart.addFillColor("rgba(151,187,205,0.5)");
	    	chart.addStrokeColor("rgba(151,187,205,1)");
	    	chart.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) { //60
	            for (Function function : project.getOutput().getTimeLines().get(i).getNodesStatus().getFunctions()) {
	            	if (function.getIdNode().equals(idFunction))
	            	{
	            		if (function.getAmount() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) function.getAmount();
	            	}
				}	
			}
	    	
	    	chart.addSeries(points);
	        
	    	vlPanel.addComponent(chart);
		}
    }
    
    void showCounterFull(Project project, VerticalLayout vlPanel)
    {
    	HorizontalLayout hlTupla;
        Label lTicket,lValue;
         
        List<Integer> countersIds = new ArrayList<Integer>();
	    for (Counter counterLocal : project.getOutput().getTimeLines().get(0).getNodesStatus().getCounters())
	    {
	    	countersIds.add(counterLocal.getIdNode());
		}
	    int timeLinesCount = project.getOutput().getTimeLines().size();
	    
	    
	    for (Integer idCounter : countersIds)
	    {
	    	ThemeResource imgCounter = new ThemeResource("img/counter.png");
	    	Image image = new Image(null,imgCounter);
	    	lTicket =  new Label("Counter: ");
	        lTicket.addStyleName("ticket");
	        lValue = new Label(idCounter.toString());
	        hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(image);
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	        
	        //hlTupla.set
	        vlPanel.addComponent(hlTupla);
	        
	    	//vlPanel.addComponent(image);	
	    	
	    	//BAR - COUNTER IMPUT
	    	
	    	vlPanel.addComponent(new Label("DeltaTProductivity"));	
	    	final Chart chart = new Chart("ChartFunction");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	chart.setWidthJS("750");
	    	chart.setHeightJS("300"); 
	    	chart.setPointStrokeColor("#fff");
	    	chart.addFillColor("rgba(151,187,205,0.5)");
	    	chart.addStrokeColor("rgba(151,187,205,1)");
	    	chart.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) { //60
	            for (Counter counter : project.getOutput().getTimeLines().get(i).getNodesStatus().getCounters()) {
	            	if (counter.getIdNode().equals(idCounter))
	            	{
	            		if (counter.getDeltaTProductivity() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) counter.getDeltaTProductivity();
	            	}
				}	
			}
	    	
	    	chart.addSeries(points);
	        
	    	vlPanel.addComponent(chart);
	    	
	    	//BAR - COUNTER IMPUT
	    	
	    	vlPanel.addComponent(new Label("TotalProductivity"));	
	    	final Chart chart2 = new Chart("ChartFunction");
	    	chart2.setType("Bar");
	    	
	    	labels= new String[timeLinesCount];
	    	points= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart2.setLabels(labels);
	    	
	    	chart2.setWidthJS("750");
	    	chart2.setHeightJS("300"); 
	    	chart2.setPointStrokeColor("#fff");
	    	chart2.addFillColor("rgba(151,187,205,0.5)");
	    	chart2.addStrokeColor("rgba(151,187,205,1)");
	    	chart2.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) { //60
	            for (Counter counter : project.getOutput().getTimeLines().get(i).getNodesStatus().getCounters()) {
	            	if (counter.getIdNode().equals(idCounter))
	            	{
	            		if (counter.getTotalProductivity() == null)
	        		    	points[i] = 0;
	        		    else
	        		    	points[i] = (double) counter.getTotalProductivity();
	            	}
				}	
			}
	    	
	    	chart2.addSeries(points);
	        
	    	vlPanel.addComponent(chart2);
	    	
	    	// CHART - AMOUNT DELAY
	    	
	    	vlPanel.addComponent(new Label("ProductivityPerTime"));	
	    	final Chart chartAmountDelay = new Chart("ChartCounter");
	    	chartAmountDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	double points2[] = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAmountDelay.setLabels(labels);
	    	
	    	chartAmountDelay.setWidthJS("750");
	    	chartAmountDelay.setHeightJS("300"); 
	    	chartAmountDelay.setPointStrokeColor("#fff");
	    	chartAmountDelay.addFillColor("rgba(151,187,205,0.5)");
	    	chartAmountDelay.addStrokeColor("rgba(151,187,205,1)");
	    	chartAmountDelay.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Counter counter : project.getOutput().getTimeLines().get(i).getNodesStatus().getCounters()) {
	            	if (counter.getIdNode().equals(idCounter))
	            	{
	            		if (counter.getProductivityPerTime() == null)
	            			points2[i] = 0;
	        		    else
	        		    	points2[i] = counter.getProductivityPerTime();
	            	}
				}	
			}
	    	
	    	chartAmountDelay.addPoints(points2);
	        
	    	vlPanel.addComponent(chartAmountDelay);
		}
    }
}