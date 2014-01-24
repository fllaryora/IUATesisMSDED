package ar.com.cron.Bootstrap;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import ar.com.cron.Bootstrap.constants.ProjectsValues;

public class DetectProgram {

	public static void main(String[] args) {
		
		System.out.println(isBotqueueRunning() ?"esta":"no esta");

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
				 System.out.println("---->" +fileEntry.getName() +" datos:"+ fileData.toString() );
				 
				 if(fileData.toString().contains(ProjectsValues.BOTQUEUE_PROGRAM))
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
