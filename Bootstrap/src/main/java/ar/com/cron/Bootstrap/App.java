package ar.com.cron.Bootstrap;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.Date;

import ar.com.cron.Bootstrap.constants.BaseConfigurations;
import ar.com.cron.Bootstrap.constants.ProjectsFiels;
import ar.com.cron.Bootstrap.constants.ProjectsValues;

import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBObject;
import com.mongodb.MongoClient;
import com.mongodb.MongoException;
import com.mongodb.ReadPreference;
import com.mongodb.util.JSON;

 
/**
 *	Bootstrap of Bootqueue
 */

public class App {


public static void main(String[] args) {
 
	try {
		/**** Connect to MongoDB ****/
		MongoClient mongo = new MongoClient(BaseConfigurations.MONGO_HOST, BaseConfigurations.MONGO_PORT);
		DB db = mongo.getDB(BaseConfigurations.BOTQUEUEDB);
		DBCollection documents = db.getCollection(BaseConfigurations.PROJECTS);
	 
		if (isThereProjectRunning(documents)) {
			
			if(isBotqueueRunning()){
				System.out.println("Ya hay un Botqueue ejecutando...");
				return;
			} else {
				System.out.println("Se encontro un proyecto en ejecucion no finalizado, resolviendo....!!");
				
				DBObject toRead = getExecutingProject( documents);
				if(toRead != null){
					String output = getOutput();
					if( output == null || output.trim().isEmpty() ){
						toErrorState( documents,  toRead );
						return;
					}
					DBObject toWrite = (DBObject)JSON.parse(output);
					/**** Update to finished ****/
					toFinishedState( documents,  toRead,  toWrite);
					toErrorState( documents,  toRead );
					return;
					
				} else {
					System.out.println("Error desconocido: Existe un proyecto en ejecucion pero mongo no me lo da.");
				}	
				
			}
			
		} else {
			System.out.println("NO Se encontro un proyecto en ejecucion!!");
			DBObject toRead = getPendingProject(documents);
		
			if(toRead != null){
				if(!executeProject( documents,  toRead ))
					toErrorState( documents,  toRead );
			} else{
				System.out.println("No se encontraron proyectos pendientes");
			}
		}
	
	} catch (UnknownHostException e) {
		e.printStackTrace();
	} catch (MongoException e) {
		e.printStackTrace();
	}
 
	}
	static boolean isThereProjectRunning(DBCollection documents){
		BasicDBObject documentQuery = new BasicDBObject();
		documentQuery.put(ProjectsFiels.STATE_FIELD, ProjectsValues.EXECUTING_STATE);
		return documents.count(documentQuery) > 0;
	}
	
	static DBObject getPendingProject(DBCollection documents){
		
		BasicDBObject documentQuery = new BasicDBObject();
		documentQuery.put(ProjectsFiels.STATE_FIELD, ProjectsValues.PENDING_STATE);
		
		BasicDBObject documentFields = new BasicDBObject();
		documentFields.put(ProjectsFiels.INPUT_FIELD, 1);
		documentFields.put(ProjectsFiels.NRO_PROCS_FIELD, 1);
		
		BasicDBObject documentSort = new BasicDBObject();
		documentSort.put(ProjectsFiels.PENDING_STAMP_FIELD, 1);
		
		return documents.findOne(documentQuery ,documentFields, documentSort, ReadPreference.nearest());
		
	}
	
	static DBObject getExecutingProject(DBCollection documents){
		BasicDBObject documentQuery = new BasicDBObject();
		documentQuery.put(ProjectsFiels.STATE_FIELD, ProjectsValues.EXECUTING_STATE);
		
		BasicDBObject documentFields = new BasicDBObject();
		documentFields.put(ProjectsFiels.NRO_PROCS_FIELD, 1);
		
		BasicDBObject documentSort = new BasicDBObject();
		documentSort.put(ProjectsFiels.EXECUTING_STAMP_FIELD, 1);
		
		return documents.findOne(documentQuery ,documentFields, documentSort, ReadPreference.nearest());
	}
	
