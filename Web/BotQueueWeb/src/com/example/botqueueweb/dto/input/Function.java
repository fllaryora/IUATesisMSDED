package com.example.botqueueweb.dto.input;

import java.util.List;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class Function {

	private Integer idNode;
	private String name;
	private Integer input;
	private Integer output;
	private List<Integer> preceders;
	private List<Integer> followers;
	private List<Double> probabilisticBranch; //TODO: optional, si existe mas de dos elementos
	
	public Integer getIdNode() {
		return idNode;
	}
	public void setIdNode(Integer idNode) {
		this.idNode = idNode;
	}
	public Integer getInput() {
		return input;
	}
	public void setInput(Integer input) {
		this.input = input;
	}
	public Integer getOutput() {
		return output;
	}
	public void setOutput(Integer output) {
		this.output = output;
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

}
