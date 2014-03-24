package com.example.botqueueweb.business;

import java.io.BufferedReader;
import java.io.FileReader;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;

import org.bson.types.ObjectId;

import com.example.botqueueweb.dto.Project;
import com.example.botqueueweb.dto.input.Combi;
import com.example.botqueueweb.dto.input.Delay;
import com.example.botqueueweb.dto.input.JsonInput;
import com.example.botqueueweb.dto.input.Queue;
import com.example.botqueueweb.dto.input.Transformation;
import com.google.code.morphia.Datastore;
import com.google.code.morphia.Morphia;
import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
import com.mongodb.Mongo;
import com.mongodb.util.JSON;

public class ProjectBussines {

	public List<Project> getProjects() {
		Mongo mongo;
		List<Project> projects = new ArrayList<Project>();
		
		try {
			//TODO LLamar DAO
			mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			Datastore ds = morphia.createDatastore(mongo,"botqueuedb");
			for (Project project : ds.find(Project.class).retrievedFields(true, "_id","name","state","nroProcs")) {
				projects.add(project);
			}
			return projects;
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		
		return null;
	}
	
	public Project getProject(ObjectId idProject) {
		
		Mongo mongo;
		
		try {
			
			//LECTURA
			
			mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			Datastore ds = morphia.createDatastore(mongo,"botqueuedb");
			for (Project project : ds.find(Project.class).filter("_id", idProject)) {
				//System.out.println(project.getOutput().getSummaryReport().getTotalTime());
				return project;
			}
			
			//GUARDAR
			
			/*mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			morphia.map(Project.class);
			Datastore ds = morphia.createDatastore(mongo,"test");
			
			Project project = new Project();
			JsonInput jsonInput = new JsonInput();
			Transformation transformation = new Transformation();
			Combi combi; Delay delay;
			List<Combi> combis = new ArrayList<Combi>();
			combi = new Combi();
			combi.setIdNode(1);
			delay = new Delay();
			delay.setMinimun(0.9);
			delay.setMaximun(1.9);
			combi.setDelay(delay);
			combis.add(combi);
			combi = new Combi();
			combi.setIdNode(2);
			delay = new Delay();
			delay.setConstant(4.4);
			combi.setDelay(delay);
			combis.add(combi);
			transformation.setCombis(combis);
			jsonInput.setTransformation(transformation);
			jsonInput.setLength(1);
			project.setName("Example Delay");;
			project.setInput(jsonInput);
			ds.save(project);*/
			
			//OTROS
			
			/*mongo = new Mongo("localhost", 27017);
			
			//List<String> dbs = mongo.getDatabaseNames();
			//for(String db1 : dbs)
			//	System.out.println(db1);
		
			DB db = mongo.getDB("test");
			DBCollection table = db.getCollection("docs");
			
			BasicDBObject searchQuery = new BasicDBObject();
			
			BasicDBObject fields = new BasicDBObject();
			fields.put("output", 1);
			fields.put("_id", 0);
			
			//DBCursor cursor = table.find(searchQuery);
			DBCursor cursor = table.find(searchQuery,fields);
			while (cursor.hasNext())
				System.out.println(cursor.next());*/
			
			/*mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			Datastore ds = morphia.createDatastore(mongo,"test");
			ds.find(JsonSalida.class);*/
			
			//for (Queue queue : ds.find(Queue.class,"idNode", 1)) {
			/*for (JsonSalida jsonSalida : ds.find(JsonSalida.class)) {
				System.out.println(jsonSalida.getSummaryReport().getTotalTime());
			}*/
			
			/*Queue queue = new Queue();
			queue.setIdNode(1);
			queue.setAmount(2);
			queue.setCounterInput(3);
			queue.setCounterOutput(4);
			queue.setAverage(1.00);
			queue.setMaximun(5);
			queue.setMinimun(6);
			queue.setTimesNotEmpty(1.11);
			queue.setPercentTimesNotEmpty(1.22);
			ds.save(queue);*/
			
			//LECTURA DE ARCHIVO
			
			/*mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			morphia.map(JsonSalida.class);
			
			FileReader fr = new FileReader("/home/Cristian/Descargas/tupla2.json");
			BufferedReader br = new BufferedReader(fr);
			String linea;
			DBObject dbObject = null;
			while((linea = br.readLine()) != null)
				dbObject = (DBObject) JSON.parse(linea); //TODO: Linea de applet
		    fr.close();
		    
			JsonSalida jsonSalida = morphia.fromDBObject(JsonSalida.class, dbObject); 
			System.out.println(jsonSalida.getSummaryReport().getTotalCost());*/
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		
		return null;
		
	}
	
	public void saveProject(Project project) {
		Mongo mongo;
		
		try {
			//TODO LLamar DAO
			mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			Datastore ds = morphia.createDatastore(mongo,"botqueuedb");
			ds.save(project);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	public Queue getQueue() {
		
		Mongo mongo;
		Queue queue = null;
		
		try {
			
			//LECTURA
			
			mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			morphia.map(Project.class);
			
			FileReader fr = new FileReader("/home/Cristian/Descargas/karina_queue.json");
			BufferedReader br = new BufferedReader(fr);
			String linea;
			DBObject dbObject = null;
			while((linea = br.readLine()) != null)
				dbObject = (DBObject) JSON.parse(linea); //TODO: Linea de applet
		    fr.close();
		    
		    //Project pr = morphia.fromDBObject(Project.class, dbObject);
		    //queue = morphia.fromDBObject(Queue.class, dbObject);
		    
		    //dbObject.get(key);
		    dbObject.get("idNode");
		    dbObject.get("name");
		    dbObject.get("resource");
			//private List<Integer> preceders;
			//private List<Integer> followers;
			dbObject.get("fixedCost");
			dbObject.get("variableCost");
			
			System.out.println(dbObject.toString());
			
			dbObject.put("fixedCost",5.6);
			
			System.out.println(dbObject.toString());
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		
		return queue;
		
	}
	
	public void insertProject(Project project)
	{
		Mongo mongo;
		List<Project> projects = new ArrayList<Project>();
		
		try {
			mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			Datastore ds = morphia.createDatastore(mongo,"botqueuedb");
			ds.save(project);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	public void deleteProject(ObjectId idProject) {
		Mongo mongo;
		List<Project> projects = new ArrayList<Project>();
		
		try {
			mongo = new Mongo("localhost", 27017);
			Morphia morphia = new Morphia();
			Datastore ds = morphia.createDatastore(mongo,"botqueuedb");
			Project project = new Project();
			project.setId(idProject);
			ds.delete(project);
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
}
