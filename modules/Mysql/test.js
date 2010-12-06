(function(){
   
   var Mysql = require('./mysql.fm').Mysql;
   
   var mysql = new Mysql();
   
   if(mysql.connect("localhost", "root", "", "test", 3306))
      print("\nCONNECTED\n");
   else
      print("\n" + JSON.stringify(mysql.error()) + "\n");
})();