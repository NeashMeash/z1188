z1188  –  Телефонный справочник Молдовы 2015 (Carte de telefoane din Moldova) 1188

Программа для поиска номеров телефонов абонентов Moldtelecom.
Содержит данные о физических и юридических лицах по состоянию на 2015 год.
Также доступны адреса (только для Кишинева и его пригородов, и Бельц).

Поддерживаемые операционные системы: Windows XP и выше, Ubuntu 14.04, Mac OS X 
(тестировалось на OS X Yosemite 10.10, OS X Mountain Lion 10.8.3 )

Скачать готовые сборки:

Инсталлятор для Windows:
 https://drive.google.com/file/d/0B4cy9KBf7lsMSHQ4NEpnTXJvc1k/view?usp=sharing

Пакет для Ubuntu 14.04 (64 bit):
https://drive.google.com/file/d/0B4cy9KBf7lsMV1E4ejlaSVR2THM/view?usp=sharing

Пакет для Ubuntu 14.04 (32 bit):
https://drive.google.com/file/d/0B4cy9KBf7lsMTy1OU2RuWk9DLW8/view?usp=sharing

Образ для Mac OS X:
https://drive.google.com/file/d/0B4cy9KBf7lsMNkFaSE51V0x4eVk/view?usp=sharing


--- СБОРКА ---

Windows:

Установите Qt 5 for Windows 32-bit (VS). 
http://www.qt.io/download-open-source/
Также требуется установить соответствующую версию Microsoft Visual Studio (под Mingw не тестировалось, но должно работать)

Откройте проект z1188.pro в QtCreator. Нажмите Сборка->Собрать проект (Ctrl+B)


Ubuntu 14.04:

Установите пакет Qt 5 для разработчиков:
sudo apt-get install qt5-default

Зайдите в каталог проекта и выполните команды:
qmake CONFIG+=release
make

Дополнительно установите:
sudo apt-get install qt5-image-formats-plugins



Другие ОС:

Можно установить Qt 5 с помощью инсталлятора (он содержит QtCreator) и собирать с помощью него.

Сама база данных для самостоятельной сборки:
https://drive.google.com/file/d/0B4cy9KBf7lsMSW83dEhfdXBPRkk/view?usp=sharing
