window.com_example_botqueueweb_js_Chart =
	
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
			chartData = new Object();
			chartData.labels = this.getState().labels;
			chartData.datasets=new Array();
			for (var i=0 ; i<this.getState().points.length ; i++)
			{
				var element = {
						fillColor : this.getState().fillColors[i],
						strokeColor : this.getState().strokeColors[i],
						pointColor : this.getState().pointColors[i],
						pointStrokeColor : this.getState().pointStrokeColor,
						data : this.getState().points[i]
					};
				chartData.datasets.push(element);
			}
		}
		
		var options = new Object();
		options.animation = false;
		
        var canvasElement = document.createElement('canvas');
        canvasElement.setAttribute('id', domId);
        canvasElement.setAttribute('width', this.getState().widthJS); // deberia ser un atributo
        canvasElement.setAttribute('height', this.getState().heightJS); // deberia ser un atributo
        
        if (type=="Bar")
		{
	        var myLine = new Chart(canvasElement.getContext("2d")).Bar(chartData,options);
		}   
        if (type=="Pie")
		{
        	var myLine = new Chart(canvasElement.getContext("2d")).Pie(chartData,options);
		}
        if (type=="Line")
		{
        	var myLine = new Chart(canvasElement.getContext("2d")).Line(chartData,options);
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