// We're going to be using a single click handler for the RGB up/down buttons
// However, it'll send a distinct request for which hue is getting updated
// The arduino code is quite verbose and simplistic, so I figured this would be the best way to minimze coding, on at least one end
// e is the event object. It tells us which button was clicked via the ID of the button clicked
function hue_change(e) { 

} 


// This handler is for changing the brightness
// Logically it is the same as the above
// Could they have been combined? Definitely
// Separated for logic and readability 
function bright_change(e) { 

}


// This is to tell the ESP8266 to start displaying a different mode
// The actual code for the different modes is stored on the controller
// Here, we send the mode that we want to start on the controller as a "parameter"
// The parameter is read and acted on from the controller side
function mode(e) { 

}


// Not sure how its going to be set up, have to search this
// This is going to be the custom colour picker's way of sending the custom color
// The request would send the "R" "G" and "B" values as parameters to the controller
// The parameters are going to be read and then acted on from the controller side
function custom_color_picker() { 

}


// This will be the function that saves the value of the custom color that was just made to the Firebase database
// This can only be used when the credentials to firebase are updated
// Though this could be updated later to be stored in the cookies
function custom_color_save() { 

}


// Opposite of above
function custom_color_delete() { 

}


// This will send a custom color to the controller to be displayed
// This color will be sent as the "R", "G", and "B" parameter
// The controller will read the parameters and then display it on the LED
function custom_color() { 

}
