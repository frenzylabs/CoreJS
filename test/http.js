(function(){
print("\nIN HTTP\n");
//	var client = new httpClient();
//	client.post("")
	var str = Http.post("http://localhost/test.php", {name:"blah", sex:"male"}, function(data){ print("inside callback = "+data); });
//	var getstr = client.get("http://www.google.com");
	print(str);
//	f.open("blah2.txt", "w+r");
	
})();