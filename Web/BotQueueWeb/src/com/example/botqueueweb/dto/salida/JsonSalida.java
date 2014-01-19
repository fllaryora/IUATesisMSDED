package com.example.botqueueweb.dto.salida;

import java.util.List;

public class JsonSalida {
	
	private List<TimeLine> timesLine;
	private SummaryReport summaryReport;
	
	public List<TimeLine> getTimesLine() {
		return timesLine;
	}
	public void setTimesLine(List<TimeLine> timesLine) {
		this.timesLine = timesLine;
	}
	public SummaryReport getSummaryReport() {
		return summaryReport;
	}
	public void setSummaryReport(SummaryReport summaryReport) {
		this.summaryReport = summaryReport;
	}
	
}
