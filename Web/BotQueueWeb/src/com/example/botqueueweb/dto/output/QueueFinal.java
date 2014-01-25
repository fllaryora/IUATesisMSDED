package com.example.botqueueweb.dto.output;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class QueueFinal {
	
	private Integer idNode;
	private Double fixCost;
	private Double variableCost;
	
	public Integer getIdNode() {
		return idNode;
	}
	public void setIdNode(Integer idNode) {
		this.idNode = idNode;
	}
	public Double getFixCost() {
		return fixCost;
	}
	public void setFixCost(Double fixCost) {
		this.fixCost = fixCost;
	}
	public Double getVariableCost() {
		return variableCost;
	}
	public void setVariableCost(Double variableCost) {
		this.variableCost = variableCost;
	}
	
}
