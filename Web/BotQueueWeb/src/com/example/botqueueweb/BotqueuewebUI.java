package com.example.botqueueweb;
	/** 
	 * Representa un marco del nabegador, se cea uno por cada request
	 */
import javax.servlet.annotation.WebServlet;
import com.example.botqueueweb.enums.WebConstantMessages;
import org.bson.types.ObjectId;
import com.example.botqueueweb.business.ProjectBussines;
import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.windows.UsuarioWindow;
import com.vaadin.annotations.Theme;
import com.vaadin.annotations.VaadinServletConfiguration;
import com.vaadin.server.Page;
import com.vaadin.server.VaadinRequest;
import com.vaadin.server.VaadinServlet;
import com.vaadin.ui.Button;
import com.vaadin.ui.Button.ClickEvent;
import com.vaadin.ui.Label;
import com.vaadin.ui.UI;
import com.vaadin.ui.VerticalLayout;
import java.util.HashMap;
import java.util.Iterator;
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
    boolean autoCreateReport = false;
    Table transactions;
    
	@WebServlet(value = "/*", asyncSupported = true)
	@VaadinServletConfiguration(productionMode = false, ui = BotqueuewebUI.class, widgetset = "com.example.botqueueweb.widgetset.BotqueuewebWidgetset")
	public static class Servlet extends VaadinServlet {
	}
	
	@Override
	protected void init(VaadinRequest request) {
        setLocale(request.getLocale());
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

        Label welcome = new Label(WebConstantMessages.ACCES);
        welcome.setSizeUndefined();
        welcome.addStyleName("h4");
        labels.addComponent(welcome);
        labels.setComponentAlignment(welcome, Alignment.MIDDLE_LEFT);

        Label title = new Label(WebConstantMessages.BOTQUEUE);
        title.setSizeUndefined();
        title.addStyleName("h2");
        title.addStyleName("light");
        labels.addComponent(title);
        labels.setComponentAlignment(title, Alignment.MIDDLE_RIGHT);

        HorizontalLayout fields = new HorizontalLayout();
        fields.setSpacing(true);
        fields.setMargin(true);
        fields.addStyleName("fields");

        final TextField username = new TextField(WebConstantMessages.USER);
        username.focus();
        fields.addComponent(username);

        final PasswordField password = new PasswordField(WebConstantMessages.PASS);
        fields.addComponent(password);

        final Button signin = new Button(WebConstantMessages.SIG_IN);
        signin.addStyleName("default");
        fields.addComponent(signin);
        fields.setComponentAlignment(signin, Alignment.BOTTOM_LEFT);

        final ShortcutListener enter = new ShortcutListener(WebConstantMessages.SIG_IN,
                KeyCode.ENTER, null) {
            @Override
            public void handleAction(Object sender, Object target) {
                signin.click();
            }
        };

        signin.addClickListener(new ClickListener() {
            @Override
            public void buttonClick(ClickEvent event) {
            	
            	//TODO: Buscar en BD usuario (para test implementar con una funncion que tenga un arrau de 3 us con pass)
            	//TODO: Buscar en BD usuario 
            	
                if (username.getValue() != null
                        && username.getValue().equals("")
                        && password.getValue() != null
                        && password.getValue().equals("")) {
                    signin.removeShortcutListener(enter);
                    buildMainView();
                } else {
                    if (loginPanel.getComponentCount() > 2) {
                        // Remove the previous error message
                        loginPanel.removeComponent(loginPanel.getComponent(2));
                    }
                    // Add new error message
                    Label error = new Label(
                    		WebConstantMessages.BAD_LOGGIN,
                            ContentMode.HTML);
                    error.addStyleName("error");
                    error.setSizeUndefined();
                    error.addStyleName("light");
                    // Add animation
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

    private void buildMainView() {

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
                                		WebConstantMessages.LOGO_HTML,
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
                        //TODO Buscar pic usuer
                        Image profilePic = new Image(null, new ThemeResource("img/profile-pic.png"));
                        profilePic.setWidth("34px");
                        vmUserMenu.addComponent(profilePic);
                        //TODO Buscar full name
                        Label userName = new Label("Cristian Zerpa");
                        userName.setSizeUndefined();
                        vmUserMenu.addComponent(userName);
                        Button settings = new NativeButton("Edit");
                        settings.setStyleName("icon-cog");
                        vmUserMenu.addComponent(settings);
                        settings.addClickListener(new ClickListener() {
                            @Override
                            public void buttonClick(ClickEvent event) {
                            	Window subWindow = new UsuarioWindow();
                                addWindow(subWindow);
                            }
                        });
                        
                        Button exit = new NativeButton("Exit");
                        exit.addStyleName("icon-cancel");
                        exit.setDescription(WebConstantMessages.EXIT);
                        vmUserMenu.addComponent(exit);
                        exit.addClickListener(new ClickListener() {
                            @Override
                            public void buttonClick(ClickEvent event) {
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
        for (final String view : modulos){
            Button b = new NativeButton();
            b.setCaption(view);
            b.addStyleName("icon-" + view);
            b.addClickListener(new ClickListener() {
                @Override
                public void buttonClick(ClickEvent event) {
                    
                    //setData(idProject);
                    if (!view.equalsIgnoreCase("Home") && getData()==null)
                    {
                    	//showNotification("Seleccione un proyecto");
                    	//Notification.show("Seleccione un proyecto", Notification.Type.WARNING_MESSAGE);
                    	Notification.show("Seleccione un Proyecto", Notification.Type.WARNING_MESSAGE);
                    	return;
                    }
                    else if (view.equalsIgnoreCase("Reporte") && getData()!=null)
                    {
                    	ObjectId idProject = (ObjectId) getData();
                    	ProjectBussines projectBussines = new ProjectBussines();
                    	Project project = projectBussines.getProject(idProject);
                    	if (project.getState().equalsIgnoreCase("F"))//todo: Pasar a PENDIENTE
                    	{
                    		Notification.show("El Proyecto seleccionado se encuentra Pendiente", Notification.Type.WARNING_MESSAGE);
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
        
        String f = Page.getCurrent().getUriFragment();
        if (f != null && f.startsWith("!")) {
            f = f.substring(1);
        }
        if (f == null || f.equals("") || f.equals("/")) {
            nav.navigateTo("/Home");
            menu.getComponent(0).addStyleName("selected");
        }
    }
    
    /** 
     * No se que hace esta funcion :(
     */
    private void clearMenuSelection() {
        for (@SuppressWarnings("deprecation")
		Iterator<Component> it = menu.getComponentIterator(); it.hasNext();) {
            Component next = it.next();
            if (next instanceof NativeButton) {
                next.removeStyleName("selected");
            } else if (next instanceof DragAndDropWrapper) {
                // Wow, this is ugly (even uglier than the rest of the code)
                ((DragAndDropWrapper) next).iterator().next()
                        .removeStyleName("selected");
            }
        }
    }
}