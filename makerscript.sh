ScriptDirectory="$(cd "$(dirname "$0")" && pwd)"

mkdir -p "worlds"

rm -rf bin
clear
make all -j$(nproc)
./bin/executable
