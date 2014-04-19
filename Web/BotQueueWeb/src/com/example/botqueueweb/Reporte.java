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
    
    HashMap<Integer, String> hashNameNodes;
    
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

    	/* Crear Hash (mapea id y nombre)*/
    	
    	hashNameNodes = new HashMap<Integer, String>();
    	
    	if (project.getInput().getTransformation().getQueues()!=null)
	    	for (com.example.botqueueweb.dto.input.Queue queue : project.getInput().getTransformation().getQueues()) {
	    		hashNameNodes.put(queue.getIdNode(), queue.getName());
			}
    	if (project.getInput().getTransformation().getCounters()!=null)
	    	for (com.example.botqueueweb.dto.input.Counter counter : project.getInput().getTransformation().getCounters()) {
	    		hashNameNodes.put(counter.getIdNode(), counter.getName());
			}
    	if (project.getInput().getTransformation().getNormals()!=null)
	    	for (com.example.botqueueweb.dto.input.Normal normal : project.getInput().getTransformation().getNormals()) {
	    		hashNameNodes.put(normal.getIdNode(), normal.getName());
			}
    	if (project.getInput().getTransformation().getFunctions()!=null)
	    	for (com.example.botqueueweb.dto.input.Function function : project.getInput().getTransformation().getFunctions()) {
	    		hashNameNodes.put(function.getIdNode(), function.getName());
			}
    	if (project.getInput().getTransformation().getCombis()!=null)
	    	for (com.example.botqueueweb.dto.input.Combi combi : project.getInput().getTransformation().getCombis()) {
	    		hashNameNodes.put(combi.getIdNode(), combi.getName());
			}
    	
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
        
        if (project.getOutput() == null || (project.getOutput()!=null && !(( project.getOutput().getTimeLines() != null && !project.getOutput().getTimeLines().isEmpty() ))))
        {
        	String errorMesage = (project.getOutput() != null &&  project.getOutput().getError() != null && !project.getOutput().getError().isEmpty())?project.getOutput().getError():"No se encontro archivo de salida. Conectese a soporte";
        	vlPanel.setSpacing(true);
        	
        	Label title = new Label("Reporte de Error de BotQueue");
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
        else
        {
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
	        }
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
	        //lValue = new Label(idCombi.toString());
	        lValue = new Label(hashNameNodes.get(idCombi));
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
	    	
	    	vlPanel.addComponent(new Label("Cantidad de Entradas"));	
	    	final Chart chart = new Chart("ChartCombi");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chart.setWidthJS("750");
	    	else
	    		chart.setWidthJS(""+(15*timeLinesCount));
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
	    	
	    	vlPanel.addComponent(new Label("Promedio de Duraciones Sorteadas"));	
	    	final Chart chartAmountDelay = new Chart("ChartCombi");
	    	chartAmountDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	double points2[] = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAmountDelay.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartAmountDelay.setWidthJS("750");
	    	else
	    		chartAmountDelay.setWidthJS(""+(15*timeLinesCount));
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
	        		    {
	        		    	if (combi.getCounterInput() == null || combi.getCounterInput() ==0)
	        		    		points2[i] = 0;
	        		    	else
	        		    		points2[i] = combi.getAmountDelay()/combi.getCounterInput();
	        		    }
	            	}
				}	
			}
	    	
	    	chartAmountDelay.addPoints(points2);
	        
	    	vlPanel.addComponent(chartAmountDelay);
	    	
	    	// MINIMA Y MINIMA DURACION SORTEADA
	    	
	    	vlPanel.addComponent(new Label("Duración Minimo / Maximo "));	
	    	final Chart chartMM = new Chart("ChartCombi");
	    	chartMM.setType("Bar");
	    	
	    	labels = new String[timeLinesCount];
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartMM.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartMM.setWidthJS("750");
	    	else
	    		chartMM.setWidthJS(""+(15*timeLinesCount));
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
	        //lValue = new Label(idQueue.toString());
	        lValue = new Label(hashNameNodes.get(idQueue));
	        hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(image);
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	        
	        vlPanel.addComponent(hlTupla);
	        
	    	//TIEMPO ESPERA PROMEDIO
	        
	        lTicket =  new Label("Tiempo de Espera Promedio: ");
            for (Queue queue : project.getOutput().getTimeLines().get(timeLinesCount-1).getNodesStatus().getQueues()) {
            	if (queue.getIdNode().equals(idQueue))
            	{
            		if (queue.getAverageDelay() == null)
            			lValue =  new Label("0");
        		    else
        		    	lValue =  new Label(queue.getAverageDelay().toString());
            	}
			}
            hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	    	
	        vlPanel.addComponent(hlTupla);
	        
	        // MAXIMO Y MINIMO
	        
	        Integer maximo=null , minimo=null;
	        
            for (Queue queue : project.getOutput().getTimeLines().get(timeLinesCount-1).getNodesStatus().getQueues()) {
            	if (queue.getIdNode().equals(idQueue))
            	{
            		if (queue.getMinimun() == null)
            			minimo = 0;
        		    else
        		    	minimo = queue.getMinimun();
            		
            		if (queue.getMaximun() == null)
            			maximo = 0;
        		    else
        		    	maximo = queue.getMaximun();
            	}
            }
	        
	        lTicket =  new Label("Tamaño Máximo: ");
	        lValue =  new Label(maximo.toString());
            hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	        vlPanel.addComponent(hlTupla);
	        
	        lTicket =  new Label("Tamaño Mínimo: ");
	        lValue =  new Label(minimo.toString());
            hlTupla = new HorizontalLayout();
	        hlTupla.addComponent(lTicket);
	        hlTupla.addComponent(lValue);
	        hlTupla.setComponentAlignment(lTicket,Alignment.MIDDLE_CENTER);
	        hlTupla.setComponentAlignment(lValue,Alignment.MIDDLE_CENTER);
	        hlTupla.setSpacing(true);
	        vlPanel.addComponent(hlTupla);
	        
	    	//BAR - AMAUNT
	    	
	    	vlPanel.addComponent(new Label("Cantidad de Elementos"));	
	    	final Chart chart = new Chart("ChartQueue");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chart.setWidthJS("750");
	    	else
	    		chart.setWidthJS(""+(15*timeLinesCount));
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
	    	
	    	vlPanel.addComponent(new Label("Promedio de recursos en espera"));	
	    	final Chart chartAmountDelay = new Chart("ChartCombi");
	    	chartAmountDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	double points2[] = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAmountDelay.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartAmountDelay.setWidthJS("750");
	    	else
	    		chartAmountDelay.setWidthJS(""+(15*timeLinesCount));
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
	    	
	    	//CHART - AVERAGE DELAY
	    	
	    	vlPanel.addComponent(new Label("Tiempo de Espera Promedio"));	
	    	final Chart chartAverageDelay = new Chart("ChartCombi");
	    	chartAverageDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	points2 = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAverageDelay.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartAverageDelay.setWidthJS("750");
	    	else
	    		chartAverageDelay.setWidthJS(""+(15*timeLinesCount));
	    	chartAverageDelay.setHeightJS("300"); 
	    	chartAverageDelay.setPointStrokeColor("#fff");
	    	chartAverageDelay.addFillColor("rgba(151,187,205,0.5)");
	    	chartAverageDelay.addStrokeColor("rgba(151,187,205,1)");
	    	chartAverageDelay.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Queue queue : project.getOutput().getTimeLines().get(i).getNodesStatus().getQueues()) {
	            	if (queue.getIdNode().equals(idQueue))
	            	{
	            		if (queue.getAverageDelay() == null)
	            			points2[i] = 0;
	        		    else
	        		    	points2[i] = queue.getAverageDelay();
	            	}
				}	
			}
	    	
	    	chartAverageDelay.addPoints(points2);
	        
	    	vlPanel.addComponent(chartAverageDelay);
	    	
	    	//  DURACION SORTEADA
	    	
	    	vlPanel.addComponent(new Label("Cantidad de Entrada/Salida de Recursos"));	
	    	final Chart chartIO = new Chart("ChartCombi");
	    	chartIO.setType("Bar");
	    	
	    	labels = new String[timeLinesCount];
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartIO.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartIO.setWidthJS("750");
	    	else
	    		chartIO.setWidthJS(""+(15*timeLinesCount));
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
	    	
	    	vlPanel.addComponent(new Label("Cantidad de Mínima/Máxima de Recursos"));	
	    	final Chart chartMM = new Chart("ChartCombi");
	    	chartMM.setType("Bar");
	    	
	    	labels = new String[timeLinesCount];
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartMM.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartMM.setWidthJS("750");
	    	else
	    		chartMM.setWidthJS(""+(15*timeLinesCount));
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
	        //lValue = new Label(idNormal.toString());
	        lValue = new Label(hashNameNodes.get(idNormal));
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
	    	
	    	vlPanel.addComponent(new Label("Cantidad de Entradas"));	
	    	final Chart chart = new Chart("ChartNormal");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chart.setWidthJS("750");
	    	else
	    		chart.setWidthJS(""+(15*timeLinesCount));
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
	    	
	    	vlPanel.addComponent(new Label("Promedio de Duraciones Sorteadas"));	
	    	final Chart chartAmountDelay = new Chart("ChartCombi");
	    	chartAmountDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	double points2[] = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAmountDelay.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartAmountDelay.setWidthJS("750");
	    	else
	    		chartAmountDelay.setWidthJS(""+(15*timeLinesCount));
	    	chartAmountDelay.setHeightJS("300"); 
	    	chartAmountDelay.setPointStrokeColor("#fff");
	    	chartAmountDelay.addFillColor("rgba(151,187,205,0.5)");
	    	chartAmountDelay.addStrokeColor("rgba(151,187,205,1)");
	    	chartAmountDelay.addPointColor("rgba(151,187,205,1)");
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	            for (Normal normal : project.getOutput().getTimeLines().get(i).getNodesStatus().getNormals()) {
	            	if (normal.getIdNode().equals(idNormal))
	            	{
	            		if (normal.getIdNode().equals(idNormal))
		            	{
		            		if (normal.getAmountDelay() == null)
		            			points2[i] = 0;
		        		    else
		        		    {
		        		    	if (normal.getCounterInput() == null || normal.getCounterInput() == 0)
		        		    		points2[i] = 0;
		        		    	else
		        		    		points2[i] = normal.getAmountDelay()/normal.getCounterInput();
		        		    }
		            	}
	            	}
				}	
			}
	    	
	    	chartAmountDelay.addPoints(points2);
	        
	    	vlPanel.addComponent(chartAmountDelay);
	    	
	    	// MINIMA Y MINIMA DURACION SORTEADA
	    	
	    	vlPanel.addComponent(new Label("Cantidad de Mínima/Máxima de Recursos"));	
	    	final Chart chartMM = new Chart("ChartCombi");
	    	chartMM.setType("Bar");
	    	
	    	labels = new String[timeLinesCount];
	    	points = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartMM.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartMM.setWidthJS("750");
	    	else
	    		chartMM.setWidthJS(""+(15*timeLinesCount));
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
	        //lValue = new Label(idFunction.toString());
	        lValue = new Label(hashNameNodes.get(idFunction));
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
	    	
	    	vlPanel.addComponent(new Label("Cantida de Elementos sin Combinar"));	
	    	final Chart chart = new Chart("ChartFunction");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chart.setWidthJS("750");
	    	else
	    		chart.setWidthJS(""+(15*timeLinesCount));
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
	        //lValue = new Label(idCounter.toString());
	        lValue = new Label(hashNameNodes.get(idCounter));
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
	    	
	    	vlPanel.addComponent(new Label("Productividad por Delta T"));	
	    	final Chart chart = new Chart("ChartFunction");
	    	chart.setType("Bar");
	    	
	    	String labels[]= new String[timeLinesCount];
	    	double points[]= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chart.setWidthJS("750");
	    	else
	    		chart.setWidthJS(""+(15*timeLinesCount));
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
	    	
	    	vlPanel.addComponent(new Label("Productividad Total"));	
	    	final Chart chart2 = new Chart("ChartFunction");
	    	chart2.setType("Bar");
	    	
	    	labels= new String[timeLinesCount];
	    	points= new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	chart2.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chart2.setWidthJS("750");
	    	else
	    		chart2.setWidthJS(""+(15*timeLinesCount));
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
	    	
	    	vlPanel.addComponent(new Label("Productividad Promedio"));	
	    	final Chart chartAmountDelay = new Chart("ChartCounter");
	    	chartAmountDelay.setType("Line");
	    	
	    	labels = new String[timeLinesCount];
	    	double points2[] = new double[timeLinesCount];
	    	
	    	for (int i = 0; i < timeLinesCount; i++) {
	    		labels[i] = String.valueOf(i);
			}
	    	
	    	chartAmountDelay.setLabels(labels);
	    	
	    	if (timeLinesCount < 50)
	    		chartAmountDelay.setWidthJS("750");
	    	else
	    		chartAmountDelay.setWidthJS(""+(15*timeLinesCount));
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