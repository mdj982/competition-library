if [ $# -ne 1 ]; then
    echo "Error: Specify a single contest site name"
else
    if [ ${1} = "atcoder" ] || [ ${1} = "ac" ]; then
        cp .vscode/bckup/ac_tasks.json .vscode/tasks.json
        echo "Success: Environment of Atcoder"
    elif [ ${1} = "codeforces" ] || [ ${1} = "cf" ]; then
        cp .vscode/bckup/cf_tasks.json .vscode/tasks.json
        echo "Success: Environment of Codeforces"
    elif [ ${1} = "custom" ]; then
        cp .vscode/bckup/custom_tasks.json .vscode/tasks.json
        echo "Success: Environment of Custom"
    else
        echo "Error: Unknown environment of "${1}
    fi
fi
