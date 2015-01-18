Pebble.addEventListener('ready',
  function(e) {
    var req = new XMLHttpRequest();
req.open('GET', 'http://api.forismatic.com/api/1.0/?method=getQuote&key=457653&format=text&lang=en', true);
req.onload = function(e) {
  if (req.readyState == 4 && req.status == 200) {
    if(req.status == 200) {
      var response = req.responseText;
      console.log(response);
      Pebble.showSimpleNotificationOnPebble("Sample", response);
    } else { console.log('Error'); }
  }
}
req.send(null);
  }
);


