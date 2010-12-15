
(function(){
	
	var f = new File('blah1.txt', 'w+r');
//	f.open();
	//f.open("blah1.txt", "w+r");
	
	//var blah = f.remove();
	//return blah;
	// var str = 'string= ';
	// 	for(var i in f){
	// 		str += 'i= '+i+' f= '+ f[i] + '::::: ';
	// 	}
//	return str;
	f.write("some text to put into the file");
	
	var str = f.read();
	print(str);
	f.onReadCb(function(data){
		print("in callback"+data+"\n");
	});
//	f.close();

	
})();