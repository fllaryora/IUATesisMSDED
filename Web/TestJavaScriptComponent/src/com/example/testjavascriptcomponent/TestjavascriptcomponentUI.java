package com.example.testjavascriptcomponent;

import java.util.Random;

import javax.servlet.annotation.WebServlet;

import com.vaadin.annotations.Theme;
import com.vaadin.annotations.VaadinServletConfiguration;
import com.vaadin.server.VaadinRequest;
import com.vaadin.server.VaadinServlet;
import com.vaadin.ui.Button;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.TextField;
import com.vaadin.ui.UI;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;

@SuppressWarnings("serial")
@Theme("testjavascriptcomponent")
public class TestjavascriptcomponentUI extends UI {

	@WebServlet(value = "/*", asyncSupported = true)
	@VaadinServletConfiguration(productionMode = false, ui = TestjavascriptcomponentUI.class, widgetset = "com.example.testjavascriptcomponent.widgetset.TestjavascriptcomponentWidgetset")
	public static class Servlet extends VaadinServlet {
	}

	@Override
	protected void init(VaadinRequest request) {
		
		VerticalLayout mainLayout = new VerticalLayout();
        mainLayout.setSpacing(true);
        mainLayout.setMargin(true);
        setContent(mainLayout);

        /*Bar Chart*/
        
        final Chart chart = new Chart("Chart1");
        
        String labels[]= {"Cola 1","Cola 2","Cola 3","Cola 4","Cola 5","Combi 1","Combi 2"};
        double points[]= { 0.6, 1.8, 2.8, 7.2, 10, 10.5, 8};
        double points2[]= { 0.6, 10.2, 2.8, 7.2, 5, 1.5, 4};
        
        chart.setType("Bar");
        
        chart.setLabels(labels);
        
        chart.addFillColor("rgba(220,220,220,0.5)");
        chart.addStrokeColor("rgba(220,220,220,1)");
        chart.addSeries(points);
        
        chart.addFillColor("rgba(151,187,205,0.5)");
        chart.addStrokeColor("rgba(151,187,205,1)");
        chart.addSeries(points2);
        
        mainLayout.addComponent(chart);
        
        /* Pie Chart*/
        
        final Chart chart2 = new Chart("Chart2");
        
        chart2.setType("Pie");
        
        chart2.addColor("#F38630");
        chart2.addPercent(25.0);
        
        chart2.addColor("#E0E4CC");
        chart2.addPercent(25.0);
        
        chart2.addColor("#69D2E7");
        chart2.addPercent(50.0);
        
        mainLayout.addComponent(chart2);
        
        final Chart chart3 = new Chart("Chart3");
        
        chart3.setType("Line");

        mainLayout.addComponent(chart3);
        
        //chart.setValue(2);
        
        /*Button b = new Button("Randomize!", new Button.ClickListener() {

            @Override
            public void buttonClick(ClickEvent event) {
                Random ran = new Random();
                int rand1 = ran.nextInt(100);
                chart.setValue(rand1);
            }
        });
        mainLayout.addComponent(b);*/
        /*HorizontalLayout gaugeLayout = new HorizontalLayout();
        final Gauge gauge = new Gauge("gauge");
        gaugeLayout.addComponent(gauge);

        final Gauge gauge2 = new Gauge("gauge2");
        gaugeLayout.addComponent(gauge2);

        mainLayout.addComponent(gaugeLayout);

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
        mainLayout.addComponent(b);*/
        
		
	}

}