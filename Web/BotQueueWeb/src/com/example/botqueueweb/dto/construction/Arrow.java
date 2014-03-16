package com.example.botqueueweb.dto.construction;

import java.util.List;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class Arrow {
	
	private Integer tail;
	private Integer head;
	private Integer tailSurface;
	private Integer headSurface;
	private Boolean enableProb;
	private Double probabilisticBranch;
	private List<MiddlePoint> middlePoints;
	public Integer getTail() {
		return tail;
	}
	public void setTail(Integer tail) {
		this.tail = tail;
	}
	public Integer getHead() {
		return head;
	}
	public void setHead(Integer head) {
		this.head = head;
	}
	public Integer getTailSurface() {
		return tailSurface;
	}
	public void setTailSurface(Integer tailSurface) {
		this.tailSurface = tailSurface;
	}
	public Integer getHeadSurface() {
		return headSurface;
	}
	public void setHeadSurface(Integer headSurface) {
		this.headSurface = headSurface;
	}
	public Boolean getEnableProb() {
		return enableProb;
	}
	public void setEnableProb(Boolean enableProb) {
		this.enableProb = enableProb;
	}
	public Double getProbabilisticBranch() {
		return probabilisticBranch;
	}
	public void setProbabilisticBranch(Double probabilisticBranch) {
		this.probabilisticBranch = probabilisticBranch;
	}
	public List<MiddlePoint> getMiddlePoints() {
		return middlePoints;
	}
	public void setMiddlePoints(List<MiddlePoint> middlePoints) {
		this.middlePoints = middlePoints;
	}
	
}
