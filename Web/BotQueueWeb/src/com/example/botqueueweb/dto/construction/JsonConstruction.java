package com.example.botqueueweb.dto.construction;

import java.util.List;

import com.example.botqueueweb.dto.input.Combi;
import com.google.code.morphia.annotations.Embedded;

@Embedded
public class JsonConstruction {
	private Integer length;
	private Integer seed;
	private Transformation transformation;
	private List<Arrow> arrows;
	
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
	public List<Arrow> getArrows() {
		return arrows;
	}
	public void setArrows(List<Arrow> arrows) {
		this.arrows = arrows;
	}

}
