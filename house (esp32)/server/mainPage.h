const char MAIN_page[] PROGMEM = R"=====(
<HTML>
  <meta charset="utf-8">
  <meta http-equiv="Content-type" content="text/html;charset=UTF-8"/>
  <TITLE>
    REMOTE LED ON/OFF CONTROL
  </TITLE>
  <BODY>
    <CENTER>
      <h2>Ссылки:</h2><br>
        "/" - Главная страница<br>
        "/getdata" - Получение всех данных<br>
        "/light-room?light=%параметр%" - Включение-отключение света в доме<br>
        "/open-door?door=%параметр%" - Открытие двери<br>
        "/fan?fan=%параметр%" - Вентилятор<br>
    </CENTER>
  </BODY>
</HTML>
)=====";
