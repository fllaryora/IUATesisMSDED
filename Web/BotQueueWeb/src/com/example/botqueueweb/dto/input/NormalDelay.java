package com.example.botqueueweb.dto.input;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class NormalDelay extends Delay{

	private Double mean;
	private Double variance;
	private Integer seed;

	public Double getMean() {
		return mean;
	}
	public void setMean(Double mean) {
		this.mean = mean;
	}
	public Double getVariance() {
		return variance;
	}
	public void setVariance(Double variance) {
		this.variance = variance;
	}
	public Integer getSeed() {
		return seed;
	}
	public void setSeed(Integer seed) {
		this.seed = seed;
	}

}
