package com.example.botqueueweb.dto.input;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class DeterministicDelay extends Delay{

	private Double constant;

	public Double getConstant() {
		return constant;
	}

	public void setConstant(Double constant) {
		this.constant = constant;
	}
		
}
