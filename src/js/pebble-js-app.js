Pebble.addEventListener('ready', function() {
  getQuote();
});

//HTTP request for inspirational quote
var getQuote = function() {
  var request = new XMLHttpRequest();
  request.open('GET',
               'http://api.forismatic.com/api/1.0/?method=getQuote&key=457653&format=json&lang=en',
               true);
  request.onload = sendQuote;
  request.send();
};

//Process JSON
var sendQuote = function(xhr) {
	try {
		var response = JSON.parse(xhr.target.response);
	} catch(err) {
		var response = JSON.parse(xhr.target.response);
	}
	var quote = response.quoteText + "\n-" + response.quoteAuthor;
  Pebble.sendAppMessage({
    'motivate': quote
  });
};



