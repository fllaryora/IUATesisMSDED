package com.example.botqueueweb.business;

import java.util.ArrayList;
import java.util.List;

import org.bson.types.ObjectId;
import com.example.botqueueweb.dao.MongoConnection;
import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.User;

public class ProjectBusiness {

	private static ProjectBusiness projectBussines = null;
	 
    private ProjectBusiness(){}
 
    private synchronized static void createInstance() {
        if (projectBussines == null) { 
        	projectBussines = new ProjectBusiness();
        }
    }
 
    public static ProjectBusiness getInstance() {
        createInstance();
        return projectBussines;
    }
    
    /** 
     * Busca todos los proyectos de la coleccion 
     * @return lista de proyectos
     */
	public List<Project> getProjects()
	{
		List<Project> projects = new ArrayList<Project>();
		try {
			for (Project project : MongoConnection.getInstance().getDatastore().find(Project.class).retrievedFields(true, "_id","name","state","nroProcs","simTime","deltaT")) {
				projects.add(project);
			}
			return projects;
			
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		
		return null;
	}
	
	 /** 
     * Busca todos los proyectos del usuario en la coleccion 
     * @return lista de proyectos
     */
	public List<Project> getProjectsByUser(User user)
	{
		List<Project> projects = new ArrayList<Project>();
		try {
			for (Project project : MongoConnection.getInstance().getDatastore().find(Project.class).retrievedFields(true, "_id","name","state","nroProcs","simTime","deltaT").filter("user", user)) {
				projects.add(project);
			}
			return projects;
			
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		
		return null;
	}
	
	/**
	 * Busca a un proyecto por Id
	 * @param idProject
	 * @return la primer coincidencia del proyecto encontrado
	 */
	public Project getProject(ObjectId idProject)
	{
		try {
			for (Project project : MongoConnection.getInstance().getDatastore().find(Project.class).filter("_id", idProject)) {
				return project;
			}
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		
		return null;
		
	}
	
	/**
	 * Inserta un proyecto en la coleccion
	 * @param project
	 */
	public void insertProject(Project project)
	{
		saveProject(project);
	}
	
	/**
	 * Actualiza un proyecto en la coleccion
	 * @param project
	 */
	public void saveProject(Project project)
	{
		try {
			MongoConnection.getInstance().getDatastore().save(project);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	/**
	 * Elimina un proyecto en la coleccion
	 * @param project
	 */
	public void deleteProject(ObjectId idProject)
	{
		try {
			Project project = new Project();
			project.setId(idProject);
			MongoConnection.getInstance().getDatastore().delete(project);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
}
