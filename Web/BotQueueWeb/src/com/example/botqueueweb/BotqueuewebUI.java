package com.example.botqueueweb;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;

import org.bson.types.ObjectId;

import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.User;
import com.example.botqueueweb.facade.Facade;
import com.example.botqueueweb.windows.UsuarioWindow;
import com.vaadin.annotations.Theme;
import com.vaadin.annotations.VaadinServletConfiguration;
import com.vaadin.server.CustomizedSystemMessages;
import com.vaadin.server.SystemMessages;
import com.vaadin.server.SystemMessagesInfo;
import com.vaadin.server.SystemMessagesProvider;
import com.vaadin.server.VaadinRequest;
import com.vaadin.server.VaadinServlet;
import com.vaadin.ui.Button;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Label;
import com.vaadin.ui.UI;
import com.vaadin.ui.VerticalLayout;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;

import com.vaadin.annotations.Title;
import com.vaadin.event.ShortcutAction.KeyCode;
import com.vaadin.event.ShortcutListener;
import com.vaadin.navigator.Navigator;
import com.vaadin.navigator.View;
import com.vaadin.server.ThemeResource;
import com.vaadin.shared.ui.label.ContentMode;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button.ClickListener;
import com.vaadin.ui.Component;
import com.vaadin.ui.CssLayout;
import com.vaadin.ui.DragAndDropWrapper;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Image;
import com.vaadin.ui.NativeButton;
import com.vaadin.ui.Notification;
import com.vaadin.ui.PasswordField;
import com.vaadin.ui.Table;
import com.vaadin.ui.TextField;
import com.vaadin.ui.Window;

@SuppressWarnings("serial")
@Theme("botqueueweb")
@Title("BotQueue Web")
public class BotqueuewebUI extends UI {

    private static final long serialVersionUID = 1L;

    CssLayout root = new CssLayout();

    VerticalLayout loginLayout;

    CssLayout menu = new CssLayout();
    CssLayout content = new CssLayout();

    //ObjectId idProject;
    
    HashMap<String, Class<? extends View>> routes = new HashMap<String, Class<? extends View>>() {
        {
        	put("/Home", Home.class);//put("/dashboard", DashboardView.class);
        	put("/Transformacion", Transformacion.class);//put("/dashboard", DashboardView.class);
        	put("/Precursor", Precursor.class);//put("/dashboard", DashboardView.class);
        	put("/Reporte", Reporte.class);//put("/dashboard", DashboardView.class);
        	/*put("/sales", SalesView.class);//put("/sales", SalesView.class);
            put("/transactions", TransactionsView.class);
            put("/reports", ReportsView.class);
            put("/schedule", ScheduleView.class);//put("/schedule", ScheduleView.class);*/
        }
    };

    HashMap<String, Button> viewNameToMenuButton = new HashMap<String, Button>();

    private Navigator nav;

    //private HelpManager helpManager;
    
	@WebServlet(value = "/*", asyncSupported = true)
	@VaadinServletConfiguration(productionMode = false, ui = BotqueuewebUI.class, widgetset = "com.example.botqueueweb.widgetset.BotqueuewebWidgetset")
	//@VaadinServletConfiguration(productionMode = false, ui = BotqueuewebUI.class, widgetset = "org.vaadin.applet.widgetset.AppletintegrationWidgetset")
	public static class Servlet extends VaadinServlet {
		
		 @Override
		    protected void servletInitialized()
		            throws ServletException {
		        super.servletInitialized();

		        getService().setSystemMessagesProvider(
		        	new SystemMessagesProvider() {
					   public SystemMessages getSystemMessages(SystemMessagesInfo systemMessagesInfo){
					       CustomizedSystemMessages messages = new CustomizedSystemMessages();
					       messages.setCommunicationErrorCaption("Error");
					       messages.setCommunicationErrorMessage("Se ha producido un fallo de conexión.");
					       messages.setCommunicationErrorNotificationEnabled(true);
					       messages.setCommunicationErrorURL("http://vaadin.com/");
					       return messages;
					   }
					}			   
		        );
		    }
		
	}

	@Override
	protected void init(VaadinRequest request) {
		
        //helpManager = new HelpManager(this);

		setLocale(Locale.US);
		
        setContent(root);
        root.addStyleName("root");
        root.setSizeFull();

        Label bg = new Label();
        bg.setSizeUndefined();
        bg.addStyleName("login-bg");
        root.addComponent(bg);

        buildLoginView(false);
	}
	
