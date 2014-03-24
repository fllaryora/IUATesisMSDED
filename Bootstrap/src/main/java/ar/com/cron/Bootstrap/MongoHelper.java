package ar.com.cron.Bootstrap;

import com.mongodb.MongoClient;

import java.net.UnknownHostException;
import java.util.Date;

import ar.com.cron.Bootstrap.constants.BaseConfigurations;
import ar.com.cron.Bootstrap.constants.ProjectsFiels;
import ar.com.cron.Bootstrap.constants.ProjectsValues;

import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBObject;
import com.mongodb.ReadPreference;


public class MongoHelper {
	private MongoClient mongo;
	private DBCollection projects;
	
	public MongoHelper() throws UnknownHostException{
		/**** Connect to MongoDB ****/
		mongo = new MongoClient(BaseConfigurations.MONGO_HOST, BaseConfigurations.MONGO_PORT);
		DB db = mongo.getDB(BaseConfigurations.BOTQUEUEDB);
		projects = db.getCollection(BaseConfigurations.PROJECTS);
	}
	
	/**
	 * ¿Algun proyecto quedo en estado de ejecucion ?
	 * @return
	 */
	public boolean isThereProjectRunning(){
		BasicDBObject documentQuery = new BasicDBObject();
		documentQuery.put(ProjectsFiels.STATE_FIELD, ProjectsValues.EXECUTING_STATE);
		return this.projects.count(documentQuery) > 0;
	}
	
	/**
	 * Trae el proyecto en pending mas viejo
	 * @return
	 */
	public DBObject getPendingProject(){
		BasicDBObject documentQuery = new BasicDBObject();
		documentQuery.put(ProjectsFiels.STATE_FIELD, ProjectsValues.PENDING_STATE);
		BasicDBObject documentFields = new BasicDBObject();
		documentFields.put(ProjectsFiels.INPUT_FIELD, 1);
		documentFields.put(ProjectsFiels.NRO_PROCS_FIELD, 1);
		BasicDBObject documentSort = new BasicDBObject();
		documentSort.put(ProjectsFiels.PENDING_STAMP_FIELD, 1);
		return this.projects.findOne(documentQuery ,documentFields, documentSort, ReadPreference.nearest());
		
	}
	
	/**
	 * Trae el proyecto en ejecucion más viejo
	 * @return
	 */
	public DBObject getExecutingProject(){
		BasicDBObject documentQuery = new BasicDBObject();
		documentQuery.put(ProjectsFiels.STATE_FIELD, ProjectsValues.EXECUTING_STATE);
		BasicDBObject documentFields = new BasicDBObject();
		documentFields.put(ProjectsFiels.NRO_PROCS_FIELD, 1);
		BasicDBObject documentSort = new BasicDBObject();
		documentSort.put(ProjectsFiels.EXECUTING_STAMP_FIELD, 1);
		return this.projects.findOne(documentQuery ,documentFields, documentSort, ReadPreference.nearest());
	}
	
	/**
	 * Pasa un proyecto a estado de error y actualiza la fecha de last updated
	 * @param documents
	 * @param toRead
	 */
	public void toErrorState( DBObject toRead ){
		/**** Update ****/
		BasicDBObject query = new BasicDBObject();
		query.put(ProjectsFiels.OID_FIELD, toRead.get(ProjectsFiels.OID_FIELD));
		BasicDBObject newDocument = new BasicDBObject();
		newDocument.put(ProjectsFiels.STATE_FIELD, ProjectsValues.ERROR_FAIL_STATE);
		Long errorTimeStamp = new Long((new Date()).getTime());
		newDocument.put(ProjectsFiels.LAST_UPDATE_STAMP_FIELD, errorTimeStamp.toString());
		BasicDBObject updateOperatios = new BasicDBObject();
		updateOperatios.put("$set", newDocument);
		this.projects.update(query, updateOperatios);
		
	}
	
	/**
	 * Pasa un proyecto a finalizado,
	 * lo busca (al id) desde  toRead, y le escribe el json toWrite, dentro del campo output del proyecto
	 * @param toRead
	 * @param toWrite
	 * @return
	 */
	public boolean toFinishedState( DBObject toRead,   DBObject toWrite){
		
		System.out.println("Se pasa al estado finalizado....");
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
		int nroFiels = this.projects.update(query, updateOperatios).getN();
		if(nroFiels > 0) return true;
		return false;
	}
	
	/** 
	 * Pasa un proyecto al estado de "en ejecucion" y actualiza las fechas
	 * lo busca por el id de toRead
	 * @param toRead
	 * @return
	 */
	public boolean toExecuteState( DBObject toRead ){
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
		int nroFiels = this.projects.update(query, updateOperatios).getN();
	
		if(nroFiels > 0) return true;
		return false;
	}
}
