package ar.com.cron.Bootstrap.constants;

public class ProjectsValues {
	public static final String  CONSTRUCTION_STATE = "C";
	public static final String PENDING_STATE = "P";
	public static final String ERROR_FAIL_STATE = "E";
	public static final String EXECUTING_STATE = "X";
	public static final String FINISHED_STATE = "F";
	private static String tryInput;
	private static String tryOutput;
	private static String tryEngine;
	
	static { //static constructor of class
		
		tryInput = System.getenv("BOTQUEUE_INPUT_FILE");
		if (tryInput == null || tryInput.trim().isEmpty() ){
			tryInput = "/tmp/defaultInputJson.json";
		}
		
		tryOutput = System.getenv("BOTQUEUE_OUTPUT_FILE");
		if (tryOutput == null || tryOutput.trim().isEmpty() ){
			tryOutput = "/tmp/defaultOutputJson.json";
		}
		
		tryEngine = System.getenv("BOTQUEUE_CODE");
		if (tryEngine == null || tryEngine.trim().isEmpty() ){
			tryEngine = "/tmp/Engine";
		}
		
	}
	
	public static final String BOTQUEUE_INPUT_FILE = tryInput;
	public static final String BOTQUEUE_OUTPUT_FILE =  tryOutput;
	public static final String BOTQUEUE_CODE = System.getenv("BOTQUEUE_CODE");
	public final static String BOTQUEUE_PROGRAM = "mpirun";
	public final static String NUMBER_OF_PROCESS = "-np";
	public final static String ERROR_TOKEN_IN_OUTPUT_FILE = "Error";
}
