package com.example.botqueueweb.dao;

import com.google.code.morphia.Datastore;
import com.google.code.morphia.Morphia;
import com.mongodb.Mongo;

public class MongoConnection {
	 
    private static final String HOST = "localhost";
    private static final int PORT = 27017;
    private static final String DB_NAME = "botqueuedb";
 
    private static MongoConnection mongoConnection;
    private Mongo mongo;
    private Datastore ds;
    private Morphia morphia;
 
    private MongoConnection() {
        
    }
 
    public static synchronized MongoConnection getInstance()
    {
        if (mongoConnection == null)
        {
        	mongoConnection = new MongoConnection();
        }
        return mongoConnection;
    }
 
    @SuppressWarnings("deprecation")
	public Datastore getDatastore()
    {
        if (mongo == null)
        {
            try {
                mongo = new Mongo(HOST, PORT);
                morphia = new Morphia();
    			ds = morphia.createDatastore(mongo,DB_NAME);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return ds;
    }
}