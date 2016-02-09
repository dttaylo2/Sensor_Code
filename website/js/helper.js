// Initialize collapse button
$(".button-collapse").sideNav();

// Load charts
google.charts.load('current', {packages: ['corechart', 'line']});
google.charts.setOnLoadCallback(drawTemperatureChart);
google.charts.setOnLoadCallback(drawCurrentChart);

function drawTemperatureChart() {
      var data = new google.visualization.DataTable();
      data.addColumn('number', 'X');
      data.addColumn('number', 'Temperature');

      var dataRows = [];

      for(i = 0; i < 25; i++) {
      	if(i === 10) {
      		dataRows.push([i, 50]);
      	} else {
      		dataRows.push([i, Math.floor((Math.random() * 11) + 85)]);
      	}
      }

      data.addRows(dataRows);

      var options = {
        hAxis: {
          title: 'Time'
        },
        vAxis: {
          title: 'Temperature',
          minValue: '0',
          maxValue: '100'
        },
        backgroundColor: '#FFFFFF',
        chartArea: {
        	width: '70%'
        },
        series: {
            0: { color: '#1976D2' }
        }
      };

      var chart = new google.visualization.LineChart(document.getElementById('temp_chart_div'));
      chart.draw(data, options);
}

function drawCurrentChart() {
      var data = new google.visualization.DataTable();
      data.addColumn('number', 'X');
      data.addColumn('number', 'Current');

      var dataRows = [];

      for(i = 0; i < 25; i++) {
      	if(i === 20) {
      		dataRows.push([i, 15]);
      	} else {
      		dataRows.push([i, Math.floor((Math.random() * 11))]);
      	}
      }

      data.addRows(dataRows);

      var options = {
        hAxis: {
          title: 'Time'
        },
        vAxis: {
          title: 'Current',
          minValue: '0',
          maxValue: '25'
        },
        backgroundColor: '#FFFFFF',
        chartArea: {
        	width: '70%'
        },
        series: {
            0: { color: '#F44336' }
        }
      };

      var chart = new google.visualization.LineChart(document.getElementById('cur_chart_div'));
      chart.draw(data, options);
}