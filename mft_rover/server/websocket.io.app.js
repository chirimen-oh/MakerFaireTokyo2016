var ws = require('websocket.io');
var server = ws.listen(8888, function () {
  console.log('\033[96m Server running localhost:8888 \033[39m');
});

server.on('connection', function(socket) {
  socket.on('message', function(data) {
    server.clients.forEach(function(client) {
      client.send(data);
    });
  });
});
