package com.example.botqueueweb.facade;

import java.util.List;

import org.bson.types.ObjectId;

import com.example.botqueueweb.business.ProjectBusiness;
import com.example.botqueueweb.business.UserBusiness;
import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.User;

public class Facade {
	private static Facade facade = null;
	 
    private Facade(){}
 
    private synchronized static void createInstance() {
        if (facade == null) { 
        	facade = new Facade();
        }
    }
 
    public static Facade getInstance() {
        createInstance();
        return facade;
    }
    
    public List<Project> getProjects()
	{
		try {
			return ProjectBusiness.getInstance().getProjects();
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		return null;
	}
	
	
	public Project getProject(ObjectId idProject)
	{
		try {
			return ProjectBusiness.getInstance().getProject(idProject);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		return null;
	}
	
	public void insertProject(Project project)
	{
		try {
			ProjectBusiness.getInstance().insertProject(project);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	public void saveProject(Project project)
	{
		try {
			ProjectBusiness.getInstance().saveProject(project);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	public void deleteProject(ObjectId idProject)
	{
		try {
			ProjectBusiness.getInstance().deleteProject(idProject);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	public User getUserByName(User user)
	{
		try {
			return UserBusiness.getInstance().getUserByName(user);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		return null;
	}
}
