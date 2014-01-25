package com.example.botqueueweb.dto.output;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class Counter {

	private Integer idNode;
	private Integer totalProductivity;
	private Integer deltaTProductivity;
	private Double productivityPerTime;
	
	public Integer getIdNode() {
		return idNode;
	}
	public void setIdNode(Integer idNode) {
		this.idNode = idNode;
	}
	public Integer getTotalProductivity() {
		return totalProductivity;
	}
	public void setTotalProductivity(Integer totalProductivity) {
		this.totalProductivity = totalProductivity;
	}
	public Integer getDeltaTProductivity() {
		return deltaTProductivity;
	}
	public void setDeltaTProductivity(Integer deltaTProductivity) {
		this.deltaTProductivity = deltaTProductivity;
	}
	public Double getProductivityPerTime() {
		return productivityPerTime;
	}
	public void setProductivityPerTime(Double productivityPerTime) {
		this.productivityPerTime = productivityPerTime;
	}

}
