package com.example.botqueueweb.dto.output;

import java.util.List;
import com.google.code.morphia.annotations.Embedded;

@Embedded
public class JsonOutput {
	
	private List<TimeLine> timeLines;
	private SummaryReport summaryReport;
	private String Error;

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
	
	public String getError() {
		return Error;
	}
	public void setError(String error) {
		Error = error;
	}
	
	
}
