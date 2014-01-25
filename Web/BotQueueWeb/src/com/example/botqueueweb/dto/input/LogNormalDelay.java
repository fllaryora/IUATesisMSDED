package com.example.botqueueweb.dto.input;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class LogNormalDelay extends Delay{

	private Double escale;
	private Double shape;
	private Double minimun;
	private Integer seed;
	
	public Double getEscale() {
		return escale;
	}
	public void setEscale(Double escale) {
		this.escale = escale;
	}
	public Double getShape() {
		return shape;
	}
	public void setShape(Double shape) {
		this.shape = shape;
	}
	public Double getMinimun() {
		return minimun;
	}
	public void setMinimun(Double minimun) {
		this.minimun = minimun;
	}
	public Integer getSeed() {
		return seed;
	}
	public void setSeed(Integer seed) {
		this.seed = seed;
	}
	
}
