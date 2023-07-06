const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
 <! -- WebPage tasarım kodları -->
 <! -- Web sayfamızı const char olarak tanımlayalım ve flash bellek için PROGMEM ile Raw String  (R) kullanalım -->
<HTML>
	<HEAD>
		<meta charset='utf-8'>
		<TITLE>Dr.TRonik YouTube</TITLE>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<link href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" rel="stylesheet">
		<style>
      html {
        font-family: Calibri;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      h1 { font-size: 1.6rem; }
      p { font-size: 2.0rem; }
      .units { font-size: 1.2rem; }
      .dht-labels{
        font-size: 1.5rem;
        vertical-align:middle;
        padding-bottom: 15px;
      }
    </style>
	</HEAD>
	
<BODY>
<body bgcolor=white>
		<center>
	
	<h1><I><B><font color=black>ESP8266 WebServer</font></B> </I></h1>	
	<hr size=1 width=250 align=“center” color=gray>
		
	 <p>		
     <i class="fa fa-thermometer-three-quarters" style="font-size:3.0rem;color:red;"></i> 
     <span class="dht-labels">Sıcaklık : </span> 
     <span id="TemperatureValue">0</span>
     <sup class="units">&deg;C</sup>
    
	 <p>
     <i class="fa fa-tint" style="font-size:3.0rem;color:blue;"></i>
     <span class="dht-labels">Nem : </span>
     <span id="HumidityValue">0</span>
      <sup class="units">%</sup>
	  
	 <p>
     <i class="fa fa-compress-arrows-alt" style="font-size:3.0rem;color:green;"></i> 
     <span class="dht-labels">Basınç : </span>
     <span id="PressureValue">0</span>
     <sup class="units">hPa</sup>   
	
	 <hr size=1 width=250 align=“center” color=gray>	
	<br>
	 <i class="far fa-calendar-check" style="font-size:1.0rem;color:black";></i>     
     <span id="date" style="font-size:1.0rem ; "></span>
	 <br>
	  
	 <i class="far fa-clock" style="font-size:1.0rem;color:purple;"></i>
     <span style="font-size:1.0rem;">Saat: </span>
     <span id="time" style="font-size:1.0rem;"></span>	 
	 <br>
	 <a href=https://www.youtube.com/channel/UCrDlm8ldOOh5cG5prINb77w target=_blank> Dr.TRonik & YouTube...</a>	
	 <i class="fab fa-youtube" style="font-size:1.2rem;color:red;"></i>		  
	
	</center>
	
	<script>
	setInterval(function() {
        getTemperatureData();
        getHumidityData();
        getPressureData();
      }, 5000); 
	
	setInterval(function() {
       Time_Date();
      }, 1000); 
	  
	  function getTemperatureData() {
        var xhttp = new XMLHttpRequest(); <!-- istek oluştur  -->
        xhttp.onreadystatechange = function() {
          <!-- onreadystatechange özelliği , readyState değiştiğinde yürütülecek bir işlevi tanımlar  -->
          if (this.readyState == 4 && this.status == 200) {
            <!-- readyState: 4: Talep bitti , status: 200: yanıt hazır, daha fazlası: https://www.w3schools.com/tags/ref_httpmessages.asp  -->
            document.getElementById("TemperatureValue").innerHTML =
            <!-- TemperatureValue öğesini döndürür -->
            this.responseText;
            <!-- Yanıt verilerini string olarak al -->
          }
        };
        xhttp.open("GET", "readTemperature", true);  <!-- Oluşturulmuş isteği başlatmak için  -->       
        xhttp.send();  <!-- Başlatılmış isteği sunucuya göndermek için  -->
      }
    


	  function getHumidityData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("HumidityValue").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readHumidity", true);
        xhttp.send();
      }

      function getPressureData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("PressureValue").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readPressure", true);
        xhttp.send();
      }
	  
	function Time_Date() {
        var t = new Date();
        document.getElementById("time").innerHTML = t.toLocaleTimeString();
        var d = new Date();
        const dayNames = ["Pazar", "Pazartesi", "Salı", "Çarşamba", "Perşembe", "Cuma","Cumartesi"];
        const monthNames = ["OCAK", "ŞUBAT", "MART", "NİSAN", "MAYIS", "HAZİRAN","TEMMUZ", "AĞUSTOS", "EYLÜL", "EKİM", "KASIM", "ARALIK"];
        document.getElementById("date").innerHTML = d.getDate() + "/" + monthNames[d.getMonth()] + "/" + d.getFullYear()+ "/" + dayNames[d.getDay()];
      }
    </script>
	
</BODY>
</HTML>

)=====";