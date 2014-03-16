package com.example.botqueueweb.dto.construction;

import java.util.List;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class Queue {
	
	private Integer idNode;
	private String name;
	private Integer posX;
	private Integer posY;
	private Integer resource;
	private List<Integer> preceders;
	private List<Integer> followers;
	private Double fixedCost;
	private Double variableCost;
	
	public Integer getIdNode() {
		return idNode;
	}
	public void setIdNode(Integer idNode) {
		this.idNode = idNode;
	}
	public Integer getResource() {
		return resource;
	}
	public void setResource(Integer resource) {
		this.resource = resource;
	}
	public List<Integer> getPreceders() {
		return preceders;
	}
	public void setPreceders(List<Integer> preceders) {
		this.preceders = preceders;
	}
	public List<Integer> getFollowers() {
		return followers;
	}
	public void setFollowers(List<Integer> followers) {
		this.followers = followers;
	}
	public Double getFixedCost() {
		return fixedCost;
	}
	public void setFixedCost(Double fixedCost) {
		this.fixedCost = fixedCost;
	}
	public Double getVariableCost() {
		return variableCost;
	}
	public void setVariableCost(Double variableCost) {
		this.variableCost = variableCost;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public Integer getPosX() {
		return posX;
	}
	public void setPosX(Integer posX) {
		this.posX = posX;
	}
	public Integer getPosY() {
		return posY;
	}
	public void setPosY(Integer posY) {
		this.posY = posY;
	}
	
	
}