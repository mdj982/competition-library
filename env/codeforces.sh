if [ $# -ne 1 ]; then
    echo "Error: Specify a single contest name"
else
    cp .vscode/bckup/cf_tasks.json .vscode/tasks.json
    cf race ${1}
    cd cf/contest
    for dir in `ls ${1}`; do
        cp ../../template.cpp ./${1}/${dir}/main.cpp
        code ./${1}/${dir}/main.cpp
    done
fi