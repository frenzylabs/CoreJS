
(function(){
	//return isFile('blah.js');
	// var w = writeFile("writeTest.txt", "This is testing global writeFile function");
	// //return w;
	// var str = readFile('writeTest.txt');
	// 
	// var rem = removeFile("writeTest.txt");
	// return str+' remove = '+rem;
	
	var f = new File();//'blah.js', 'r');
	f.open("blah2.txt", "w+r");
	
	//var blah = f.remove();
	//return blah;
	// var str = 'string= ';
	// 	for(var i in f){
	// 		str += 'i= '+i+' f= '+ f[i] + '::::: ';
	// 	}
//	return str;
	f.write("some text to put into the file");
	
	//var str = f.read();
//	f.close();
//	return 'true';//str;
	
	//f.read();
	var ff = new File('test.txt', 'r');
	var str2 = ff.read();
	var str = f.read();
	f.close();
	var rew = ff.rewind();
	
	//return rew;
	
	return "The content for str= "+str+" and content for str2= "+str2;
	
	f.read();
	
	var f3 = new File('test2.txt', 'r');
	f3.read();
	ff.read();
	return "filepath = "+f.getFilePath()+" fpath = "+ff.getFilePath()+" f3mode = "+f3.getFileMode();
	ff.read();
	return JS_SYS_PATH;
	
})();