package com.example.botqueueweb.dto.input;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class UniformDelay  extends Delay{

	private Double least;
	private Double highest;
	private Integer seed;
	
	public Double getLeast() {
		return least;
	}
	public void setLeast(Double least) {
		this.least = least;
	}
	public Double getHighest() {
		return highest;
	}
	public void setHighest(Double highest) {
		this.highest = highest;
	}
	public Integer getSeed() {
		return seed;
	}
	public void setSeed(Integer seed) {
		this.seed = seed;
	}
	
}
