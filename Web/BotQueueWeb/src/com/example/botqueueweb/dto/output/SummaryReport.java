package com.example.botqueueweb.dto.output;

import java.util.List;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class SummaryReport {
	
	private Double totalTime;
	private List<CounterFinal> counters;
	private List<QueueFinal> queues;
	private Double totalCost;
	
	public Double getTotalTime() {
		return totalTime;
	}
	public void setTotalTime(Double totalTime) {
		this.totalTime = totalTime;
	}
	public List<CounterFinal> getCounters() {
		return counters;
	}
	public void setCounters(List<CounterFinal> counters) {
		this.counters = counters;
	}
	public List<QueueFinal> getQueues() {
		return queues;
	}
	public void setQueues(List<QueueFinal> queues) {
		this.queues = queues;
	}
	public Double getTotalCost() {
		return totalCost;
	}
	public void setTotalCost(Double totalCost) {
		this.totalCost = totalCost;
	}
	
}
