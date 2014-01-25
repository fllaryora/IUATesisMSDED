package com.example.botqueueweb;

import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.ui.Label;
import com.vaadin.ui.VerticalLayout;

public class Transformacion extends VerticalLayout implements View {

    private static final long serialVersionUID = 1L;

    Object editableId = null;

    @Override
    public void enter(ViewChangeEvent event) {
        setSizeFull();
        addStyleName("transactions");
        addComponent(new Label("En Construccion"));
    }

}
