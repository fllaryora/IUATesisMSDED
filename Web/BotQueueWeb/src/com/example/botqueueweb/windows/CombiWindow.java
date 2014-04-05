package com.example.botqueueweb.windows;

import java.util.ArrayList;
import java.util.List;

import org.vaadin.applet.AppletIntegration;

import com.mongodb.BasicDBList;
import com.mongodb.BasicDBObject;
import com.mongodb.DBObject;
import com.vaadin.data.Property;
import com.vaadin.data.Property.ValueChangeEvent;
import com.vaadin.data.Property.ValueChangeListener;
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

	private static final long serialVersionUID = 1L;

	@SuppressWarnings({ "deprecation", "rawtypes" })
	public CombiWindow(final DBObject combi,DBObject dbProbBranch, final AppletIntegration applet, boolean isFull) {
    	
    	//CONFIGURACION INICIAL
    	this.setCaption("Combi");
    	this.setModal(true);
    	this.setResizable(false);
    	
    	//PANEL
    	Panel bodyPanel = new Panel();
    	bodyPanel.setWidth("100%");
    	bodyPanel.setHeight("100%");
        VerticalLayout subContent = new VerticalLayout();
        subContent.setMargin(true);
        subContent.setSpacing(true);
        bodyPanel.setContent(subContent);
        setContent(bodyPanel);
        
        //NOMBRE COMBI
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label("Nombre: ");
        if (isFull)	lNombre.setWidth("110");
        else		lNombre.setWidth("90");
        final TextField tfName = new TextField();
        if (isFull)	tfName.setWidth("310");
        else		tfName.setWidth("338");
        if (combi.get("name")!=null)
        	tfName.setValue(combi.get("name").toString());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);

        //ELEMENTOS DE COMBOBOX
    	
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
        
        //PROBABILISTIC BRANCH (COMBO DEFINICION)
        final CheckBox cbProbBranch = new CheckBox();
        //cbProbBranch.setHeight("32px");
        if( combi.get("followers")!=null && ((BasicDBList)combi.get("followers")).size()>1) {
	        if ((BasicDBList)combi.get("probabilisticBranch")!=null && ((BasicDBList)combi.get("probabilisticBranch")).size()>0)
	        	cbProbBranch.setValue(true);
        }
        
        //COMBO
        final ComboBox cbDelay = new ComboBox();
        cbDelay.setWidth("310");
        final HorizontalLayout hlSpace = new HorizontalLayout();
        final Integer hlSpaceHeight = 410;
        final Integer hlSpaceHeightItem = 40;
        
        if (isFull)
        {
        	cbDelay.addItem("Deterministica");
        	cbDelay.addItem("Uniforme");
        	cbDelay.addItem("Exponencial");
        	cbDelay.addItem("Normal");
        	cbDelay.addItem("Beta");
        	cbDelay.addItem("Triangular");

        	cbDelay.addListener(new Property.ValueChangeListener() {
				
        		public void setSpaceHeight(Integer space) {
        			if (space==null)
        				hlSpace.setHeight("0px");
        			else
        				hlSpace.setHeight( space.toString()+"px");
        		}
        		
        		public void setVisibleComponentsDelay(boolean boolMinimun, boolean boolMaximun, boolean boolShapeAlpha, boolean boolShapeBeta, boolean  boolConstant,
									        		  boolean boolLambda,  boolean boolEscale,  boolean boolShape,      boolean boolMean,      boolean boolVariance,
									        		  boolean boolLeast,   boolean boolHighest, boolean boolMode,       boolean boolSeed) 
        		{
    		        hlMinimun.setVisible(boolMinimun);
    		        hlMaximun.setVisible(boolMaximun);
    		        hlShapeAlpha.setVisible(boolShapeAlpha);
    		        hlShapeBeta.setVisible(boolShapeBeta);
    		        hlConstant.setVisible(boolConstant);
    		        hlLambda.setVisible(boolLambda);
    		        hlEscale.setVisible(boolEscale);
    		        hlShape.setVisible(boolShape);
    		        hlMean.setVisible(boolMean);
    		        hlVariance.setVisible(boolVariance);
    		        hlLeast.setVisible(boolLeast);
    		        hlHighest.setVisible(boolHighest);
    		        hlMode.setVisible(boolMode);
    		        hlSeed.setVisible(boolSeed);
        		}
        		 
				private static final long serialVersionUID = 1L;
				@Override
				public void valueChange(ValueChangeEvent event) {
					if (cbDelay.getValue()!=null)
					{
						int spaceComponents = 0;
						int spaceCheck = 0;
						int spaceFollowers = 0;
						
						if(((BasicDBList)combi.get("followers")).size()>1) {
							spaceCheck = 1;
							if (cbProbBranch.getValue())
								spaceFollowers = ((BasicDBList)combi.get("followers")).size();
							else
								spaceFollowers = 0;
						}
						
						if (cbDelay.getValue().toString().equalsIgnoreCase("Deterministica")) {
							setVisibleComponentsDelay(false,false,false,false,true,false,false,false,false,false,false,false,false,false);
					        spaceComponents = 1;
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Uniforme")) {
							setVisibleComponentsDelay(false,false,false,false,false,false,false,false,false,false,true,true,false,true);
					        spaceComponents = 3;
					    }
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Exponencial")) {
							setVisibleComponentsDelay(false,false,false,false,false,true,false,false,false,false,false,false,false,true);
					        spaceComponents = 2;
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Normal")) {
							setVisibleComponentsDelay(false,false,false,false,false,false,false,false,true,true,false,false,false,true);
					        spaceComponents = 3;
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Beta")) {
							setVisibleComponentsDelay(true,true,true,true,false,false,false,false,false,false,false,false,false,true);
					        spaceComponents = 5;
						}
						else if (cbDelay.getValue().toString().equalsIgnoreCase("Triangular")) {
							setVisibleComponentsDelay(false,false,false,false,false,false,false,false,false,false,true,true,true,true);
					        spaceComponents = 4;
						}
						
						if( ( hlSpaceHeight - (hlSpaceHeightItem * (spaceComponents + spaceCheck + spaceFollowers) ) ) > 0)
			        		setSpaceHeight( hlSpaceHeight - (hlSpaceHeightItem * (spaceComponents + spaceCheck + spaceFollowers) ) );
				        else
				        	setSpaceHeight(0);
					}
				}
			});
        	cbDelay.setImmediate(true);
        	
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
        
        //PROBABILISTIC BRANCH (AGREGAR COMBO A INTERFACE)
        if( combi.get("followers")!=null && ((BasicDBList)combi.get("followers")).size()>1)
        {
	        HorizontalLayout hlProbBranch = new HorizontalLayout();
	        hlProbBranch.setSpacing(true);
	        cbProbBranch.setCaption("Probabilistic Branch");
	        hlProbBranch.addComponent(cbProbBranch);
	        subContent.addComponent(hlProbBranch);
        }
        
        //ITEMS PROBABILISTIC BRANCH
        final List<TextField> ltfProbBranchItem = new ArrayList<TextField>();
        final List<HorizontalLayout> hlProbBranchItems = new ArrayList<HorizontalLayout>();
        if (dbProbBranch!=null)
        {
	        BasicDBList probBranchList = (BasicDBList) dbProbBranch.get("nameList");
	    	@SuppressWarnings("unchecked")
			ArrayList<BasicDBObject> probBranchArray = (ArrayList) probBranchList;
	    	
	        for (int i=0 ; i < ((BasicDBList)combi.get("followers")).size() ; i++)
	        {
	        	HorizontalLayout hlProbBranchItem = new HorizontalLayout();
	        	hlProbBranchItem.setSpacing(true);
	        	
	        	//LABEL
	        	Label lProbBranchItem = null;
	        	if (probBranchArray!=null && probBranchArray.size()>0)
		        	for (BasicDBObject probBranch :probBranchArray){
			        	if (((BasicDBList)combi.get("followers")).get(i).toString().equalsIgnoreCase(probBranch.get("id").toString()))
			        		lProbBranchItem = new Label(probBranch.get("name").toString());
		        	}
	        	else
	        		lProbBranchItem = new Label("");
	        	lProbBranchItem.setWidth("98");
	        	
	        	//TEXTINPUT
	        	TextField tfProbBranchItem = new TextField();
	        	tfProbBranchItem.setWidth("310");
	        	if ((BasicDBList)combi.get("probabilisticBranch")!=null && ((BasicDBList)combi.get("probabilisticBranch")).size()>0)
	        		tfProbBranchItem.setValue(((BasicDBList)combi.get("probabilisticBranch")).get(i).toString());
	
	        	ltfProbBranchItem.add(tfProbBranchItem);
	        	hlProbBranchItem.addComponent(lProbBranchItem);
	        	hlProbBranchItem.addComponent(tfProbBranchItem);
	        	hlProbBranchItems.add(hlProbBranchItem);
	        	subContent.addComponent(hlProbBranchItem);
	        }
        }
        
        //SETEO INICIAL DE DELAY (COMBOBOX)
        if (combi.get("delay")!=null)
        {
        	if (((BasicDBObject)combi.get("delay")).get("distribution") == null) //TODO: VALIDACION <> --- // cambio a este no muestre elementos debajo
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
        
        //CALCULO DE ESPACIO POR CHECK PROB BRANCH
        if (dbProbBranch!=null)
        {
	        cbProbBranch.setImmediate(true);
	        cbProbBranch.addValueChangeListener(new ValueChangeListener() {
				private static final long serialVersionUID = 1L;
				
				public void setSpaceHeight(Integer space) {
        			if (space==null)
        				hlSpace.setHeight("0px");
        			else
        				hlSpace.setHeight( space.toString()+"px");
        		}
				
				public void valueChange(ValueChangeEvent event)
				{
					if(cbProbBranch.getValue()==true) {
						for (HorizontalLayout horizontalLayout : hlProbBranchItems) {
							horizontalLayout.setVisible(true);
						}
					}
					else {
						for (HorizontalLayout horizontalLayout : hlProbBranchItems) {
							horizontalLayout.setVisible(false);
						}
					}
					
					int spaceComponents = 0;
					int spaceCheck = 0;
					int spaceFollowers = 0;
					
					if(((BasicDBList)combi.get("followers")).size()>1) {
						spaceCheck = 1;
						if (cbProbBranch.getValue())
							spaceFollowers = ((BasicDBList)combi.get("followers")).size();
						else
							spaceFollowers = 0;
					}
					
					if (cbDelay.getValue().toString().equalsIgnoreCase("Deterministica")) {
				        spaceComponents = 1;
					}
					else if (cbDelay.getValue().toString().equalsIgnoreCase("Uniforme")) {					        
				        spaceComponents = 3;
				    }
					else if (cbDelay.getValue().toString().equalsIgnoreCase("Exponencial")) {				        
				        spaceComponents = 2;
					}
					else if (cbDelay.getValue().toString().equalsIgnoreCase("Normal")) {					        
				        spaceComponents = 3;
					}
					else if (cbDelay.getValue().toString().equalsIgnoreCase("Beta")) {
				        spaceComponents = 5;
					}
					else if (cbDelay.getValue().toString().equalsIgnoreCase("Triangular")) {
				        spaceComponents = 4;
					}
					
			        if( ( hlSpaceHeight - (hlSpaceHeightItem * (spaceComponents + spaceCheck + spaceFollowers) ) ) > 0)
		        		setSpaceHeight( hlSpaceHeight - (hlSpaceHeightItem * (spaceComponents + spaceCheck + spaceFollowers) ) );
			        else
			        	setSpaceHeight(0);
	            }
	        });
	        
	        if (cbProbBranch.getValue()) cbProbBranch.setValue(false);
	        else						 cbProbBranch.setValue(true);
	        
	        if (cbProbBranch.getValue()) cbProbBranch.setValue(false);
	        else						 cbProbBranch.setValue(true);
        }
        
        if (isFull)
        	subContent.addComponent(hlSpace);
        
        //BOTONES
	    HorizontalLayout hlBotones = new HorizontalLayout();
	    hlBotones.setSpacing(true);
	    hlBotones.setHeight("100%");
	    
        Button bAceptar = new Button("Aceptar");
        if(isFull)
        {
        	bAceptar.addClickListener(new ClickListener() {
				private static final long serialVersionUID = 1L;
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
	            	arrayParams[15] = "no";
	            	arrayParams[16] = tfName.getValue();
	            	arrayParams[17] = ""; 
			        
	            	if (cbProbBranch.getValue() && ltfProbBranchItem.size()>0)
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
	        	    
	        	    applet.executeCommand("editNode", arrayParams);
	        	    close();
				}
			});
        }
        else
        {
	        bAceptar.addClickListener(new ClickListener() {
				private static final long serialVersionUID = 1L;
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
