// Initialize collapse button
$(".button-collapse").sideNav();

// Load charts
google.charts.load('current', {packages: ['corechart', 'line']});
google.charts.setOnLoadCallback(getData);

function getData() {
  $.ajax({
    url: '/getData',
    type: 'GET',
    success: function(res) {
      var dataObj = JSON.parse(res);

      drawTemperatureChart(dataObj);
      drawCurrentChart(dataObj);
      drawSpeedChart(dataObj);

      console.log(res);
    },
    error: function(error) {
      console.log(error);
    }
  });
}

function drawTemperatureChart(dataObj) {
  var data = new google.visualization.DataTable();
  data.addColumn('number', 'X');
  data.addColumn('number', 'Temperature');

  var dataRows = [];

  $.each(dataObj, function(index, value) {
    dataRows.push([index, value.Temperature]);
  });

  data.addRows(dataRows);

  var options = {
    hAxis: {
      title: 'Time'
    },
    vAxis: {
      title: 'Temperature',
      minValue: '20',
      maxValue: '40'
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

function drawCurrentChart(dataObj) {
  var data = new google.visualization.DataTable();
  data.addColumn('number', 'X');
  data.addColumn('number', 'Current');

  var dataRows = [];

  $.each(dataObj, function(index, value) {
    dataRows.push([index, value.Current]);
  });

  data.addRows(dataRows);

  var options = {
    hAxis: {
      title: 'Time'
    },
    vAxis: {
      title: 'Current',
      minValue: '0',
      maxValue: '2'
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

function drawSpeedChart(dataObj) {
  var data = new google.visualization.DataTable();
  data.addColumn('number', 'X');
  data.addColumn('number', 'Speed');

  var dataRows = [];

  $.each(dataObj, function(index, value) {
    dataRows.push([index, value.Speed]);
  });

  data.addRows(dataRows);

  var options = {
    hAxis: {
      title: 'Time'
    },
    vAxis: {
      title: 'Speed',
      minValue: '0',
      maxValue: '3000'
    },
    backgroundColor: '#FFFFFF',
    chartArea: {
      width: '70%'
    },
    series: {
      0: { color: '#4CAF50' }
    }
  };

    var chart = new google.visualization.LineChart(document.getElementById('speed_chart_div'));
    chart.draw(data, options);
}