(function(){

print("a new server = ");
var server = new serverSocket(8888);	
// server.onConnect(function(data){
// 	print("user has been connected\n");
// });
server.onDisconnect(function(data){
	print("User has left\n");
});
server.onData(function(data){
	if(data == "Notify"){
		server.onConnect(function(data){
			print("User has connected\n");
		});
	}
	print("received data= "+data+"\n");
});
print("end of file\n");

	
})();