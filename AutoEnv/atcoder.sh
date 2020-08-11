if [ $# -ne 1 ]; then
    echo "Error: Specify a single contest name"
else
    atcoder-tools gen --workspace ./ --template ./template.cpp --without-login ${1}
fi