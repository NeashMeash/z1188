AppVersion="1.1"

# Get the machine Architecture
Architecture=$(uname -m)
case "$Architecture" in
    x86)    Architecture="i386"                  ;;
    ia64)   Architecture="ia64"                 ;;
    i?86)   Architecture="i386"                  ;;
    amd64)  Architecture="amd64"                    ;;
    x86_64) Architecture="amd64"                   ;;
    sparc64)    Architecture="sparc64"                  ;;
* ) echo    "Your Architecture '$Architecture' -> ITS NOT SUPPORTED."   
exit
;;
esac

echo "Detected Architecture : $Architecture"

cp z1188/DEBIAN/control_ z1188/DEBIAN/control
 sed -i "s/YOUR_ARCHITECTURE/$Architecture/g" z1188/DEBIAN/control
mkdir  -p ./z1188/usr/share/z1188/languages/
mkdir -p ./z1188/usr/share/z1188/languages/qt
cp -r ../../languages/*.qm ./z1188/usr/share/z1188/languages/
cp -r ../../languages/qt/*.qm ./z1188/usr/share/z1188/languages/qt/
cp ../../database/db.db ./z1188/usr/share/z1188/
cp ../../build/linux/${Architecture}/release/executable/z1188 ./z1188/usr/bin/
if [ ! -f ./z1188/usr/bin/z1188 ]; then
    echo "Executable not found!"
    exit
fi

if [ ! -f ./z1188/usr/share/z1188/db.db ]; then
    echo "Database file db.db not found"
    exit
fi

fakeroot dpkg-deb --build z1188/ "z1188_${AppVersion}_${Architecture}.deb"