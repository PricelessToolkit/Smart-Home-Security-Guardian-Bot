class HABot extends HTMLElement {
  // Whenever the state changes, a new `hass` object is set. Use this to
  // update your content.
  set hass(hass) {
    // Initialize the content if it's not there yet.
    if (!this.content) {
      this.innerHTML = `
        <ha-card header="Custom Cards HABot">
          <div class="card-content"></div>
        </ha-card>
      `;
      this.content = this.querySelector('div');
    }



    this.content.innerHTML = `
<html>

  <head>
  <script src="jquery-3.5.0.js"></script>
  <script src="mqttws31.min.js"></script>
  <script src="app.js"></script>
  
	
	
	
	  <style>
         input[id=forward] {
            background-color: rgb(3 169 244);
            border:2px solid #e1e1e1;
            text-decoration: none;
            color: white;
            padding: 15px 15px;
            margin: 10px 20px;
			margin:0 auto;
			display:block;
            cursor: pointer;
			width:35%;
			
         }
      </style>
	  <style>
         input[id=backward] {
            background-color: rgb(3 169 244);
            border:2px solid #e1e1e1;
            text-decoration: none;
            color: white;
            padding: 15px 15px;
            margin: 10px 20px;
			margin:0 auto;
			display:block;
            cursor: pointer;
			width:35%;
			
         }
      </style>
	  <style>
         input[id=left] {
            background-color: rgb(3 169 244);
            border:2px solid #e1e1e1;
            text-decoration: none;
            color: white;
            padding: 20px 20px;
            margin: 10px 10px;
			float:left;
            cursor: pointer;
			width:35%;
			
         }
      </style>
	  <style>
         input[id=right] {
            background-color: rgb(3 169 244);
            border:2px solid #e1e1e1;
            text-decoration: none;
            color: white;
            padding: 20px 20px;
            margin: 10px 10px;
			float:right;
            cursor: pointer;
			width:35%;
			
         }
      </style>
	  
  </head>
  <!-- this is an HTML comment -->
  

	
	<body style="background-color:rgb(28 28 28);">
			<input type="button" value="FORWARD" id="forward">	
			<input type="button" value="LEFT" id="left">
			<input type="button" value="RIGHT" id="right">
			<input type="button" value="BACKWARD" id="backward">

 
 
 
	<script>
	$( "[id^=left]" ).on('mousedown touchstart',function(){
		$(left());
	  });
	$( "[id^=left]" ).on('mouseup touchend',function(){
		$(stop());
	  });
	  
	  
	$( "[id^=right]" ).on('mousedown touchstart',function(){
		$(right());
	  });
	$( "[id^=right]" ).on('mouseup touchend',function(){
		$(stop());
	  });
	  

	$( "[id^=forward]" ).on('mousedown touchstart',function(){
		$(forward());
	  });
	$( "[id^=forward]" ).on('mouseup touchend',function(){
		$(stop());
	  });
	  
	$( "[id^=backward]" ).on('mousedown touchstart',function(){
		$(backward());
	  });
	$( "[id^=backward]" ).on('mouseup touchend',function(){
		$(stop());
	  });
	  
	  
	  
	  
	</script>



	</body>

</html>
	  
    `;
  }

  // The user supplied configuration. Throw an exception and Lovelace will
  // render an error card.
  setConfig(config) {
    this.config = config;
  }

  // The height of your card. Home Assistant uses this to automatically
  // distribute all cards over the available columns.
  getCardSize() {
    return 3;
  }
}


customElements.define('habot-card', HABot);