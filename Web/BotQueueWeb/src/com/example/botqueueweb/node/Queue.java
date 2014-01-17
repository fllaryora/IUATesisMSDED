package com.example.botqueueweb.node;

public class Queue extends Node{

	private String resource;
	private String fixedCost;
	private String variableCost;
	
	public String getResource() {
		return resource;
	}
	public void setResource(String resource) {
		this.resource = resource;
	}
	public String getFixedCost() {
		return fixedCost;
	}
	public void setFixedCost(String fixedCost) {
		this.fixedCost = fixedCost;
	}
	public String getVariableCost() {
		return variableCost;
	}
	public void setVariableCost(String variableCost) {
		this.variableCost = variableCost;
	}
	
}
