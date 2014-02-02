package com.example.botqueueweb.windows;

import java.util.ArrayList;
import java.util.List;

import org.vaadin.applet.AppletIntegration;

import com.example.botqueueweb.dto.input.Combi;
import com.mongodb.BasicDBList;
import com.mongodb.BasicDBObject;
import com.mongodb.DBObject;
import com.mongodb.util.JSON;
import com.vaadin.data.Property;
import com.vaadin.data.Property.ValueChangeEvent;
import com.vaadin.ui.AbsoluteLayout;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
import com.vaadin.ui.CheckBox;
import com.vaadin.ui.ComboBox;
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

public class CombiWindow extends Window {

    public CombiWindow(final DBObject combi,DBObject dbProbBranch, final AppletIntegration applet, boolean isFull) {
    	
    	this.setCaption("Combi");
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
        if (isFull)	lNombre.setWidth("110");
        else		lNombre.setWidth("90");
        final TextField tfName = new TextField();
        if (isFull)	tfName.setWidth("310");
        else		tfName.setWidth("330");
        if (combi.get("name")!=null)
        	tfName.setValue(combi.get("name").toString());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);

     // Elementos Modificacion //
    	
    	final HorizontalLayout hlMinimun = new HorizontalLayout();
        Label lMinimun = new Label("Minimo: ");
        lMinimun.setWidth("110");
        final TextField tfMinimun = new TextField();
        tfMinimun.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("minimun")!=null)
        	tfMinimun.setValue(((BasicDBObject)combi.get("delay")).get("minimun").toString());
        hlMinimun.addComponent(lMinimun);
        hlMinimun.addComponent(tfMinimun);
        hlMinimun.setVisible(false);
        
        final HorizontalLayout hlMaximun = new HorizontalLayout();
        Label lMaximun = new Label("Maximo: ");
        lMaximun.setWidth("110");
        final TextField tfMaximun = new TextField();
        tfMaximun.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("maximun")!=null)
        	tfMaximun.setValue(((BasicDBObject)combi.get("delay")).get("maximun").toString());
        hlMaximun.addComponent(lMaximun);
        hlMaximun.addComponent(tfMaximun);
        hlMaximun.setVisible(false);
        
        final HorizontalLayout hlShapeAlpha = new HorizontalLayout();
        Label lShapeAlpha = new Label("Shape Alpha: ");
        lShapeAlpha.setWidth("110");
        final TextField tfShapeAlpha = new TextField();
        tfShapeAlpha.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("shapeAlpha")!=null)
        	tfShapeAlpha.setValue(((BasicDBObject)combi.get("delay")).get("shapeAlpha").toString());
        hlShapeAlpha.addComponent(lShapeAlpha);
        hlShapeAlpha.addComponent(tfShapeAlpha);
        hlShapeAlpha.setVisible(false);
        
        final HorizontalLayout hlShapeBeta = new HorizontalLayout();
        Label lShapeBeta = new Label("Shape Beta: ");
        lShapeBeta.setWidth("110");
        final TextField tfShapeBeta = new TextField();
        tfShapeBeta.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("shapeBeta")!=null)
        	tfShapeBeta.setValue(((BasicDBObject)combi.get("delay")).get("shapeBeta").toString());
        hlShapeBeta.addComponent(lShapeBeta);
        hlShapeBeta.addComponent(tfShapeBeta);
        hlShapeBeta.setVisible(false);
        
        final HorizontalLayout hlConstant = new HorizontalLayout();
        Label lConstant = new Label("Constante: ");
        lConstant.setWidth("110");
        final TextField tfConstant = new TextField();
        tfConstant.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("constant")!=null)
        	tfConstant.setValue(((BasicDBObject)combi.get("delay")).get("constant").toString());
        hlConstant.addComponent(lConstant);
        hlConstant.addComponent(tfConstant);
        hlConstant.setVisible(false);
        
        final HorizontalLayout hlLambda = new HorizontalLayout();
        Label lLambda = new Label("Lambda: ");
        lLambda.setWidth("110");
        final TextField tfLambda = new TextField();
        tfLambda.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("lambda")!=null)
        	tfLambda.setValue(((BasicDBObject)combi.get("delay")).get("lambda").toString());
        hlLambda.addComponent(lLambda);
        hlLambda.addComponent(tfLambda);
        hlLambda.setVisible(false);
        
        final HorizontalLayout hlEscale = new HorizontalLayout();
        Label lEscale = new Label("Escale: ");
        lEscale.setWidth("110");
        final TextField tfEscale = new TextField();
        tfEscale.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("escale")!=null)
        	tfEscale.setValue(((BasicDBObject)combi.get("delay")).get("escale").toString());
        hlEscale.addComponent(lEscale);
        hlEscale.addComponent(tfEscale);
        hlEscale.setVisible(false);
        
        final HorizontalLayout hlShape = new HorizontalLayout();
        Label lShape = new Label("Shape: ");
        lShape.setWidth("110");
        final TextField tfShape = new TextField();
        tfShape.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("shape")!=null)
        	tfShape.setValue(((BasicDBObject)combi.get("delay")).get("shape").toString());
        hlShape.addComponent(lShape);
        hlShape.addComponent(tfShape);
        hlShape.setVisible(false);
        
        final HorizontalLayout hlMean = new HorizontalLayout();
        Label lMean = new Label("Mean: ");
        lMean.setWidth("110");
        final TextField tfMean = new TextField();
        tfMean.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("mean")!=null)
        	tfMean.setValue(((BasicDBObject)combi.get("delay")).get("mean").toString());
        hlMean.addComponent(lMean);
        hlMean.addComponent(tfMean);
        hlMean.setVisible(false);
        
        final HorizontalLayout hlVariance = new HorizontalLayout();
        Label lVariance = new Label("Variansa: ");
        lVariance.setWidth("110");
        final TextField tfVariance = new TextField();
        tfVariance.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("variance")!=null)
        	tfVariance.setValue(((BasicDBObject)combi.get("delay")).get("variance").toString());
        hlVariance.addComponent(lVariance);
        hlVariance.addComponent(tfVariance);
        hlVariance.setVisible(false);
        
        
        final HorizontalLayout hlLeast = new HorizontalLayout();
        Label lLeast = new Label("Least: ");
        lLeast.setWidth("110");
        final TextField tfLeast = new TextField();
        tfLeast.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("least")!=null)
        	tfLeast.setValue(((BasicDBObject)combi.get("delay")).get("least").toString());
        hlLeast.addComponent(lLeast);
        hlLeast.addComponent(tfLeast);
        hlLeast.setVisible(false);
        	        
        final HorizontalLayout hlHighest = new HorizontalLayout();
        Label lHighest = new Label("Highest: ");
        lHighest.setWidth("110");
        final TextField tfHighest = new TextField();
        tfHighest.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("highest")!=null)
        	tfHighest.setValue(((BasicDBObject)combi.get("delay")).get("highest").toString());
        hlHighest.addComponent(lHighest);
        hlHighest.addComponent(tfHighest);
        hlHighest.setVisible(false);
        
        final HorizontalLayout hlMode = new HorizontalLayout();
        Label lMode = new Label("Moda: ");
        lMode.setWidth("110");
        final TextField tfMode = new TextField();
        tfMode.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("mode")!=null)
        	tfMode.setValue(((BasicDBObject)combi.get("delay")).get("mode").toString());
        hlMode.addComponent(lMode);
        hlMode.addComponent(tfMode);
        hlMode.setVisible(false);
    	
        final HorizontalLayout hlSeed = new HorizontalLayout();
        Label lSeed = new Label("Semilla: ");
        lSeed.setWidth("110");
        final TextField tfSeed = new TextField();
        tfSeed.setWidth("310");
        if (combi.get("delay")!=null && ((BasicDBObject)combi.get("delay")).get("seed")!=null)
        	tfSeed.setValue(((BasicDBObject)combi.get("delay")).get("seed").toString());
        hlSeed.addComponent(lSeed);
        hlSeed.addComponent(tfSeed);
        hlSeed.setVisible(false);
        
        final ComboBox cbDelay = new ComboBox();
        
        final HorizontalLayout hlSpace = new HorizontalLayout();
        
        if (isFull)
        {
	        final HorizontalLayout hlSpace = new HorizontalLayout();
	        hlSpace.setHeight("410px");
	        
        	cbDelay.addItem("---");
        	cbDelay.addItem("Deterministica");
        	cbDelay.addItem("Uniforme");
        	cbDelay.addItem("Exponencial");
        	cbDelay.addItem("Normal");
        	cbDelay.addItem("Beta");
        	cbDelay.addItem("Triangular");
        	cbDelay.addListener(new Property.ValueChangeListener() {
				
				@Override
				public void valueChange(ValueChangeEvent event) {
					if (cbDelay.getValue()!=null)
					{
						if (cbDelay.getValue().toString().equalsIgnoreCase("Deterministica"))
						{
							hlMinimun.setVisible(false);
					        hlMaximun.setVisible(false);
					        hlShapeAlpha.setVisible(false);
					        hlShapeBeta.setVisible(false);
					        hlConstant.setVisible(true);
					        hlLambda.setVisible(false);
					        hlEscale.setVisible(false);
					        hlShape.setVisible(false);
					        hlMean.setVisible(false);
					        hlVariance.setVisible(false);
					        hlLeast.setVisible(false);
					        hlHighest.setVisible(false);
					        hlMode.setVisible(false);
					        hlSeed.setVisible(false);
					        
					        hlSpace.setHeight("370px"); //-40
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Uniforme"))
						{
							hlMinimun.setVisible(false);
					        hlMaximun.setVisible(false);
					        hlShapeAlpha.setVisible(false);
					        hlShapeBeta.setVisible(false);
					        hlConstant.setVisible(false);
					        hlLambda.setVisible(false);
					        hlEscale.setVisible(false);
					        hlShape.setVisible(false);
					        hlMean.setVisible(false);
					        hlVariance.setVisible(false);
					        hlLeast.setVisible(true);
							hlHighest.setVisible(true);
					        hlMode.setVisible(false);
					        hlSeed.setVisible(true);
					        
					        hlSpace.setHeight("290px"); //-120
					    }
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Exponencial"))
						{
							hlMinimun.setVisible(false);
					        hlMaximun.setVisible(false);
					        hlShapeAlpha.setVisible(false);
					        hlShapeBeta.setVisible(false);
					        hlConstant.setVisible(false);
					        hlLambda.setVisible(true);
					        hlEscale.setVisible(false);
					        hlShape.setVisible(false);
					        hlMean.setVisible(false);
					        hlVariance.setVisible(false);
					        hlLeast.setVisible(false);
					        hlHighest.setVisible(false);
					        hlMode.setVisible(false);
					        hlSeed.setVisible(true);
					        
					        hlSpace.setHeight("330px"); //-80
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Normal"))
						{
							hlMinimun.setVisible(false);
					        hlMaximun.setVisible(false);
					        hlShapeAlpha.setVisible(false);
					        hlShapeBeta.setVisible(false);
					        hlConstant.setVisible(false);
					        hlLambda.setVisible(false);
					        hlEscale.setVisible(false);
					        hlShape.setVisible(false);
					        hlMean.setVisible(true);
							hlVariance.setVisible(true);
					        hlLeast.setVisible(false);
					        hlHighest.setVisible(false);
					        hlMode.setVisible(false);
					        hlSeed.setVisible(true);
					        
					        hlSpace.setHeight("290px"); //-120
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Beta"))
						{
							hlMinimun.setVisible(true);
					        hlMaximun.setVisible(true);
					        hlShapeAlpha.setVisible(true);
					        hlShapeBeta.setVisible(true);
					        hlConstant.setVisible(false);
					        hlLambda.setVisible(false);
					        hlEscale.setVisible(false);
					        hlShape.setVisible(false);
					        hlMean.setVisible(false);
					        hlVariance.setVisible(false);
					        hlLeast.setVisible(false);
					        hlHighest.setVisible(false);
					        hlMode.setVisible(false);
					        hlSeed.setVisible(true);
					        
					        hlSpace.setHeight("210px"); //-200 (40*5)
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Triangular"))
						{
							hlMinimun.setVisible(false);
					        hlMaximun.setVisible(false);
					        hlShapeAlpha.setVisible(false);
					        hlShapeBeta.setVisible(false);
					        hlConstant.setVisible(false);
					        hlLambda.setVisible(false);
					        hlEscale.setVisible(false);
					        hlShape.setVisible(false);
					        hlMean.setVisible(false);
					        hlVariance.setVisible(false);
					        hlLeast.setVisible(true);
					        hlHighest.setVisible(true);
					        hlMode.setVisible(true);
					        hlSeed.setVisible(true);
					        
					        hlSpace.setHeight("250px"); //-160
						}
					}
				}
			});
        	HorizontalLayout hlDelay = new HorizontalLayout();
	        Label lDelay = new Label("Distribucion: ");
	        lDelay.setWidth("110");
	        hlDelay.addComponent(lDelay);
	        hlDelay.addComponent(cbDelay);
        	
        	subContent.addComponent(hlDelay);
        	
        	subContent.addComponent(hlMinimun);
	        subContent.addComponent(hlMaximun);
	        subContent.addComponent(hlShapeAlpha);
	        subContent.addComponent(hlShapeBeta);
	        subContent.addComponent(hlConstant);
	        subContent.addComponent(hlLambda);
	        subContent.addComponent(hlEscale);
	        subContent.addComponent(hlShape);
	        subContent.addComponent(hlMean);
	        subContent.addComponent(hlVariance);
	        subContent.addComponent(hlLeast);
	        subContent.addComponent(hlHighest);
	        subContent.addComponent(hlMode);
	        subContent.addComponent(hlSeed);
        }
        
        //
        
        if (combi.get("delay")!=null)
        {
        	if (((BasicDBObject)combi.get("delay")).get("distribution") == null)
        		cbDelay.setValue("---");
        	else if (((BasicDBObject)combi.get("delay")).get("distribution").toString().equalsIgnoreCase("deterministic"))
        		cbDelay.setValue("Deterministica");
        	else if (((BasicDBObject)combi.get("delay")).get("distribution").toString().equalsIgnoreCase("uniform"))
        		cbDelay.setValue("Uniforme");
        	else if (((BasicDBObject)combi.get("delay")).get("distribution").toString().equalsIgnoreCase("exponential"))
				cbDelay.setValue("Exponencial");
        	else if (((BasicDBObject)combi.get("delay")).get("distribution").toString().equalsIgnoreCase("normal"))
				cbDelay.setValue("Normal");
        	else if (((BasicDBObject)combi.get("delay")).get("distribution").toString().equalsIgnoreCase("beta"))
				cbDelay.setValue("Beta");
        	else if (((BasicDBObject)combi.get("delay")).get("distribution").toString().equalsIgnoreCase("triangular"))
				cbDelay.setValue("Triangular");
        }
        
        HorizontalLayout hlProbBranch = new HorizontalLayout();
        hlProbBranch.setSpacing(true);
        final CheckBox cbProbBranch = new CheckBox();
        cbProbBranch.setCaption("Probabilistic Branch");
        if ((BasicDBList)combi.get("probabilisticBranch")!=null && ((BasicDBList)combi.get("probabilisticBranch")).size()>0)
        	cbProbBranch.setValue(true);
        hlProbBranch.addComponent(cbProbBranch);
        subContent.addComponent(hlProbBranch);
        
        //DBLIST
        
        //HorizontalLayout hlProbBranchItem = new HorizontalLayout();
        
        final List<TextField> ltfProbBranchItem = new ArrayList();
        BasicDBList probBranchList = (BasicDBList) dbProbBranch.get("nameList");
    	ArrayList<BasicDBObject> probBranchArray = (ArrayList) probBranchList;
    	
        for (int i=0 ; i < ((BasicDBList)combi.get("followers")).size() ; i++)
        {
        	HorizontalLayout hlProbBranchItem = new HorizontalLayout();
        	hlProbBranchItem.setSpacing(true);
        	
        	
        	Label lProbBranchItem = null;
        	if (probBranchArray!=null && probBranchArray.size()>0)
	        	for (BasicDBObject probBranch :probBranchArray)
	        	{
		        	if (((BasicDBList)combi.get("followers")).get(i).toString().equalsIgnoreCase(probBranch.get("id").toString()))
		        		lProbBranchItem = new Label(probBranch.get("name").toString());
	        	}
        	else
        		lProbBranchItem = new Label("");
        		
        	TextField tfProbBranchItem = new TextField();
        	
        	if ((BasicDBList)combi.get("probabilisticBranch")!=null && ((BasicDBList)combi.get("probabilisticBranch")).size()>0)
        		tfProbBranchItem.setValue(((BasicDBList)combi.get("probabilisticBranch")).get(i).toString());

        	ltfProbBranchItem.add(tfProbBranchItem);
        	hlProbBranchItem.addComponent(lProbBranchItem);
        	hlProbBranchItem.addComponent(tfProbBranchItem);
        	subContent.addComponent(hlProbBranchItem);
        }
        
        subContent.addComponent(hlSpace);
        
        //BOTONES
        
	    //AbsoluteLayout alBotones = new AbsoluteLayout();
	    HorizontalLayout hlBotones = new HorizontalLayout();
	    hlBotones.setSpacing(true);
	    hlBotones.setHeight("100%");
	    
        Button bAceptar = new Button("Aceptar");
        
        if(isFull)
        {
        	//TODO: applet.executeCommand("editNode", arrayParams);
        	//con json completo de Queue
        	bAceptar.addClickListener(new ClickListener() {
	            @Override
				public void buttonClick(ClickEvent event) {
	            	String[] arrayParams = new String[18];
	            	
	            	if (cbDelay.getValue().toString().equalsIgnoreCase("Deterministica"))
	            		arrayParams[0] = "deterministic";
	            	if (cbDelay.getValue().toString().equalsIgnoreCase("Uniforme"))
	            		arrayParams[0] = "uniform";
	            	if (cbDelay.getValue().toString().equalsIgnoreCase("Exponencial"))
	            		arrayParams[0] = "exponential";
	    			if (cbDelay.getValue().toString().equalsIgnoreCase("Normal"))
	    				arrayParams[0] = "normal";
	    			if (cbDelay.getValue().toString().equalsIgnoreCase("Beta"))
	    				arrayParams[0] = "beta";
	    			if (cbDelay.getValue().toString().equalsIgnoreCase("Triangular"))
	    				arrayParams[0] = "triangular";

	            	arrayParams[1] = tfSeed.getValue();
	            	arrayParams[2] = tfLeast.getValue();
	            	arrayParams[3] = tfHighest.getValue();
	            	arrayParams[4] = tfConstant.getValue();
	            	arrayParams[5] = tfMean.getValue();
	            	arrayParams[6] = tfVariance.getValue();
	            	arrayParams[7] = tfLambda.getValue();
	            	arrayParams[8] = tfMode.getValue();
	            	arrayParams[9] = tfMinimun.getValue();
	            	arrayParams[10] = tfMaximun.getValue();
	            	arrayParams[11] = tfShapeAlpha.getValue();
	            	arrayParams[12] = tfShapeBeta.getValue();
	            	arrayParams[13] = tfShape.getValue();
	            	arrayParams[14] = tfEscale.getValue();
	            	arrayParams[15] = "no"; //"yes"
	            	arrayParams[16] = tfName.getValue();
	            	arrayParams[17] = ""; 
			        
	            	if (cbProbBranch.getValue())
	            	{
		            	String ProbBranch = "";
		            	for(int i=0 ;i < ltfProbBranchItem.size() ;i++)
		            	{
		            		ProbBranch += ((BasicDBList)combi.get("followers")).get(i).toString();
		            		ProbBranch += ",";
		            		ProbBranch += ltfProbBranchItem.get(i).getValue();
		            		if (i<ltfProbBranchItem.size()-1)
		            			ProbBranch += ",";
		            	}
		            	
		            	arrayParams[15] = "yes";
		            	arrayParams[17] = ProbBranch;
	            	}
	            	
	        	    /*String[] arrayParams = new String[4];
	        	    arrayParams[0] = tfName.getValue();
	        	    arrayParams[1] = tfResource.getValue();
	        	    arrayParams[2] = tfFixedCost.getValue();
	        	    arrayParams[3] = tfVariableCost.getValue();*/
	        	    
	        	    applet.executeCommand("editNode", arrayParams);
	        	    close();
				}
			});
        }
        else
        {
	        bAceptar.addClickListener(new ClickListener() {
	            @Override
				public void buttonClick(ClickEvent event) {
	            	String[] arrayParams = new String[2];
	        	    arrayParams[0] = "1";
	        	    arrayParams[1] = tfName.getValue();		
	        	    applet.executeCommand("createNode", arrayParams);
	        	    close();
				}
			});
        }
        
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
    }

    public void addComponent(Component c) {
        ((CssLayout) getContent()).addComponent(c);
    }

}