	static boolean executeProject(DBCollection documents, DBObject toRead ){
		
		/**** Update ****/		
		if(!toExecuteState( documents,  toRead )) return false;
		if(!writeFile( toRead )) return false;
		
		String runForestRun = "mpirun -np "+ toRead.get(ProjectsFiels.NRO_PROCS_FIELD).toString() +" /home/francisco/Tesis/repo/IUATesisMSDED/Backend/Engine";
		try {
			Process process = new ProcessBuilder(runForestRun).start();
			process.wait();
			String output = getOutput();
			
			if( output == null || output.trim().isEmpty() ){
				return false;
			}
			
			DBObject toWrite = (DBObject)JSON.parse(output);
			/**** Update to finished ****/
			if(!toFinishedState( documents,  toRead,  toWrite)) {
				return false;
			}
			
			if( output.matches(ProjectsValues.ERROR_TOKEN_IN_OUTPUT_FILE) ) {
				return false;
			}
			
			
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	  
		return true;
	}
	
	private static String getOutput() {
		File newTextFile = new File(ProjectsValues.BOTQUEUE_OUTPUT_FILE);
		if(newTextFile.exists()){
			StringBuffer fileData = new StringBuffer();
	        char[] buf = new char[1024];
	        int numRead=0;
	        try {
	        	BufferedReader reader = new BufferedReader(  new FileReader(newTextFile));
				while((numRead=reader.read(buf)) != -1){
				    String readData = String.valueOf(buf, 0, numRead);
				    fileData.append(readData);
				}
				reader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
	       
	        return fileData.toString();
		} else return null;
        
    }
	
	private static boolean toExecuteState(DBCollection documents, DBObject toRead ){
		/**** Update ****/
		BasicDBObject query = new BasicDBObject();
		query.put(ProjectsFiels.OID_FIELD, toRead.get(ProjectsFiels.OID_FIELD));
	 
		BasicDBObject newDocument = new BasicDBObject();
		newDocument.put(ProjectsFiels.STATE_FIELD, ProjectsValues.EXECUTING_STATE);
		Long executingTimeStap = new Long((new Date()).getTime());
		newDocument.put(ProjectsFiels.EXECUTING_STAMP_FIELD, executingTimeStap.toString());
		newDocument.put(ProjectsFiels.LAST_UPDATE_STAMP_FIELD, executingTimeStap.toString());
		BasicDBObject updateOperatios = new BasicDBObject();
		updateOperatios.put("$set", newDocument);
		int nroFiels = documents.update(query, updateOperatios).getN();
		if(nroFiels > 0) return false;
		return true;
	}
	
	private static boolean toFinishedState(DBCollection documents, DBObject toRead,   DBObject toWrite){
		/**** Update ****/
		BasicDBObject query = new BasicDBObject();
		query.put(ProjectsFiels.OID_FIELD, toRead.get(ProjectsFiels.OID_FIELD));
	 
		BasicDBObject newDocument = new BasicDBObject();
		newDocument.put(ProjectsFiels.STATE_FIELD, ProjectsValues.FINISHED_STATE);
		Long finishedTimeStap = new Long((new Date()).getTime());
		newDocument.put(ProjectsFiels.FINISHED_STAMP_FIELD, finishedTimeStap.toString());
		newDocument.put(ProjectsFiels.LAST_UPDATE_STAMP_FIELD, finishedTimeStap.toString());
		newDocument.put(ProjectsFiels.OUTPUT_FIELD, toWrite);
		BasicDBObject updateOperatios = new BasicDBObject();
		updateOperatios.put("$set", newDocument);
		int nroFiels = documents.update(query, updateOperatios).getN();
		if(nroFiels > 0) return false;
		return true;
	}
	
	private static boolean writeFile(DBObject toRead ){
		try {
			File newTextFile = new File(ProjectsValues.BOTQUEUE_INPUT_FILE);
			if(newTextFile.exists())newTextFile.delete();
				
			FileWriter fw = new FileWriter(newTextFile);
			fw.write(toRead.get(ProjectsFiels.INPUT_FIELD).toString());
			fw.close();
		
		} catch (IOException iox) {
			//do stuff with exception
			iox.printStackTrace();
			return false;
		}
		return true;
	}
	
	private static void toErrorState(DBCollection documents, DBObject toRead ){
		/**** Update ****/
		BasicDBObject query = new BasicDBObject();
		query.put(ProjectsFiels.OID_FIELD, toRead.get(ProjectsFiels.OID_FIELD));
	 
		BasicDBObject newDocument = new BasicDBObject();
		newDocument.put(ProjectsFiels.STATE_FIELD, ProjectsValues.ERROR_FAIL_STATE);
		Long errorTimeStamp = new Long((new Date()).getTime());
		newDocument.put(ProjectsFiels.LAST_UPDATE_STAMP_FIELD, errorTimeStamp.toString());
		BasicDBObject updateOperatios = new BasicDBObject();
		updateOperatios.put("$set", newDocument);
		documents.update(query, updateOperatios);
		
	}
	
	private static boolean isBotqueueRunning(){
		final File folder = new File("/proc");
		for (final File fileEntry : folder.listFiles()) {
			if (fileEntry.isDirectory() && isNum(fileEntry.getName())) {
				 File procName = new File("/proc/" + fileEntry.getName() + "/comm");
				 StringBuffer fileData = new StringBuffer();
				 char[] buf = new char[1024];
				 int numRead=0;
				 try {
					 BufferedReader reader = new BufferedReader(  new FileReader(procName));
					 while((numRead=reader.read(buf)) != -1){
						 String readData = String.valueOf(buf, 0, numRead);
						 fileData.append(readData);
					 }
					 reader.close();
				 } catch (IOException e) {
					 e.printStackTrace();
				 }
			       
				 if(fileData.toString().equals(ProjectsValues.BOTQUEUE_PROGRAM))
					 return true;
			}
		}
		return false;
		
	}
	
	private static boolean isNum(String value){  
		try{  
			Integer.parseInt(value);  
			return true;  
		} catch(NumberFormatException nfe){  
	          return false;  
		}  
	}
	
}


