(function(){

print("a new client socket = \n");
var server = new clientSocket("http://localhost", 8888);	

server.onData(function(data){
	if(data == "close"){
		server.close();
	}else{
		print("inside Data callback and received data and = "+data+"\n");
	}
});

print("\nEnd of javascript File\n");
})();