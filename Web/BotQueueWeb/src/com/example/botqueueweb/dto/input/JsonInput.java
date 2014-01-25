package com.example.botqueueweb.dto.input;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class JsonInput {
	
	private Integer length;
	private Integer seed;
	private Transformation transformation;
	
	public Integer getLength() {
		return length;
	}
	public void setLength(Integer length) {
		this.length = length;
	}
	public Integer getSeed() {
		return seed;
	}
	public void setSeed(Integer seed) {
		this.seed = seed;
	}
	public Transformation getTransformation() {
		return transformation;
	}
	public void setTransformation(Transformation transformation) {
		this.transformation = transformation;
	}
	
}
