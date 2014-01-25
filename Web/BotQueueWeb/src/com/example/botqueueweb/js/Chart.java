package com.example.botqueueweb.js;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.vaadin.annotations.JavaScript;
import com.vaadin.ui.AbstractJavaScriptComponent;

@JavaScript({ "Chart.js", "chart_connector.js" })
public class Chart extends AbstractJavaScriptComponent {
	
    public Chart(String domId) {
        getState().domId = domId;
    }
    public void setValue(int value) {
        getState().value = value;
    }
    
    public void setType(String type) {
        getState().type = type;
    }
    
    public void addSeries(double... points) {
        List<Double> pointList = new ArrayList<Double>();
        for (int i = 0; i < points.length; i++)
            pointList.add(points[i]);
        getState().series.add(pointList);
    }
    
    public void setLabels(String... labels) {
        List<String> pointList = new ArrayList<String>();
        for (int i = 0; i < labels.length; i++)
            pointList.add(labels[i]);
        getState().labels = pointList;
    }
    
    public void addLabel(String label) {
        getState().labels.add(label);
    }
    
    public void addFillColor(String fillColor) {
        getState().fillColors.add(fillColor);
    }
    
    public void addStrokeColor(String strokeColor) {
        getState().strokeColors.add(strokeColor);
    }
    
    /*PIE*/
    
    public void addPercent(Double percent) {
    	getState().percents.add(percent);
    }
    
    public void addColor(String color) {
        getState().colors.add(color);
    }
    
    @Override
    protected ChartState getState() {
        return (ChartState) super.getState();
    }
    
    /*LINE*/
    
    public void addPointColor(String pointColor) {
        getState().pointColors.add(pointColor);
    }
    
    public void setPointStrokeColor(String pointStrokeColor) {
        getState().pointStrokeColor = pointStrokeColor;
    }
    
    public void setWidthJS(String widthJS) {
        getState().widthJS = widthJS;
    }
	
    public void setHeightJS(String heightJS) {
        getState().heightJS = heightJS;
    }
    
    public void addPoints(double... points) {
        List<Double> pointList = new ArrayList<Double>();
        for (int i = 0; i < points.length; i++)
            pointList.add(points[i]);
        getState().points.add(pointList);
    }
}