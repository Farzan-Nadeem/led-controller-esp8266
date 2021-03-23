// Initialization function
// This will set all the saved colors if there are any
var savedColors = ""
var savedColorsKey = "savedColors"
var savedColorsNum = 0
var savedContainer = null; 

var ESP8266 = "http://esp8266.local/"

window.onload = init()

async function init() {
    savedContainer = document.getElementById("savedColorsContainer")
    while (savedContainer == null) {
        savedContainer = document.getElementById("savedColorsContainer")
        await sleep(50)
    }

    savedColors = getCookie(savedColorsKey)
    if (savedColors.length < 1) return

    savedColorsArray = savedColors.split(",")
    savedColorsNum = savedColorsArray.length;
    savedColorsArray.forEach(color => {
        createSavedColorButton(color)
    })
}

// We're going to be using a single click handler for the RGB up/down buttons
// However, it'll send a distinct request for which hue is getting updated
// The arduino code is quite verbose and simplistic, so I figured this would be the best way to minimze coding, on at least one end
// e is the event object. It tells us which button was clicked via the ID of the button clicked
function hueChange(e) {
    // Send this to controller 
    var url = ESP8266 + "/" + e

    sendRequest(url)
}

// This handler is for changing the brightness
// Logically it is the same as the above
// Could they have been combined? Definitely
// Separated for logic and readability 
function brightChange(e) {
    // Send this to controller
    var url = ESP8266 + "/" + e

    sendRequest(url)
}

// This is to tell the ESP8266 to start displaying a different mode
// The actual code for the different modes is stored on the controller
// Here, we send the mode that we want to start on the controller as a "parameter"
// The parameter is read and acted on from the controller side
function mode(e) {
    // Send this to controller
    var url = ESP8266 + "/mode?mode=" + e

    sendRequest(url)
}

// Going to have an input handler function that will update the controller
// This will be intermittent until I figure out the rate that I can send data to the controller
// This is going to be the blocking boolean
var inputChangeBlocker = false
function custom_color_picker_input(hex) {
    if (inputChangeBlocker) { return; }
    inputChangeBlocker = true;
    
    var rgb = hexToRgb(hex)
    // send to the controller
    var url = ESP8266 + "/color?red=" + rgb.r + "&green=" + rgb.g + "&blue=" + rgb.b
    sendRequest(url)

    // Reset the blocker after some ms, which will open the path for more sends to the controller
    setTimeout(() => {
        inputChangeBlocker = false
    }, 100);
}

// This is going to be the custom colour picker's way of sending the custom color
// The request would send the "R" "G" and "B" values as parameters to the controller
// The parameters are going to be read and then acted on from the controller side
// This will be the event that fires at the end of the changing
function custom_color_picker_change(hex) {
    var rgb = hexToRgb(hex) 

    // Send this to controller
    var url = ESP8266 + "/color?red=" + rgb.r + "&green=" + rgb.g + "&blue=" + rgb.b
    sendRequest(url)
}

// This will be the function that saves the value of the custom color that was just made to the browser cookies
function custom_color_save() {
    var value = document.getElementById("colorPicker").value

    if (savedColors.length < 1) {
        savedColors = value
    } else {
        savedColors += "," + value
    }

    var d = new Date();
    d.setTime(d.getTime() + (10 * 365 * 24 * 60 * 60 * 1000));
    var expires = "expires=" + d.toUTCString();

    // Append to the custom color json and then set the cookie again to update it 
    document.cookie = savedColorsKey + "=" + savedColors + ";" + expires

    // Add the color to the ui 
    createSavedColorButton(value)
}

// Opposite of above, clears everything
function clearSavedColors() {

    if (confirm("Are you sure? There is no way to retrieve these items once lost")) {
        var d = new Date();
        d.setTime(d.getTime() - (10 * 24 * 60 * 1000));
        var expires = "expires=" + d.toUTCString();

        // Append to the custom color json and then set the cookie again to update it 
        document.cookie = savedColorsKey + "=" + savedColors + ";" + expires

        savedContainer.innerHTML = ""
    }
}

// This will send a custom color to the controller to be displayed
// This color will be sent as the "R", "G", and "B" parameter
// The controller will read the parameters and then display it on the LED
function custom_color(e) { 
    var rgb = hexToRgb(e)
   
    // Send this to controller
    var url = ESP8266 + "/color?red=" + rgb.r + "&green=" + rgb.g + "&blue=" + rgb.b
    sendRequest(url) 
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

function createSavedColorButton(color) {
    var classes = "btn-floating btn-large waves-effect waves-light pressable"
    var tag = `<button value="${color}" onclick="custom_color(this.value)" style="background:${color}" class="${classes}"></button>`
    savedContainer.innerHTML += tag
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function getCookie(cname) {
    var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for (var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

function sendRequest(url) { 
    var request = new XMLHttpRequest();  
    request.onload = function () { } 
    request.open("POST", url, true);
    request.send(); 
}