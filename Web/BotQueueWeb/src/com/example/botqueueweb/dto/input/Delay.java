package com.example.botqueueweb.dto.input;

import com.google.code.morphia.annotations.Embedded;
import com.google.code.morphia.annotations.NotSaved;

@Embedded
public class Delay {

	private String distribution;
	private Double minimun;
	private Double maximun;
	private Double shapeAlpha;
	private Double shapeBeta;
	private Double constant;
	private Double lambda;
	private Double escale;
	private Double shape;
	private Double mean;
	private Double variance;
	private Double least;
	private Double highest;
	private Double mode;
	
	private Integer seed;

	public Double getLambda() {
		return lambda;
	}

	public void setLambda(Double lambda) {
		this.lambda = lambda;
	}

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

	public Double getMode() {
		return mode;
	}

	public void setMode(Double mode) {
		this.mode = mode;
	}

	public String getDistribution() {
		return distribution;
	}

	public void setDistribution(String distribution) {
		this.distribution = distribution;
	}

	public Double getMinimun() {
		return minimun;
	}

	public void setMinimun(Double minimun) {
		this.minimun = minimun;
	}

	public Double getMaximun() {
		return maximun;
	}

	public void setMaximun(Double maximun) {
		this.maximun = maximun;
	}

	public Double getShapeAlpha() {
		return shapeAlpha;
	}

	public void setShapeAlpha(Double shapeAlpha) {
		this.shapeAlpha = shapeAlpha;
	}

	public Double getShapeBeta() {
		return shapeBeta;
	}

	public void setShapeBeta(Double shapeBeta) {
		this.shapeBeta = shapeBeta;
	}

	public Integer getSeed() {
		return seed;
	}

	public void setSeed(Integer seed) {
		this.seed = seed;
	}

	public Double getConstant() {
		return constant;
	}

	public void setConstant(Double constant) {
		this.constant = constant;
	}
		
	
}
