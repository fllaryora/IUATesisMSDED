package com.example.botqueueweb.js;

import java.util.ArrayList;
import java.util.List;

import com.vaadin.shared.ui.JavaScriptComponentState;

public class ChartState extends JavaScriptComponentState
{
	private static final long serialVersionUID = 1L;
	
	public int value = 0;
    public String domId;
    
    public String type;

    public List<String> labels = new ArrayList<String>();

    public List<String> fillColors = new ArrayList<String>();
    public List<String> strokeColors = new ArrayList<String>();
    public List<String> pointColors = new ArrayList<String>();
    
    public List<List<Double>> series = new ArrayList<List<Double>>();
    public List<List<Double>> points = new ArrayList<List<Double>>();
    
    public List<Double> percents = new ArrayList<Double>();
    public List<String> colors = new ArrayList<String>();
    
    public String pointStrokeColor;
    public String widthJS;
    public String heightJS; 
    
    //Bar Chart
	//Pie Chart
    //Polar Area Chart
    //Radar Chart
    //Doughnut Chart
    //Line Chart
}