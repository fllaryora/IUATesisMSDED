package com.example.botqueueweb.node;

import java.util.List;

public class Node {

	private String idNode;
	private String name;
	private String type;
	private List<String> preceders;
	private List<String> followers;
	
	
	public String getIdNode() {
		return idNode;
	}
	public void setIdNode(String idNode) {
		this.idNode = idNode;
	}
	public List<String> getPreceders() {
		return preceders;
	}
	public void setPreceders(List<String> preceders) {
		this.preceders = preceders;
	}
	public List<String> getFollowers() {
		return followers;
	}
	public void setFollowers(List<String> followers) {
		this.followers = followers;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	

}
