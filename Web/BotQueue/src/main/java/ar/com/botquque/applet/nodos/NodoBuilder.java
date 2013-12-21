package ar.com.botquque.applet.nodos;


public  class NodoBuilder {
	//asumo que todo esta en el mismo paquete
	public static String COLA = "Cola";
	public static String CONTADOR = "Contador";
	public static String COMBI =  "Combi";
	public static String NORMAL = "Normal";
	public static String FUNCION =  "Funcion";
	public static String NODOFLECHA =  "NodoFlecha";
	
	//le paso el nombre cualificado de la clase y le juro que es un nodo
	public static Nodo createNodo(String nombreClase, int posX, int posY, String mensaje){
		try{
			nombreClase = NodoBuilder.class.getPackage().toString().substring(8) + "."+nombreClase;
			return (Nodo) Class.forName(nombreClase).getConstructor(Integer.TYPE,Integer.TYPE, String.class).newInstance(posX, posY, mensaje); 
		} catch( Exception e){
			System.out.println(e);
			System.err.println(e);
		}	
		return null;
	}
	
	public static Nodo createNodo(String nombreClase, int posX, int posY, String mensaje, int orden){
		try{
			nombreClase = NodoBuilder.class.getPackage().toString().substring(8) + "."+nombreClase;
			return (Nodo) Class.forName(nombreClase).getConstructor(Integer.TYPE, Integer.TYPE, String.class, Integer.TYPE).newInstance(posX, posY, mensaje, orden); 
		} catch( Exception e){
			System.out.println(e);
			System.err.println(e);
		}	
		return null;
	}
	
}
