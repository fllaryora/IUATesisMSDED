package com.example.botqueueweb.dto.input;

import java.util.List;

import com.google.code.morphia.annotations.Embedded;

@Embedded
public class Counter {

	private Integer idNode;
	private Integer quantity;
	private Integer cycle;
	private List<Integer> preceders;
	private List<Integer> followers;
	
	public Integer getIdNode() {
		return idNode;
	}
	public void setIdNode(Integer idNode) {
		this.idNode = idNode;
	}
	public Integer getQuantity() {
		return quantity;
	}
	public void setQuantity(Integer quantity) {
		this.quantity = quantity;
	}
	public Integer getCycle() {
		return cycle;
	}
	public void setCycle(Integer cycle) {
		this.cycle = cycle;
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

}
