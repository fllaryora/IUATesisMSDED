package ar.com.botquque.pruebas;
import java.lang.reflect.InvocationTargetException;

public  class NodoBuilder {
	//asumo que todo esta en el mismo paquete
	public static String COLA = NodoBuilder.class.getPackage().toString().substring(8) + ".Cola";
	public static String CONTADOR = NodoBuilder.class.getPackage().toString().substring(8) + ".Contador";
	public static String COMBI = NodoBuilder.class.getPackage().toString().substring(8) +  ".Combi";
	public static String NORMAL = NodoBuilder.class.getPackage().toString().substring(8) + ".Normal";
	public static String FUNCION = NodoBuilder.class.getPackage().toString().substring(8) + ".Funcion";
	//le paso el nombre cualificado de la clase y le juro que es un nodo
	public static Nodo createNodo(String nombreClase, int posX, int posY, String mensaje){
		try{
			
			return (Nodo) Class.forName(nombreClase).getConstructor(Integer.TYPE,Integer.TYPE, String.class).newInstance(posX, posY, mensaje); 
			 
		} catch( IllegalArgumentException iae){
			System.out.println("A");
		} catch( SecurityException se){
			System.out.println("e");
		} catch(InstantiationException ie){
			System.out.println("i");
		} catch( IllegalAccessException iae){
			System.out.println("o");
		} catch(InvocationTargetException ite){
			System.out.println("u");
		} catch( NoSuchMethodException nsme){
			System.out.println("r");
		} catch( ClassNotFoundException cnfe){
			System.out.println("q "+ nombreClase );
		}
		
		return null;
	}
	
}
