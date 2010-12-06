(function(){
   var memcache = new require("./modules/Memcache/memcache").Memcache;
   //memcache.addServer("localhost", 11211);
   var m = new memcache();
   m.addServer("localhost", 11211);
   m.set("testing", "hello world");
   
   print("\n" + JSON.stringify(m.get("testing")) + "\n");
})();