(function(){
	var client = new HttpClient();
	client.post("http://localhost/test.php", {name:"object", place:"obj"}, function(data){ print("\n inside object callback = \n"+data+"\n"); });
	client.post("http://localhost/test.php", {name:"object2", place:"here"}, function(data){ print("\n inside second object callback = \n"+data+"\n"); });
	var str1 = client.post("http://localhost/test.php", {name:"object3", place:"here34"});
	print(str1);

	Http.post("http://localhost/test.php", {name:"fourth", sex:"male"}, function(data){ print("\n inside fourth callback = \n"+data); });
	var str = Http.post("http://localhost/test.php", {name:"billy", sex:"uni"});//, function(data){ print("inside second callback = "+data); });
	print("\nthe sync string = "+str+"\n");

	print("END OF HTTP.JS FILE");
})();