// DOMContentLoaded is fired once the document has been loaded and parsed,
// but without waiting for other external resources to load (css/images/etc)
// That makes the app more responsive and perceived as faster.
// https://developer.mozilla.org/Web/Reference/Events/DOMContentLoaded
window.addEventListener('DOMContentLoaded', function() {
  var command;
  ws = new WebSocket('ws://192.168.11.15:8888/') ;
  ws.onopen = function() {
    
  };

     // Log errors
  ws.onerror = function (error) {
    console.log('WebSocket Error ' + error);
  };

     // Log messages from the server
  ws.onmessage = function (e) {
    console.log('Server: ' + e.data);
    command = e.data;
    if (command === "0") {
      console.log(command);
    }
    navigator.requestGPIOAccess()
    .then(gpioAccess=>{
      var port1 = gpioAccess.ports.get(198);
      var port2 = gpioAccess.ports.get(199);
      var port3 = gpioAccess.ports.get(245);
      var port4 = gpioAccess.ports.get(243);
      return Promise.all([
        port1.export("out"),
        port2.export("out"),
        port3.export("out"),
        port4.export("out")
        ]).then(()=>{
        if (command == 0){
          console.log(command);
          port1.write(0);
          port2.write(0);
          port3.write(0);
          port4.write(0);
        }
        if (command == 1){
          console.log(command);
          port1.write(1);
          port2.write(1);
          setTimeout(function(){
            port1.write(0);
            port2.write(0);
          },5000);
        }
        if (command == 2){
          console.log(command);
          port1.write(1);
          port4.write(1);
          setTimeout(function(){
            port1.write(0);
            port4.write(0);
          },5000);
        }
        if (command == 3){
          console.log(command);
          port2.write(1);
          port3.write(1);
          setTimeout(function(){
            port2.write(0);
            port3.write(0);
          },5000);
        }
        if (command == 4){
          console.log(command);
          port3.write(1);
          port4.write(1);
          setTimeout(function(){
            port3.write(0);
            port4.write(0);
          },5000);
        }
      });
  });
  };
});
