package com.example.botqueueweb.dto.salida;

import java.util.List;

public class TimeLine {

	private Double deltaT;
	private List<NodeStatus> nodesStatus;
	
	public Double getDeltaT() {
		return deltaT;
	}
	public void setDeltaT(Double deltaT) {
		this.deltaT = deltaT;
	}
	public List<NodeStatus> getNodesStatus() {
		return nodesStatus;
	}
	public void setNodesStatus(List<NodeStatus> nodesStatus) {
		this.nodesStatus = nodesStatus;
	} 
	
}
