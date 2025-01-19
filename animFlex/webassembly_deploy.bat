set BUILD_PATH=C:\_programming\Learning\animFlex\cmake\build
set DEST_PATH=C:\_programming\Learning\docs

cd "%BUILD_PATH%\"

copy animFlex.wasm "%DEST_PATH%\"
copy animFlex.js "%DEST_PATH%\"
copy animFlex.html "%DEST_PATH%\"

cd "%DEST_PATH%\"

del  index.html
rename animFlex.html index.html

call git add .
call git commit -m "bat deploy"
call git push