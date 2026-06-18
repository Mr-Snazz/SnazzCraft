ScriptDirectory="$(cd "$(dirname "$0")" && pwd)"

mkdir -p "worlds"

make all -j$(nproc)
./bin/executable
