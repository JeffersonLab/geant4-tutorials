let g4version = "11.3.0";


// When the user scrolls the page, execute myFunction
window.onscroll = function() { keep_top_bar() };

// Get the navbar
let topnav = document.getElementById("topnav");

// Get the offset position of the navbar
let sticky = topnav.offsetTop;

// Add the sticky class to the navbar when you reach its scroll position. Remove "sticky" when you leave the scroll position
function keep_top_bar() {
  if (window["pageYOffset"] >= sticky) {
    topnav.classList.add("sticky")
  } else {
  //  topnav.classList.remove("sticky");
  }

}
