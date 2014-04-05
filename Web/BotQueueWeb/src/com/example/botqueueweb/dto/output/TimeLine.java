package com.example.botqueueweb.dto.output;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class TimeLine {

	private Double deltaT;
	private NodesStatus nodesStatus;
	
	public Double getDeltaT() {
		return deltaT;
	}
	public void setDeltaT(Double deltaT) {
		this.deltaT = deltaT;
	}
	
	public NodesStatus getNodesStatus() {
		return nodesStatus;
	}
	public void setNodesStatus(NodesStatus nodesStatus) {
		this.nodesStatus = nodesStatus;
	}
	
}
