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

 
// This is going to be the custom colour picker's way of sending the custom color
// The request would send the "R" "G" and "B" values as parameters to the controller
// The parameters are going to be read and then acted on from the controller side

/// =============================================================
// could change this to the input event, to have realtime updating of the leds
// to limit the send rate, could enforce a global variable
// if the variable is false, cant send
// the variable is set to false cant send
// set a timer to reset the variable to true after x amount of time 
// Mainly have to see how the controller handles it and how many requests are made 
// ==============================================================
function custom_color_picker(hex) { 
    var rgb = hexToRgb(hex)
    console.log(rgb)

}


// This will be the function that saves the value of the custom color that was just made to the Firebase database
// This can only be used when the credentials to firebase are updated
// Though this could be updated later to be stored in the cookies
// Going to be storing as a hex value
function custom_color_save() { 
    var value = document.getElementById("colorPicker").value

    // Send value to firebae for saving
}


// Opposite of above
function custom_color_delete(e) { 
    // Will get the id of the delete from the click even
    // Send a delete command to the firebase database via the id
}


// This will send a custom color to the controller to be displayed
// This color will be sent as the "R", "G", and "B" parameter
// The controller will read the parameters and then display it on the LED
function custom_color(e) { 
    var rgb = hexToRgb(e.value)
}



// The color picker sends back the value as a hex string 
// We want this as a RGB value 
// This function, found online at: 
// Converts the hex to rgb 
function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}