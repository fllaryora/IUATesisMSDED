package com.example.botqueueweb;

import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

import com.vaadin.data.Item;
import com.vaadin.data.util.IndexedContainer;

public class QueueContainer extends IndexedContainer {

    private static final long serialVersionUID = 1L;

    public QueueContainer() {
        addContainerProperty("timestamp", Date.class, new Date());
        addContainerProperty("Time", Calendar.class, new GregorianCalendar());
        addContainerProperty("Nombre", String.class, "");
        addContainerProperty("Recursos Iniciales", Integer.class, 0);
        addContainerProperty("Promedio", Double.class, 0);
    }

    public void addQueue(Calendar time, String nombre, int resourceInit, double promedio) {
        Object id = addItem();
        Item item = getItem(id);
        if (item != null) {
            item.getItemProperty("timestamp").setValue(time.getTime());
            item.getItemProperty("Time").setValue(time);
            item.getItemProperty("Nombre").setValue(nombre);
            item.getItemProperty("Recursos Iniciales").setValue(resourceInit);
            item.getItemProperty("Promedio").setValue(promedio);
        }
    }

}