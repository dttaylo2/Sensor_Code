$(function() {
    $('#btnTest').click(function() {
 
        $.ajax({
            url: '/getData',
            type: 'POST',
            success: function(response) {
                console.log(response);
            },
            error: function(error) {
                console.log(error);
            }
        });
    });
});