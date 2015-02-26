z1188  –  Телефонный справочник Молдовы
======
Carte de telefoane din Moldova 1188 (2015)

![z1188 –  Телефонный справочник Молдовы 2015](http://fs1.directupload.net/images/150226/iot43kit.png)

Программа для поиска номеров телефонов абонентов из Молдовы.
Содержит данные о физических и юридических лицах по состоянию на 2015 год.
Также доступны адреса (в основном для Кишинева и его пригородов, и Бельц).

Поддерживаемые операционные системы: Windows XP и выше, Linux (Ubuntu 14.10, Fedora), Mac OS X 10.7+ (тестировалось на Mountain Lion, Yosemite)

Скачать готовые сборки:

<a href="https://drive.google.com/file/d/0B4cy9KBf7lsMYkxYempCZlpISXM/view?usp=sharing">z1188_1.1_setup.exe</a> (инсталлятор для Windows)

<a href="https://drive.google.com/file/d/0B4cy9KBf7lsMbk1MbktLRGNEdWc/view?usp=sharing">z1188_1.1_amd64.deb</a> (пакет для Ubuntu 64-bit)

<a href="https://drive.google.com/file/d/0B4cy9KBf7lsMZldVTzVDN013QUk/view?usp=sharing">z1188_1.1_i386.deb</a> (пакет для Ubuntu 32-bit)

<a href="https://drive.google.com/file/d/0B4cy9KBf7lsMSDJ6TGYxdnZvX3M/view?usp=sharing">z1188_1.1_x86_64.dmg</a> (образ диска для OS X 10.7+)

<a href="https://drive.google.com/file/d/0B4cy9KBf7lsMR2VGSFViMHdqcms/view?usp=sharing">z1188_1.1_amd64_linux_generic.7z</a> (архив для Fedora 21 или других Linux 64-bit)

<a href="https://drive.google.com/file/d/0B4cy9KBf7lsMNHJGRXU1MndWVVE/view?usp=sharing">z1188_1.1_i386_linux_generic.7z</a> (архив для Fedora 21 или других Linux 32-bit)

При установке на Linux из архива у вас должны быть установлены библиотеки Qt5 (для Fedora 21 это пакет qt5-base)


<a href="https://drive.google.com/file/d/0B4cy9KBf7lsMbDVCNzJKVExzYUE/view?usp=sharing">Сама база данных версии 1.1</a> (в формате sqlite3)

--- Инструкция для самостоятельной сборки ---

Windows:

Установите Qt 5 for Windows 32-bit (VS). 
http://www.qt.io/download-open-source/

Также требуется установить соответствующую версию Microsoft Visual Studio (под Mingw не тестировалось, но должно работать)

Откройте проект z1188.pro в QtCreator. Нажмите Сборка->Собрать проект (Ctrl+B)


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

<b>Старые версии:</b>

<i>v1.0</i>

Инсталлятор для Windows:
https://drive.google.com/file/d/0B4cy9KBf7lsMZW1laVptU0p1b3c/view?usp=sharing

Пакет для Ubuntu 14.04 (64 bit):
https://drive.google.com/file/d/0B4cy9KBf7lsMV1E4ejlaSVR2THM/view?usp=sharing

Пакет для Ubuntu 14.04 (32 bit):
https://drive.google.com/file/d/0B4cy9KBf7lsMTy1OU2RuWk9DLW8/view?usp=sharing

Образ для Mac OS X:
https://drive.google.com/file/d/0B4cy9KBf7lsMNkFaSE51V0x4eVk/view?usp=sharing

Сама база данных версии 1.0 (в формате sqlite3):
https://drive.google.com/file/d/0B4cy9KBf7lsMSW83dEhfdXBPRkk/view?usp=sharing
