package com.example.botqueueweb;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Random;
import java.util.Set;

import com.example.botqueueweb.js.Flot;
import com.example.botqueueweb.js.Gauge;
import com.example.botqueueweb.js.GaugeState;
import com.vaadin.data.Container.Filter;
import com.vaadin.data.Item;
import com.vaadin.data.Property;
import com.vaadin.data.Property.ValueChangeEvent;
import com.vaadin.data.Property.ValueChangeListener;
//import com.vaadin.demo.dashboard.data.DataProvider;
//import com.vaadin.demo.dashboard.data.TransactionsContainer;
import com.vaadin.event.Action;
import com.vaadin.event.Action.Handler;
import com.vaadin.event.FieldEvents.TextChangeEvent;
import com.vaadin.event.FieldEvents.TextChangeListener;
import com.vaadin.event.ShortcutAction.KeyCode;
import com.vaadin.event.ShortcutListener;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Button.ClickListener;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.Notification;
import com.vaadin.ui.Table;
import com.vaadin.ui.Table.Align;
import com.vaadin.ui.Table.ColumnGenerator;
import com.vaadin.ui.Table.TableDragMode;
import com.vaadin.ui.TextField;
import com.vaadin.ui.UI;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.Window;

public class Transformacion extends VerticalLayout implements View {

    private static final long serialVersionUID = 1L;

    Table t;

    Object editableId = null;

    QueueContainer data;

    @Override
    public void enter(ViewChangeEvent event) {
    	
    	data = new QueueContainer();
    	data.addQueue(Calendar.getInstance(), "Cola 1", 1, 1.1); // TODO: hacer model queue y actualizar queuecontainer, correlacion 1 a 1 y quiza algun dato mas en container
    	data.addQueue(Calendar.getInstance(), "Cola 2", 2, 1.2);
    	
        //data = ((BotqueuewebUI) getUI()).dataProvider.getTransactions();

        setSizeFull();
        addStyleName("transactions");

        t = new Table() {
            @Override
            protected String formatPropertyValue(Object rowId, Object colId,
                    Property<?> property) {
                if (colId.equals("Time")) {
                    SimpleDateFormat df = new SimpleDateFormat();
                    df.applyPattern("MM/dd/yyyy hh:mm:ss a");
                    return df
                            .format(((Calendar) property.getValue()).getTime());
                } else if (colId.equals("Price")) {
                    if (property != null && property.getValue() != null) {
                        String ret = new DecimalFormat("#.##").format(property
                                .getValue());
                        return "$" + ret;
                    } else {
                        return "";
                    }
                }
                return super.formatPropertyValue(rowId, colId, property);
            }
        };
        t.setSizeFull();
        t.addStyleName("borderless");
        t.setSelectable(true);
        t.setColumnCollapsingAllowed(true);
        t.setColumnReorderingAllowed(true);
        data.removeAllContainerFilters();
        t.setContainerDataSource(data);
        sortTable();

        t.setColumnAlignment("Seats", Align.RIGHT);
        t.setColumnAlignment("Price", Align.RIGHT);

        t.setVisibleColumns(new Object[] { "Time", "Nombre","Recursos Iniciales","Promedio" });

        t.setFooterVisible(true);
        t.setColumnFooter("Time", "Total");
        updatePriceFooter();

        // Allow dragging items to the reports menu
        t.setDragMode(TableDragMode.MULTIROW);
        t.setMultiSelect(true);

        HorizontalLayout toolbar = new HorizontalLayout();
        toolbar.setWidth("100%");
        toolbar.setSpacing(true);
        toolbar.setMargin(true);
        toolbar.addStyleName("toolbar");
        addComponent(toolbar);

        Label title = new Label("All Transactions");
        title.addStyleName("h1");
        title.setSizeUndefined();
        toolbar.addComponent(title);
        toolbar.setComponentAlignment(title, Alignment.MIDDLE_LEFT);

        final TextField filter = new TextField();
        filter.addTextChangeListener(new TextChangeListener() {
            @Override
            public void textChange(final TextChangeEvent event) {
                data.removeAllContainerFilters();
                data.addContainerFilter(new Filter() {
                    @Override
                    public boolean passesFilter(Object itemId, Item item)
                            throws UnsupportedOperationException {

                        if (event.getText() == null
                                || event.getText().equals("")) {
                            return true;
                        }

                        return filterByProperty("Nombre", item,
                                event.getText())
                                || filterByProperty("Nombre", item,
                                        event.getText())
                                || filterByProperty("Nombre", item,
                                        event.getText());

                    }

                    @Override
                    public boolean appliesToProperty(Object propertyId) {
                        if (propertyId.equals("Nombre")
                                || propertyId.equals("Nombre")
                                || propertyId.equals("Nombre"))
                            return true;
                        return false;
                    }
                });
            }
        });

        filter.setInputPrompt("Filter");
        filter.addShortcutListener(new ShortcutListener("Clear",
                KeyCode.ESCAPE, null) {
            @Override
            public void handleAction(Object sender, Object target) {
                filter.setValue("");
                data.removeAllContainerFilters();
            }
        });
        toolbar.addComponent(filter);
        toolbar.setExpandRatio(filter, 1);
        toolbar.setComponentAlignment(filter, Alignment.MIDDLE_LEFT);

        final Button newReport = new Button("Create Report From Selection");
        newReport.addClickListener(new ClickListener() {
            @Override
            public void buttonClick(ClickEvent event) {
                createNewReportFromSelection();
            }
        });
        newReport.setEnabled(false);
        newReport.addStyleName("small");
        toolbar.addComponent(newReport);
        toolbar.setComponentAlignment(newReport, Alignment.MIDDLE_LEFT);

        addComponent(t);
        setExpandRatio(t, 1);

        t.addValueChangeListener(new ValueChangeListener() {
            @Override
            public void valueChange(ValueChangeEvent event) {
                if (t.getValue() instanceof Set) {
                    Set<Object> val = (Set<Object>) t.getValue();
                    newReport.setEnabled(val.size() > 0);
                } else {
                }
            }
        });
        t.setImmediate(true);
        
        
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

    private void sortTable() {
        t.sort(new Object[] { "timestamp" }, new boolean[] { false });
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

    void updatePriceFooter() {
        /*String ret = new DecimalFormat("#.##").format(DataProvider
                .getTotalSum());*/
        //t.setColumnFooter("Price", "$" + ret);

    }

}