	private void buildLoginView(boolean exit) {
        if (exit) {
            root.removeAllComponents();
        }
        
        addStyleName("login");

        loginLayout = new VerticalLayout();
        loginLayout.setSizeFull();
        loginLayout.addStyleName("login-layout");
        root.addComponent(loginLayout);

        final CssLayout loginPanel = new CssLayout();
        loginPanel.addStyleName("login-panel");

        HorizontalLayout labels = new HorizontalLayout();
        labels.setWidth("100%");
        labels.setMargin(true);
        labels.addStyleName("labels");
        loginPanel.addComponent(labels);

        Label welcome = new Label("Acceso");
        welcome.setSizeUndefined();
        welcome.addStyleName("h4");
        labels.addComponent(welcome);
        labels.setComponentAlignment(welcome, Alignment.MIDDLE_LEFT);

        Label title = new Label("BotQueue");
        title.setSizeUndefined();
        title.addStyleName("h2");
        title.addStyleName("light");
        labels.addComponent(title);
        labels.setComponentAlignment(title, Alignment.MIDDLE_RIGHT);

        HorizontalLayout fields = new HorizontalLayout();
        fields.setSpacing(true);
        fields.setMargin(true);
        fields.addStyleName("fields");

        final TextField username = new TextField("Usuario");
        username.focus();
        fields.addComponent(username);

        final PasswordField password = new PasswordField("Contraseña");
        fields.addComponent(password);

        final Button signin = new Button("Entrar");
        signin.addStyleName("default");
        fields.addComponent(signin);
        fields.setComponentAlignment(signin, Alignment.BOTTOM_LEFT);

        final ShortcutListener enter = new ShortcutListener("Entrar",
                KeyCode.ENTER, null) {
            @Override
            public void handleAction(Object sender, Object target) {
                signin.click();
            }
        };

        signin.addClickListener(new ClickListener() {
            @Override
            public void buttonClick(ClickEvent event)
            {
            	if (username.getValue() == null || username.getValue().equals(""))
            	{
            		if (loginPanel.getComponentCount() > 2) {
                        loginPanel.removeComponent(loginPanel.getComponent(2));
                    }
            		Label error = new Label( "Ingrese Usuario.", ContentMode.HTML);
                    error.addStyleName("error");
                    error.setSizeUndefined();
                    error.addStyleName("light");
                    error.addStyleName("v-animate-reveal");
                    loginPanel.addComponent(error);
                    username.focus();
                    return;
            	}
            	
            	if (password.getValue() == null || password.getValue().equals(""))
            	{
            		if (loginPanel.getComponentCount() > 2) {
                        loginPanel.removeComponent(loginPanel.getComponent(2));
                    }
            		Label error = new Label( "Ingrese Contraseña.", ContentMode.HTML);
                    error.addStyleName("error");
                    error.setSizeUndefined();
                    error.addStyleName("light");
                    error.addStyleName("v-animate-reveal");
                    loginPanel.addComponent(error);
                    username.focus();
                    return;
            	}
            	
            	//TODO implementar proyectos propios de usuario
            	User user = new User();
            	user.setUsername(username.getValue());
            	user = Facade.getInstance().getUserByName(user);
            	
                if (user != null && user.getPassword().equals(password.getValue())) {
                	setData(new HashMap<String,Object>());
                	((HashMap<String,Object>)getData()).put("user", user);
                    signin.removeShortcutListener(enter);
                    buildMainView(user);
                } else {
                    if (loginPanel.getComponentCount() > 2) {
                        loginPanel.removeComponent(loginPanel.getComponent(2));
                    }
                    Label error = new Label("Usuario o Contraseña incorrecto.", ContentMode.HTML);
                    error.addStyleName("error");
                    error.setSizeUndefined();
                    error.addStyleName("light");
                    error.addStyleName("v-animate-reveal");
                    loginPanel.addComponent(error);
                    username.focus();
                }
            }
        });

        signin.addShortcutListener(enter);

        loginPanel.addComponent(fields);

        loginLayout.addComponent(loginPanel);
        loginLayout.setComponentAlignment(loginPanel, Alignment.MIDDLE_CENTER);
    }

