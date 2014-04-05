package com.example.botqueueweb.business;

import com.example.botqueueweb.dao.MongoConnection;
import com.example.botqueueweb.dto.User;

public class UserBusiness {

	private static UserBusiness userBusiness = null;
	 
    private UserBusiness(){}
 
    private synchronized static void createInstance() {
        if (userBusiness == null) { 
        	userBusiness = new UserBusiness();
        }
    }
 
    public static UserBusiness getInstance() {
        createInstance();
        return userBusiness;
    }
    
    /**
     * Busca un usuario por username
     * @param user
     * @return la primer coincidencia 
     */
	public User getUserByName(User user)
	{
		try {
			for (User userDb : MongoConnection.getInstance().getDatastore().find(User.class).filter("username", user.getUsername())) {
				return userDb;
			}
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		
		return null;
		
	}
}