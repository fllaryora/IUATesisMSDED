package ar.com.cron.Bootstrap;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import com.mongodb.DBObject;
import ar.com.cron.Bootstrap.constants.ProjectsFiels;
import ar.com.cron.Bootstrap.constants.ProjectsValues;

public class OSHelper {
	
	/**
	 * Busca en el sistema operativo si
	 *  hay un proceso corriendo con el nombre MPI
	 *  Linux guarda el nombre del proceso en el archivo de texto: /proc/XXXX/comm
	 * @return
	 */
	public static boolean isBotqueueRunning(){
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
				 if(fileData.toString().contains(ProjectsValues.BOTQUEUE_PROGRAM))
					 return true;
			}
		}
		return false;
		
	}
	
	/**
	 * Funcion auxiliar para saber si es un numero
	 * @param value
	 * @return
	 */
	private static boolean isNum(String value){  
		try{  
			Integer.parseInt(value);  
			return true;  
		} catch(NumberFormatException nfe){  
	          return false;  
		}  
	}
	
	/**
	 * Busca el texto que esta en el archivo de salida del botQueue y lo borra tras extraerlo
	 * Si no hay noda dentro o no existe el archivo trae null o vacio
	 */
	public static String getOutput() {
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
	        newTextFile.deleteOnExit();
	        return fileData.toString();
		} else return null;
        
    }
	
	/**
	 * Escribe en el archivo de entra de de botqueue, 
	 * el json toRead
	 * @param toRead
	 * @return
	 */
	public static boolean writeFile(DBObject toRead ){
		try {
			File newTextFile = new File(ProjectsValues.BOTQUEUE_INPUT_FILE);
			if(newTextFile.exists())newTextFile.delete();
			FileWriter fw = new FileWriter(newTextFile);
			fw.write(toRead.get(ProjectsFiels.INPUT_FIELD).toString());
			fw.close();
		} catch (IOException iox) {
			iox.printStackTrace();
			return false;
		}
		return true;
	}
	
}
