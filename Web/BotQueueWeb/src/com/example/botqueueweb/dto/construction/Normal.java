package com.example.botqueueweb.dto.construction;

import java.util.List;

import com.example.botqueueweb.dto.input.Delay;
import com.google.code.morphia.annotations.Embedded;

@Embedded
public class Normal {

	private Integer idNode;
	private String name;
	private Integer posX;
	private Integer posY;
	private List <Integer> preceders;
	private List <Integer> followers;
	private Delay delay;
	private Boolean probBranch;
	private List<Double> probabilisticBranch; //TODO: optional, si existe mas de dos elementos
	
	public Integer getIdNode() {
		return idNode;
	}
	public void setIdNode(Integer idNode) {
		this.idNode = idNode;
	}
	public List<Integer> getPreceders() {
		return preceders;
	}
	public void setPreceders(List<Integer> preceders) {
		this.preceders = preceders;
	}
	public List<Integer> getFollowers() {
		return followers;
	}
	public void setFollowers(List<Integer> followers) {
		this.followers = followers;
	}
	public Delay getDelay() {
		return delay;
	}
	public void setDelay(Delay delay) {
		this.delay = delay;
	}
	public List<Double> getProbabilisticBranch() {
		return probabilisticBranch;
	}
	public void setProbabilisticBranch(List<Double> probabilisticBranch) {
		this.probabilisticBranch = probabilisticBranch;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
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
	public Boolean getProbBranch() {
		return probBranch;
	}
	public void setProbBranch(Boolean probBranch) {
		this.probBranch = probBranch;
	}

	
}
