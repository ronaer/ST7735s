const char mainPage[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<! -- WebPage tasarım kodları -->
<! -- Web sayfamızı const char olarak tanımlayalım ve flash bellek için PROGMEM ile Raw String  (R) kullanalım -->

<html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset='utf-8'>
		<TITLE>PWM WebServer...</TITLE>
    <! -- YouTube Logo css linki -->
    <link href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" rel="stylesheet">

  <style>
    html {
        font-family: Calibri;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      h1 { font-size: 1.6rem; }
      p { font-size: 1.5rem; }	 
    <! -- Slider bar genişlik tanımlama -->
      .slider {  width: 220px; }
    
  </style>

</head>

<body>
<body bgcolor=white>
<CENTER>
        <br>
					<h1><I><B><font color=black>ESP8266 PWM</font></B> </I></h1>	
          <h1><I><B><font color=black>WebServer Kontrol</font></B> </I></h1>	
	<hr size=1 width=250 align=“center” color=red>
        <br>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="254" value="%SLIDERVALUE%" step="1" class="slider" ></p>
  <p><I>PWM Değeri: <span id="textSliderValue">%SLIDERVALUE%</span></I></p>
  <hr size=1 width=250 align=“center” color=red>
        <br>  
				<i class="fab fa-youtube" style="font-size:1.2rem;color:red;"></i>	
				<a href=https://www.youtube.com/channel/UCrDlm8ldOOh5cG5prINb77w target=_blank> Dr.TRonik & YouTube...</a>	
				<i class="fab fa-youtube" style="font-size:1.2rem;color:red;"></i>	
</CENTER>

<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)=====";