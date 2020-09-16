if [ $# -ne 1 ]; then
    echo "Error: Specify a single contest name"
else
    cp .vscode/bckup/ac_tasks.json .vscode/tasks.json
    atcoder-tools gen --workspace ./ --template ./template.cpp --without-login ${1}
    cd ${1}
    for dir in `ls ./`; do
        code ${dir}/main.cpp
    done
fi