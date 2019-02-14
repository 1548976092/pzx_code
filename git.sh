#初始化仓库
git config credential.helper store
git config --global user.name "1548976092" 
git config --global user.email "1548976092@qq.com"
git init

#上传readme
touch  readme.txt
git add .
git commit -m "readme.txt" 
git log
git status

#推送远程仓库
git remote add origin https://github.com/1548976092/git_go.git
git push -u origin master