    private void buildMainView(final User user) {

        nav = new Navigator(this, content);

        for (String route : routes.keySet()) {
            nav.addView(route, routes.get(route));
        }

        //helpManager.closeAll();
        removeStyleName("login");
        root.removeComponent(loginLayout);

        root.addComponent(new HorizontalLayout() {
            {
                setSizeFull();
                addStyleName("main-view");
                addComponent(new VerticalLayout() {
                    // Sidebar
                    {
                        addStyleName("sidebar");
                        setWidth(null);
                        setHeight("100%");

                        // Branding element
                        addComponent(new CssLayout() {
                            {
                                addStyleName("branding");
                                Label logo = new Label(
                                        "<span>Tesis</span> BotQueue",
                                        ContentMode.HTML);
                                logo.setSizeUndefined();
                                addComponent(logo);
                            }
                            
                        });

                        // Main menu
                        addComponent(menu);
                        setExpandRatio(menu, 1);

                        //USER MENU
                        VerticalLayout vmUserMenu = new VerticalLayout();
                        vmUserMenu.setSizeUndefined();
                        vmUserMenu.addStyleName("user");
                        Image profilePic = new Image(null, new ThemeResource("img/profile-pic.png"));
                        profilePic.setWidth("34px");
                        vmUserMenu.addComponent(profilePic);
                        Label userName = new Label(user.getNickname());
                        userName.setSizeUndefined();
                        vmUserMenu.addComponent(userName);
                        
                        Button settings = new NativeButton("Edit");
                        settings.setStyleName("icon-cog");
                        vmUserMenu.addComponent(settings);
                        settings.addClickListener(new ClickListener() {
                            @Override
                            public void buttonClick(ClickEvent event) {
                            	Window subWindow = new UsuarioWindow();
                            	//Notification.show("Hola");
                                addWindow(subWindow);
                            }
                        });
                        
                        Button exit = new NativeButton("Exit");
                        exit.addStyleName("icon-cancel");
                        exit.setDescription("Salir");
                        vmUserMenu.addComponent(exit);
                        exit.addClickListener(new ClickListener() {
                            @Override
                            public void buttonClick(ClickEvent event) {
                            	setData(null);
                                buildLoginView(true);
                            }
                        });
                        
                        addComponent(vmUserMenu);
                        
                    }
                });
                // Content
                addComponent(content);
                content.setSizeFull();
                content.addStyleName("view-content");
                setExpandRatio(content, 1);
            }

        });

        menu.removeAllComponents();

        String[] modulos = new String[] { "Home","Transformacion","Precursor", "Reporte", };
        for (final String view : modulos)
        {
            Button b = new NativeButton();
            b.setCaption(view);
            b.addStyleName("icon-" + view);
            b.addClickListener(new ClickListener() {
                @Override
                public void buttonClick(ClickEvent event) {
                    
                    //setData(idProject);
                    //if (!view.equalsIgnoreCase("Home") && getData()==null)
                    if (!view.equalsIgnoreCase("Home") && ((HashMap<String,Object>)getData()).get("idProjectSelected")==null)
                    {
                    	//showNotification("Seleccione un proyecto");
                    	//Notification.show("Seleccione un proyecto", Notification.Type.WARNING_MESSAGE);
                    	Notification.show("Seleccione un Proyecto", Notification.Type.WARNING_MESSAGE);
                    	return;
                    }
                    //else if (view.equalsIgnoreCase("Reporte") && getData()!=null)
                    else if (view.equalsIgnoreCase("Reporte") && ((HashMap<String,Object>)getData()).get("idProjectSelected")!=null)
                    {
                    	//ObjectId idProject = (ObjectId) getData();
                    	ObjectId idProject = (ObjectId) ((HashMap<String,Object>)getData()).get("idProjectSelected");
                    	Project project = Facade.getInstance().getProject(idProject);
                    	if (project.getState().equalsIgnoreCase("C") ||
                			project.getState().equalsIgnoreCase("P") ||
                			//project.getState().equalsIgnoreCase("E") ||
                			project.getState().equalsIgnoreCase("X") 
                    		)//todo: Pasar a PENDIENTE
                    	{
                    		Notification.show("El Proyecto seleccionado debe estar Finalizado", Notification.Type.WARNING_MESSAGE);
    	                	return;
                    	}
	                	//showNotification("Seleccione un proyecto");
	                	//Notification.show("Seleccione un proyecto", Notification.Type.WARNING_MESSAGE);
	                }

                    clearMenuSelection();
                    event.getButton().addStyleName("selected");
                    
                    if (!nav.getState().equals("/" + view))
                        nav.navigateTo("/" + view);
                }
            });
            
            menu.addComponent(b);

            viewNameToMenuButton.put("/" + view, b);
        }
        menu.addStyleName("menu");
        menu.setHeight("100%");
        
        /*String f = Page.getCurrent().getUriFragment();
        if (f != null && f.startsWith("!")) {
            f = f.substring(1);
        }
        if (f == null || f.equals("") || f.equals("/")) {
            nav.navigateTo("/Home");
            menu.getComponent(0).addStyleName("selected");
        }*/
        
        nav.navigateTo("/Home");
        menu.getComponent(0).addStyleName("selected");
    }
    
    private void clearMenuSelection() {
        for (@SuppressWarnings("deprecation")
		Iterator<Component> it = menu.getComponentIterator(); it.hasNext();) {
            Component next = it.next();
            if (next instanceof NativeButton) {
                next.removeStyleName("selected");
            } else if (next instanceof DragAndDropWrapper) {
                ((DragAndDropWrapper) next).iterator().next()
                        .removeStyleName("selected");
            }
        }
    }
    
    boolean autoCreateReport = false;
    Table transactions;
}