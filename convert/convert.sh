rm -f *.sql ../database/db.db sqlitecmd.txt
echo "Dumping structure..."
./mysql2sqlite.sh --no-data -u root my1188 --default-character-set=utf8 > NEW_structure_dump.sql
echo "Dumping data..."
./mysql2sqlite.sh  --disable-keys --no-create-info --default-character-set=utf8 -u root my1188  > NEW_data_dump.sql
csplit  -f NEW_structure_dump -b '%d.sql' NEW_structure_dump.sql "/CREATE INDEX/" > /dev/null
echo "END TRANSACTION;" >> NEW_structure_dump0.sql
printf  "BEGIN TRANSACTION;\n" > NEW_structure_dump_indexes.sql
cat NEW_structure_dump1.sql >> NEW_structure_dump_indexes.sql
echo "Converting timestamps..."
php -f convert_timestamps.php NEW_data_dump.sql NEW_data_dump_converted.sql
echo ".read NEW_structure_dump0.txt" > sqlitecmd.txt
echo ".read NEW_data_dump_converted.sql" >> sqlitecmd.txt
echo "Loading dump into database..."
sqlite3 ../database/db.db < sqlitecmd.txt
echo "Converting diacritics..."
../build/windows/release/executable/z1188.exe -f ../database/db.db -n
echo "Creating indexes..."
echo "delete from regions where id = 3000;"  | sqlite3 ../database/db.db
echo ".read NEW_structure_dump_indexes.txt" | sqlite3 ../database/db.db
echo "Finished!"

