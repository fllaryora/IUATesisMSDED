package com.example.botqueueweb.dto.output;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class CounterFinal {
	
	private Integer idNode;
	private Integer totalProductivity;
	
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
	
}
