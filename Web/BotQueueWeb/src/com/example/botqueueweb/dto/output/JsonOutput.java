package com.example.botqueueweb.dto.output;

import java.util.List;

import org.bson.types.ObjectId;

import com.google.code.morphia.annotations.Embedded;
import com.google.code.morphia.annotations.Entity;
import com.google.code.morphia.annotations.Id;

@Embedded
public class JsonOutput {
	
	private List<TimeLine> timeLines;
	private SummaryReport summaryReport;
	

	public SummaryReport getSummaryReport() {
		return summaryReport;
	}
	public void setSummaryReport(SummaryReport summaryReport) {
		this.summaryReport = summaryReport;
	}
	
	public List<TimeLine> getTimeLines() {
		return timeLines;
	}
	public void setTimeLines(List<TimeLine> timeLines) {
		this.timeLines = timeLines;
	}
	
}
