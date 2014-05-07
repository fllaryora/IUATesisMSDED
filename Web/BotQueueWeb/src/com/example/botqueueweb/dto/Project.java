package com.example.botqueueweb.dto;

import org.bson.types.ObjectId;

import com.example.botqueueweb.dto.construction.JsonConstruction;
import com.example.botqueueweb.dto.input.JsonInput;
import com.example.botqueueweb.dto.output.JsonOutput;
import com.google.code.morphia.annotations.Entity;
import com.google.code.morphia.annotations.Id;
import com.google.code.morphia.annotations.Reference;

/**
 * Clase de dominio de un proyecto
 *
 */
@Entity("projects")
public class Project {
	
	private @Id ObjectId id;
	private String name;
	private String state;
	private String constructionStamp;
	private String pendingStamp;
	private String executingStamp;
	private String finishingStamp;
	private String lastUpdatedStamp;
	private JsonConstruction construction;
	private JsonInput input;
	private Integer nroProcs;
	private Double deltaT;
	private Double simTime;
	private JsonOutput output;
	
	@Reference
	private User user;
	
	public ObjectId getId() {
		return id;
	}
	public void setId(ObjectId id) {
		this.id = id;
	}
	public User getUser() {
		return user;
	}
	public void setUser(User user) {
		this.user = user;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getState() {
		return state;
	}
	public void setState(String state) {
		this.state = state;
	}
	public String getPendingStamp() {
		return pendingStamp;
	}
	public void setPendingStamp(String pendingStamp) {
		this.pendingStamp = pendingStamp;
	}
	public String getExecutingStamp() {
		return executingStamp;
	}
	public void setExecutingStamp(String executingStamp) {
		this.executingStamp = executingStamp;
	}
	public String getFinishingStamp() {
		return finishingStamp;
	}
	public void setFinishingStamp(String finishingStamp) {
		this.finishingStamp = finishingStamp;
	}
	public String getLastUpdatedStamp() {
		return lastUpdatedStamp;
	}
	public void setLastUpdatedStamp(String lastUpdatedStamp) {
		this.lastUpdatedStamp = lastUpdatedStamp;
	}
	public JsonInput getInput() {
		return input;
	}
	public void setInput(JsonInput input) {
		this.input = input;
	}
	public Integer getNroProcs() {
		return nroProcs;
	}
	public void setNroProcs(Integer nroProcs) {
		this.nroProcs = nroProcs;
	}
	public JsonOutput getOutput() {
		return output;
	}
	public void setOutput(JsonOutput output) {
		this.output = output;
	}
	public String getConstructionStamp() {
		return constructionStamp;
	}
	public void setConstructionStamp(String constructionStamp) {
		this.constructionStamp = constructionStamp;
	}
	public JsonConstruction getConstruction() {
		return construction;
	}
	public void setConstruction(JsonConstruction construction) {
		this.construction = construction;
	}
	public Double getDeltaT() {
		return deltaT;
	}
	public void setDeltaT(Double deltaT) {
		this.deltaT = deltaT;
	}
	public Double getSimTime() {
		return simTime;
	}
	public void setSimTime(Double simTime) {
		this.simTime = simTime;
	}

}
