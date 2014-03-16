package com.example.botqueueweb.dto.construction;

import java.util.List;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class Transformation {

	private List<Queue> queues;
	private List<Counter> counters;
	private List<Normal> normals;
	private List<Function> functions;
	private List<Combi> combis;
	
	public List<Queue> getQueues() {
		return queues;
	}
	public void setQueues(List<Queue> queues) {
		this.queues = queues;
	}
	public List<Counter> getCounters() {
		return counters;
	}
	public void setCounters(List<Counter> counters) {
		this.counters = counters;
	}
	public List<Normal> getNormals() {
		return normals;
	}
	public void setNormals(List<Normal> normals) {
		this.normals = normals;
	}
	public List<Function> getFunctions() {
		return functions;
	}
	public void setFunctions(List<Function> functions) {
		this.functions = functions;
	}
	public List<Combi> getCombis() {
		return combis;
	}
	public void setCombis(List<Combi> combis) {
		this.combis = combis;
	}
	
	
}