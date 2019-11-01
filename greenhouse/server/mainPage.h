const char MAIN_page[] PROGMEM = R"=====(
<HTML>
  <meta charset="utf-8">
  <meta http-equiv="Content-type" content="text/html;charset=UTF-8"/>
  <TITLE>
    REMOTE LED ON/OFF CONTROL
  </TITLE>
  <BODY>
    <CENTER>
      <h1>Теплица</h1>
      <p>Добро пожаловать на наш сервер!</p>
      <p>Здесь описаны основные ссылки, по которым можно получить информацию<br>с датчиков, либо задать параметры необходимым устройствам.</p>
      <p>Спасибо, что пользуетесь нашим продуктом!<br>С уважением, НИИССУ!</p>
      <h2>REST API:</h2><br>
        "/" - Главная страница<br>
        "/status" - Получение состояния<br>
        "/getdata" - Получение всех данных<br>
        "/light-room?red=%параметр%&green=%параметр%&blue=%параметр%&light=%параметр%" - Включение-отключение света в доме<br>
        "/open-door?door=%параметр%" - Открытие двери<br>
        "/fan?fan=%параметр%" - Вентилятор<br>
        "/pump?pump=%параметр%" - Полив<br>
    </CENTER>
  </BODY>
</HTML>
)=====";
