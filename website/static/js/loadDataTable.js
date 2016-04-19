$(function() {
	setInterval(
	    function()
	    {
	      $.ajax({
			url: '/getTableData',
			type: 'GET',
			success: function(res) {
				var dataObj = JSON.parse(res);

				var classifiers = ['Not operating.', 'Operating without cutting.', 'Cutting with a sharp bit.', 'Cutting with a dull bit.', 'Bad headstock bearing.'];
				
				$(".tbody tr").remove(); 

				$.each(dataObj, function(index, value) {
					var $tr = $('<tr>')
						.append($('<td>').text(value.Date))
						.append($('<td>').text(Math.round(value.Vibration1)))
						.append($('<td>').text(Math.round(value.Vibration2)))
						.append($('<td>').text(value.Temperature))
						.append($('<td>').text(value.Current))
						.append($('<td>').text(value.Speed))
						.append($('<td>').text(classifiers[value.Classification]));

					$('.tbody').append($tr);
				});
			},
			error: function(error) {
				console.log(error);
			}
		  });
	    },
	    1000); 
});
