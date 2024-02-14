var g4version = "11.2.0";

window.onload = function () {

	// sets all tags with name "g4version" to the value of g4version
	// not just the first one
	// not just in the current document but in all included html files
	var g4versions = document.getElementsByName("g4version");
	for (var i = 0; i < g4versions.length; i++) {
		g4versions[i].innerHTML = g4version;
	}



};

// When the user scrolls the page, execute myFunction
window.onscroll = function() { keep_top_bar() };

// Get the navbar
var topnav = document.getElementById("topnav");

// Get the offset position of the navbar
var sticky = topnav.offsetTop;

// Add the sticky class to the navbar when you reach its scroll position. Remove "sticky" when you leave the scroll position
function keep_top_bar() {
  if (window["pageYOffset"] >= sticky) {
    topnav.classList.add("sticky")
  } else {
    topnav.classList.remove("sticky");
  }
}