package com.example.botqueueweb.dto.output;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class Queue {
	
	private Integer idNode;
	private Integer amount;
	private Integer counterInput;
	private Integer counterOutput;
	private Double average;
	private Integer maximun;
	private Integer minimun;
	private Double timesNotEmpty;
	private Double percentTimesNotEmpty;
	private Double averageDelay;
	
	public Integer getIdNode() {
		return idNode;
	}
	public void setIdNode(Integer idNode) {
		this.idNode = idNode;
	}
	public Integer getAmount() {
		return amount;
	}
	public void setAmount(Integer amount) {
		this.amount = amount;
	}
	public Integer getCounterInput() {
		return counterInput;
	}
	public void setCounterInput(Integer counterInput) {
		this.counterInput = counterInput;
	}
	public Integer getCounterOutput() {
		return counterOutput;
	}
	public void setCounterOutput(Integer counterOutput) {
		this.counterOutput = counterOutput;
	}
	public Double getAverage() {
		return average;
	}
	public void setAverage(Double average) {
		this.average = average;
	}
	public Integer getMaximun() {
		return maximun;
	}
	public void setMaximun(Integer maximun) {
		this.maximun = maximun;
	}
	public Integer getMinimun() {
		return minimun;
	}
	public void setMinimun(Integer minimun) {
		this.minimun = minimun;
	}
	public Double getTimesNotEmpty() {
		return timesNotEmpty;
	}
	public void setTimesNotEmpty(Double timesNotEmpty) {
		this.timesNotEmpty = timesNotEmpty;
	}
	public Double getPercentTimesNotEmpty() {
		return percentTimesNotEmpty;
	}
	public void setPercentTimesNotEmpty(Double percentTimesNotEmpty) {
		this.percentTimesNotEmpty = percentTimesNotEmpty;
	}
	public Double getAverageDelay() {
		return averageDelay;
	}
	public void setAverageDelay(Double averageDelay) {
		this.averageDelay = averageDelay;
	}
	
}
