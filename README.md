z1188  –  Телефонный справочник Молдовы
======
Carte de telefoane din Moldova 1188 (2015)

![z1188 –  Телефонный справочник Молдовы 2015](http://fs1.directupload.net/images/150614/2j4iy2k9.png)

Программа для поиска номеров телефонов абонентов из Молдовы.
Содержит данные о физических и юридических лицах по состоянию на 2015 год.
Также доступны адреса (в основном для Кишинева и его пригородов, и Бельц).

ОС: Windows XP и выше, Linux (Ubuntu 14.10, Fedora), Mac OS X 10.7+ (тестировалось на Mountain Lion, Yosemite)

Скачать готовые сборки:

http://pastebin.com/qZxGiyWy

При установке на Linux из архива у вас должны быть установлены библиотеки Qt5 (для Fedora 21 это пакет qt5-base)

--- Инструкция для самостоятельной сборки ---

Windows:

Установите Qt 5 for Windows 32-bit (VS). 
http://www.qt.io/download-open-source/

Также требуется установить соответствующую версию Microsoft Visual Studio (под Mingw не тестировалось, но должно работать)

Откройте проект z1188.pro в QtCreator. Нажмите Сборка->Собрать проект (Ctrl+B)

Либо с помощью командной строки:

``` bash
qmake CONFIG+=release
jom
```

Ubuntu 14.04:

Установите пакет Qt 5 для разработчиков:
sudo apt-get install qt5-default

Зайдите в каталог проекта и выполните команды:
``` bash
qmake CONFIG+=release
make
```

Дополнительно установите:
``` bash
sudo apt-get install qt5-image-formats-plugins
```


Другие ОС:

Можно установить Qt 5 с помощью инсталлятора (он содержит QtCreator) и собирать с помощью него.
