$(function() {
	$.ajax({
		url: '/getData',
		type: 'GET',
		success: function(res) {
			var dataObj = JSON.parse(res);

			$.each(dataObj, function(index, value) {
				var $tr = $('<tr>')
					.append($('<td>').text(value.Date))
					.append($('<td>').text(value.Vibration))
					.append($('<td>').text(value.Temperature))
					.append($('<td>').text(value.Current))
					.append($('<td>').text(value.Speed));

				$('.tbody').append($tr);
			});
		},
		error: function(error) {
			console.log(error);
		}
	});
});