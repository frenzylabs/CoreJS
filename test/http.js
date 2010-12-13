(function(){
//	var client = new httpClient();
//	client.post("")
//	Http.test();
//	Http.test2("http://localhost/test.php", {name:"blah", sex:"male"}, function(data){ print("\ninside first callback = "+data+"\n"); });
//	print("after first request \n")
//	Http.test("http://localhost/test.php", {name:"blah", sex:"male"}, function(data){ print("\ninside sleeping second callback = "+data+"\n"); });
//	print("After second request \n");
//	Http.test2("http://localhost/test.php", {name:"blah", sex:"male"}, function(data){ print("\ninside third callback = "+data+"\n"); });
	
//	Http.test("http://localhost/test.php", {name:"blah", sex:"male"}, function(data){ print("\ninside sleeping fourth callback = "+data+"\n"); });
Http.post("http://localhost/test.php", {name:"first", sex:"male"}, function(data){ print("\n inside callback = "+data+"\n\n"); });
//print("\n**** second call =\n")
Http.post("http://localhost/test.php", {name:"second", sex:"male"}, function(data){ print("\n inside second callback = \n"+data); });
Http.post("http://localhost/test.php", {name:"third", sex:"male"}, function(data){ print("\n inside third callback =\n "+data); });
Http.post("http://localhost/test.php", {name:"fourth", sex:"male"}, function(data){ print("\n inside fourth callback = \n"+data); });
var str = Http.post("http://localhost/test.php", {name:"billy", sex:"uni"});//, function(data){ print("inside second callback = "+data); });
print("\nthe sync string = "+str+"\n");
//	var getstr = client.get("http://www.google.com");
//	print(str);
//	f.open("blah2.txt", "w+r");
	print("END OF HTTP.JS FILE");
})();