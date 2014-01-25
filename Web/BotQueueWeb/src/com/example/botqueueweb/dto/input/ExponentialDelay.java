package com.example.botqueueweb.dto.input;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class ExponentialDelay extends Delay{

	private Double lambda;
	private Integer seed;
	
	public Double getLambda() {
		return lambda;
	}
	public void setLambda(Double lambda) {
		this.lambda = lambda;
	}
	public Integer getSeed() {
		return seed;
	}
	public void setSeed(Integer seed) {
		this.seed = seed;
	}
		
}
