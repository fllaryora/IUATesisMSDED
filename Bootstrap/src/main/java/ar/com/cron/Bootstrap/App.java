package ar.com.cron.Bootstrap;

import java.io.IOException;
import java.net.UnknownHostException;

import ar.com.cron.Bootstrap.constants.ProjectsFiels;
import ar.com.cron.Bootstrap.constants.ProjectsValues;

import com.mongodb.DBObject;
import com.mongodb.MongoException;
import com.mongodb.util.JSON;

 
/**
 *	Bootstrap of Bootqueue
 */

public class App {


	public static void main(String[] args) {
		System.out.println("...START JOB BOOTSTRAP BOTQUEUE...");
		try {
			MongoHelper myMongo = new MongoHelper();
		 
			if (myMongo.isThereProjectRunning( )) {
				
				if( OSHelper.isBotqueueRunning() ){
					anotherBotQueueIsRunningCase();
					return;
				} else {
					resolvingOldProjectCase(myMongo);	
				}
				
			} else {
				passToExecuteAProjectCase( myMongo );
			}
		
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (MongoException e) {
			e.printStackTrace();
		}
		System.out.println("...END JOB BOOTSTRAP BOTQUEUE...");
	}

	private static void anotherBotQueueIsRunningCase() {
		System.out.println("There is already a running Botqueue, Ya hay un Botqueue ejecutando...");
	}
	
	private static void passToExecuteAProjectCase(MongoHelper myMongo) {
		System.out.println("Was not found a project on execution, No se encontro un proyecto en ejecución...");
		DBObject toRead = myMongo.getPendingProject();
	
		if(toRead != null){
			if(!executeProject( myMongo,  toRead )){
				myMongo.toErrorState( toRead );
				System.out.println("The project was passed to wrong state, Ya se paso a estado erroneo el proyecto..." );
			}
		} else{
			System.out.println("Was not found pending projects, No se encontraron proyectos pendientes...");
		}
	}
	
	private static void resolvingOldProjectCase(MongoHelper myMongo) {
		System.out.println("A project not completed was found, solving; Se encontro un proyecto en ejecucion no finalizado, resolviendo....");
		DBObject toRead = myMongo.getExecutingProject();
		if(toRead != null){
			String output = OSHelper.getOutput();
			if( output == null || output.trim().isEmpty() ){
				myMongo.toErrorState( toRead );
				return;
			}
			DBObject toWrite = null;
			try{
				toWrite = (DBObject)JSON.parse(output);
			} catch(Exception e){
				//archivo tiene texto json mal formado
				myMongo.toErrorState( toRead );
				return;
			}
			myMongo.toFinishedState(toRead,  toWrite);
			myMongo.toErrorState( toRead );
			return;
			
		} else {
			System.out.println("Unknown error, Error desconocido....");
		}
	}
	
	/**
	 * Pone en ejecucion al id que le llega en toRead
	 * como se aprendio en http://docs.oracle.com/cd/E19708-01/821-1319-10/ExecutingPrograms.html
	 * @param myMongo
	 * @param toRead
	 * @return
	 */
	static boolean executeProject(MongoHelper myMongo, DBObject toRead ){
		System.out.println("Se pasa el proyecto a estado en ejecucion...");
		if(!myMongo.toExecuteState( toRead )) return false;
		System.out.println("Se escribe el input en el archivo de entrada...");
		if(! OSHelper.writeFile( toRead )) return false;
		
		try {
			System.out.println("Se arma parametros de entrada para MPI...");
			int nrp = (Integer)toRead.get(ProjectsFiels.NRO_PROCS_FIELD);
			Object dT = toRead.get(ProjectsFiels.DELTA_T);
			String deltaT = ""+(Double)(dT!=null?dT:1.0);
			String nroProces = ""+ (nrp+ProjectsValues.SCHEDULER_PROCS);
			
			Process compilationProcess = new ProcessBuilder(
					ProjectsValues.BOTQUEUE_COMPILE_SCRIPT,
					ProjectsValues.COMPILATION_MODE_PRODUCTION, deltaT ).start();
			System.out.println("waiting, esperando a que termine....." );
			
			if (compilationProcess == null) {
				System.out.println("Can't execute botqueue compile, No se pudo ejecutar el compilador de botqueue...");
				return false;	
			}
			
			synchronized (compilationProcess){
				compilationProcess.wait();
			}
			
			Process mpiProcess = new ProcessBuilder(
					ProjectsValues.BOTQUEUE_PROGRAM,
					ProjectsValues.NUMBER_OF_PROCESS, nroProces ,
					ProjectsValues.BOTQUEUE_CODE ).start();
			System.out.println("waiting, esperando a que termine....." );
			
			if (mpiProcess == null) {
				System.out.println("Can't execute botqueue, No se pudo ejecutar botqueue...");
				return false;	
			}
			
			synchronized (mpiProcess){
				mpiProcess.wait();
			}
			System.out.println("Se finalizo la ejecucion...");
			String output = OSHelper.getOutput();
			
			if( output == null || output.trim().isEmpty() ){
				System.out.println("El archivo esta vacio...");
				return false;
			}
			System.out.println("Se obtubo salida..." );
			DBObject toWrite = (DBObject)JSON.parse(output);
			if(!myMongo.toFinishedState( toRead, toWrite)) {
				System.out.println("No pude finalizar un proyecto en mongo..." );
				return false;
			}
			System.out.println("Se finalizo el proyecto..." );
			if( output.contains(ProjectsValues.ERROR_TOKEN_IN_OUTPUT_FILE) ) {
				System.out.println("Pero se detecto error dentro del archivo de salida..." );
				return false;
			}
			System.out.println("No se detecto error dentro del archivo de salida..." );
			
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {
			e.printStackTrace();
			return false;
		}catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	  
		return true;
	}
	
}
