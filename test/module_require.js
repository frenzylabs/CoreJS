(function(){
   print("In 'module_require'\n");
   var tm = require("./testmodule/innertest");
   print("\nAnnddd. Next!!");
   var gfde = require("./directory");
})();