package com.example.botqueueweb.dto.input;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class BetaDelay extends Delay{

	private Double minimun;
	private Double maximun;
	private Double shapeAlpha;
	private Double shapeBeta;
	private Integer seed;
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
		
}
