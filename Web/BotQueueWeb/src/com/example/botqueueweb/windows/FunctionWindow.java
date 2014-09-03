package com.example.botqueueweb.windows;

import java.util.ArrayList;
import java.util.List;

import com.example.botqueueweb.dto.Project;
import com.mongodb.BasicDBList;
import com.mongodb.BasicDBObject;
import com.mongodb.DBObject;

import org.vaadin.applet.AppletIntegration;

import com.vaadin.data.Property.ValueChangeEvent;
import com.vaadin.data.Property.ValueChangeListener;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
import com.vaadin.ui.CheckBox;
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

public class FunctionWindow extends Window {

	private static final long serialVersionUID = 1L;

	public FunctionWindow(final DBObject function,DBObject dbProbBranch, final AppletIntegration applet, boolean isFull, Project project) {
    	
		//CONFIGURACION INICIAL
    	this.setCaption("Función");
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
        
        //NOMBRE
        HorizontalLayout hlNombre = new HorizontalLayout();
        Label lNombre = new Label("Nombre: ");
        if (isFull)	lNombre.setWidth("110");
        else		lNombre.setWidth("90");
        final TextField tfName = new TextField();
        if (isFull)	tfName.setWidth("310");
        else		tfName.setWidth("338");
        if (function.get("name")!=null)
        	tfName.setValue(function.get("name").toString());
        hlNombre.addComponent(lNombre);
        hlNombre.addComponent(tfName);
        subContent.addComponent(hlNombre);

        //ELEMENTOS MODIFICACION
        
    	final HorizontalLayout hlInput = new HorizontalLayout();
        Label lMinimun = new Label("Input: ");
        lMinimun.setWidth("110");
        final TextField tfInput = new TextField();
        tfInput.setWidth("310");
        if (function.get("input")!=null)
        	tfInput.setValue(function.get("input").toString());
        hlInput.addComponent(lMinimun);
        hlInput.addComponent(tfInput);
        
        final HorizontalLayout hlOutput = new HorizontalLayout();
        Label lMaximun = new Label("Output: ");
        lMaximun.setWidth("110");
        final TextField tfOutput = new TextField();
        tfOutput.setWidth("310");
        if (function.get("output")!=null)
        	tfOutput.setValue(function.get("output").toString());
        hlOutput.addComponent(lMaximun);
        hlOutput.addComponent(tfOutput);
        
        if (isFull)
        {
	        subContent.addComponent(hlInput);
	        subContent.addComponent(hlOutput);
        }
        
        final HorizontalLayout hlSpace = new HorizontalLayout();
        final Integer hlSpaceHeight = 370;
        final Integer hlSpaceHeightItem = 40;
        
        //PROBABILISTIC BRANCH
        final CheckBox cbProbBranch = new CheckBox();
        //cbProbBranch.setHeight("28px");
        if( function.get("followers")!=null && ((BasicDBList)function.get("followers")).size()>1)
        {
	        HorizontalLayout hlProbBranch = new HorizontalLayout();
	        hlProbBranch.setSpacing(true);
	        cbProbBranch.setCaption("Probabilistic Branch");
	        if ((BasicDBList)function.get("probabilisticBranch")!=null && ((BasicDBList)function.get("probabilisticBranch")).size()>0)
	        	cbProbBranch.setValue(true);
	        hlProbBranch.addComponent(cbProbBranch);
	        subContent.addComponent(hlProbBranch);
        }
        
        //ITEMS PROBABILISTIC BRANCH
        final List<TextField> ltfProbBranchItem = new ArrayList<TextField>();
        final List<HorizontalLayout> hlProbBranchItems = new ArrayList<HorizontalLayout>();
        if (dbProbBranch!=null)
        {
	        BasicDBList probBranchList = (BasicDBList) dbProbBranch.get("nameList");
	    	@SuppressWarnings({ "unchecked", "rawtypes" })
			ArrayList<BasicDBObject> probBranchArray = (ArrayList) probBranchList;
	    	
	        for (int i=0 ; i < ((BasicDBList)function.get("followers")).size() ; i++)
	        {
	        	HorizontalLayout hlProbBranchItem = new HorizontalLayout();
	        	hlProbBranchItem.setSpacing(true);
	        	
	        	//LABEL
	        	Label lProbBranchItem = null;
	        	if (probBranchArray!=null && probBranchArray.size()>0)
		        	for (BasicDBObject probBranch :probBranchArray){
			        	if (((BasicDBList)function.get("followers")).get(i).toString().equalsIgnoreCase(probBranch.get("id").toString()))
			        		lProbBranchItem = new Label(probBranch.get("name").toString());
		        	}
	        	else
	        		lProbBranchItem = new Label("");
	        	lProbBranchItem.setWidth("98");
	        	
	        	//TEXTINPUT
	        	TextField tfProbBranchItem = new TextField();
	        	tfProbBranchItem.setWidth("310");
	        	if ((BasicDBList)function.get("probabilisticBranch")!=null && ((BasicDBList)function.get("probabilisticBranch")).size()>0)
	        		tfProbBranchItem.setValue(((BasicDBList)function.get("probabilisticBranch")).get(i).toString());
	
	        	ltfProbBranchItem.add(tfProbBranchItem);
	        	hlProbBranchItem.addComponent(lProbBranchItem);
	        	hlProbBranchItem.addComponent(tfProbBranchItem);
	        	hlProbBranchItems.add(hlProbBranchItem);
	        	subContent.addComponent(hlProbBranchItem);
	        }
        }
        
        //CALCULO ESPACIO
        if (isFull)
        {
        	/*if(((BasicDBList)function.get("followers")).size()>1)
	        {
	        	if(((Integer)(hlSpaceHeight - hlSpaceHeightItem - ((BasicDBList)function.get("followers")).size() * hlSpaceHeightItem))>0)
	        		hlSpace.setHeight( ((Integer)(hlSpaceHeight - hlSpaceHeightItem - ((BasicDBList)function.get("followers")).size() * hlSpaceHeightItem)).toString()+"px");
	        	else
	        		hlSpace.setHeight("0px");
	        }
	        else
	        {
	        	hlSpace.setHeight(((Integer)hlSpaceHeight)+"px");
	        }*/
	        subContent.addComponent(hlSpace);
        	
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
					
					int spaceCheck = 0;
					int spaceFollowers = 0;
					
					if(((BasicDBList)function.get("followers")).size()>1) {
						spaceCheck = 1;
						if (cbProbBranch.getValue())
							spaceFollowers = ((BasicDBList)function.get("followers")).size();
						else
							spaceFollowers = 0;
					}
					
			        if( ( hlSpaceHeight - (hlSpaceHeightItem * (spaceCheck + spaceFollowers) ) ) > 0)
		        		setSpaceHeight( hlSpaceHeight - (hlSpaceHeightItem * (spaceCheck + spaceFollowers) ) );
			        else
			        	setSpaceHeight(0);
	            }
	        });
	        
	        if (cbProbBranch.getValue()) cbProbBranch.setValue(false);
	        else						 cbProbBranch.setValue(true);
	        
	        if (cbProbBranch.getValue()) cbProbBranch.setValue(false);
	        else						 cbProbBranch.setValue(true);
        }
        
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
	            	String[] arrayParams = new String[5];
	            	arrayParams[0] = tfInput.getValue().toString();
	            	arrayParams[1] = tfOutput.getValue();
	            	arrayParams[2] = "no"; //"yes" //prob bhanch
	            	arrayParams[3] = tfName.getValue();	            	
	            	arrayParams[4] = ""; 
	            	
	            	if (cbProbBranch.getValue() && ltfProbBranchItem.size()>0)
	            	{
		            	String ProbBranch = "";
		            	for(int i=0 ;i < ltfProbBranchItem.size() ;i++)
		            	{
		            		ProbBranch += ((BasicDBList)function.get("followers")).get(i).toString();
		            		ProbBranch += ",";
		            		ProbBranch += ltfProbBranchItem.get(i).getValue();
		            		if (i<ltfProbBranchItem.size()-1)
		            			ProbBranch += ",";
		            	}
		            	arrayParams[2] = "yes";
		            	arrayParams[4] = ProbBranch;
	            	}
	        	    
	        	    applet.executeCommand("editNode", arrayParams);
	        	    applet.setStyleName("v-AppletVisible");
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
	        	    arrayParams[0] = "4";
	        	    arrayParams[1] = tfName.getValue();		
	        	    applet.executeCommand("createNode", arrayParams);
	        	    applet.setStyleName("v-AppletVisible");
	        	    close();
				}
			});
        }
        
        Button bCancelar = null;
        if (!project.getState().equalsIgnoreCase("C"))
        	bCancelar = new Button("Cerrar");
        else
        	bCancelar = new Button("Cancelar");	
        bCancelar.addClickListener(new ClickListener() {
			private static final long serialVersionUID = 1L;
			@Override
			public void buttonClick(ClickEvent event) {
				applet.setStyleName("v-AppletVisible");
        	    close();
			}
		});
        
        bAceptar.setWidth("100");
        bCancelar.setWidth("100");
        if (project.getState().equalsIgnoreCase("C"))
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
