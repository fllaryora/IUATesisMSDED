window.com_example_testjavascriptcomponent_Chart =
	
	function () {

		var domId = this.getState().domId;
		var type = this.getState().type;
		
		var chartData;
		
		if (type=="Bar")
		{
			chartData = new Object();
			chartData.labels = this.getState().labels;
			chartData.datasets=new Array();
			for (var i=0 ; i<this.getState().series.length ; i++)
			{
				var element = {
						fillColor : this.getState().fillColors[i],
						strokeColor : this.getState().strokeColors[i],
						data : this.getState().series[i]
					};
				chartData.datasets.push(element);
			}
		}
		if (type=="Pie")
		{
			chartData = new Array();
			for (var i=0 ; i<this.getState().percents.length ; i++)
			{
				var element = {
						value : this.getState().percents[i],
						color : this.getState().colors[i]
					};
				chartData.push(element);
			}
		}
		if (type=="Line")
		{
			chartData = {
				labels : ["January","February","March","April","May","June","July"],
				datasets : [
					{
						fillColor : "rgba(220,220,220,0.5)",
						strokeColor : "rgba(220,220,220,1)",
						pointColor : "rgba(220,220,220,1)",
						pointStrokeColor : "#fff",
						data : [65,59,90,81,56,55,40]
					},
					{
						fillColor : "rgba(151,187,205,0.5)",
						strokeColor : "rgba(151,187,205,1)",
						pointColor : "rgba(151,187,205,1)",
						pointStrokeColor : "#fff",
						data : [28,48,40,19,96,27,100]
					}
				]
			}
		}
			
		
        var canvasElement = document.createElement('canvas');
        canvasElement.setAttribute('id', domId);
        
        canvasElement.setAttribute('width', "600"); // deberia ser un atributo
        canvasElement.setAttribute('height', "300"); // deberia ser un atributo
        
        if (type=="Bar")
		{
	        var myLine = new Chart(canvasElement.getContext("2d")).Bar(chartData);
		}   
        if (type=="Pie")
		{
        	var myLine = new Chart(canvasElement.getContext("2d")).Pie(chartData);
		}
        if (type=="Line")
		{
        	var myLine = new Chart(canvasElement.getContext("2d")).Line(chartData);
		}
        
        this.getElement().appendChild(canvasElement);
        
	    var chart = new Chart({
		        renderTo: domId,
		        value: this.getState().value
	    	});
	   chart.draw();
    
	    /*this.onStateChange = function () {
	    	chart.setValue(this.getState().value);
	    	};*/
	    	
	};