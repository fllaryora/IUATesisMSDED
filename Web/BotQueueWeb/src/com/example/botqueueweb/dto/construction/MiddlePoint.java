package com.example.botqueueweb.dto.construction;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class MiddlePoint {
	
	private Integer posX;
	private Integer posY;
	private Integer order;
	
	public Integer getPosX() {
		return posX;
	}
	public void setPosX(Integer posX) {
		this.posX = posX;
	}
	public Integer getPosY() {
		return posY;
	}
	public void setPosY(Integer posY) {
		this.posY = posY;
	}
	public Integer getOrder() {
		return order;
	}
	public void setOrder(Integer order) {
		this.order = order;
	}
	
}
