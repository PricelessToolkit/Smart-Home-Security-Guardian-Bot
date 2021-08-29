// Generate a new random MQTT client id on each page load
var MQTT_CLIENT_ID = "iot_web_"+Math.floor((1 + Math.random()) * 0x10000000000).toString(16);

// Create a MQTT client instance
var MQTT_CLIENT = new Paho.MQTT.Client("Your_HA_IP", 1884, "/ws", MQTT_CLIENT_ID);

// Tell the client instance to connect to the MQTT broker
MQTT_CLIENT.connect({userName : "Your_MQTT_Username", password : "Your_MQTT_Password", onSuccess: myClientConnected });

// This is the function which handles button clicks



function left() {
  // create a new MQTT message with a specific payload
  var left = new Paho.MQTT.Message("left");

  // Set the topic it should be published to
  left.destinationName = "HABot/Commands/Steering";

  // Publish the message
  MQTT_CLIENT.send(left);
}

function right() {
  // create a new MQTT message with a specific payload
  var right = new Paho.MQTT.Message("right");

  // Set the topic it should be published to
  right.destinationName = "HABot/Commands/Steering";

  // Publish the message
  MQTT_CLIENT.send(right);
}


function forward() {
  // create a new MQTT message with a specific payload
  var forward = new Paho.MQTT.Message("forward");

  // Set the topic it should be published to
  forward.destinationName = "HABot/Commands/Steering";

  // Publish the message
  MQTT_CLIENT.send(forward);
}



function backward() {
  // create a new MQTT message with a specific payload
  var backward = new Paho.MQTT.Message("backward");

  // Set the topic it should be published to
  backward.destinationName = "HABot/Commands/Steering";

  // Publish the message
  MQTT_CLIENT.send(backward);
}



function stop() {
  // create a new MQTT message with a specific payload
  var stop = new Paho.MQTT.Message("stop");

  // Set the topic it should be published to
  stop.destinationName = "HABot/Commands/Steering";

  // Publish the message
  MQTT_CLIENT.send(stop);
}



// This is the function which handles subscribing to topics after a connection is made
function myClientConnected() {
  MQTT_CLIENT.subscribe("<your_random_topic_root>/iot_tutorial/from_esp8266");
}

// This is the function which handles received messages
function myMessageArrived(message) {
  // Get the payload
  var messageBody = message.payloadString;

  // Create a new HTML element wrapping the message payload
  var messageHTML = $("<p>"+messageBody+"</p>");

  // Insert it inside the ```id=updateMe``` element above everything else that is there 
  $("#updateMe").prepend(messageHTML);
};

// Tell MQTT_CLIENT to call myMessageArrived(message) each time a new message arrives
MQTT_CLIENT.onMessageArrived = myMessageArrived;



