package com.example.botqueueweb.dto.salida;

import java.util.List;

public class Normal {

	private Integer idNode;
	private List <Double> contdownWorktask;
	private List <Double> delayWorktask;
	private Integer counterInput;
	private Double amountDelay;
	private Double maximunDrawn;
	private Double minimunDrawn;
	
	public Integer getIdNode() {
		return idNode;
	}
	public void setIdNode(Integer idNode) {
		this.idNode = idNode;
	}
	public List<Double> getContdownWorktask() {
		return contdownWorktask;
	}
	public void setContdownWorktask(List<Double> contdownWorktask) {
		this.contdownWorktask = contdownWorktask;
	}
	public List<Double> getDelayWorktask() {
		return delayWorktask;
	}
	public void setDelayWorktask(List<Double> delayWorktask) {
		this.delayWorktask = delayWorktask;
	}
	public Integer getCounterInput() {
		return counterInput;
	}
	public void setCounterInput(Integer counterInput) {
		this.counterInput = counterInput;
	}
	public Double getAmountDelay() {
		return amountDelay;
	}
	public void setAmountDelay(Double amountDelay) {
		this.amountDelay = amountDelay;
	}
	public Double getMaximunDrawn() {
		return maximunDrawn;
	}
	public void setMaximunDrawn(Double maximunDrawn) {
		this.maximunDrawn = maximunDrawn;
	}
	public Double getMinimunDrawn() {
		return minimunDrawn;
	}
	public void setMinimunDrawn(Double minimunDrawn) {
		this.minimunDrawn = minimunDrawn;
	}

}
