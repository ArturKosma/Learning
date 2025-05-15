set DEST_PATH=C:\_programming\Learning\docs

cd /d "%DEST_PATH%"

rmdir /s /q .git
git init
git remote add origin https://github.com/ArturKosma/Learning.git
git checkout -b gh-pages

git add .
git commit -m "Deploy to GitHub Pages"
git push origin gh-pages --force