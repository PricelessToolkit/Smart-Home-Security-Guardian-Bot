Thys is for test "this code doesn't work"

If u have experience creating custom UI cards for home assistant, then go ahead and try to make it works. I didn't succeed .

I think problem is here 

  <script src="jquery-3.5.0.js"></script>
  <script src="mqttws31.min.js"></script>
  <script src="app.js"></script>


i need to do something like this

import "jquery-3.5.0.js?module";
import "mqttws31.min.js?module";
import "app.js?module";

but UI give me an error