set -o errexit
cd $(dirname -- "$0"; )
cd ..

rm -rf build
rm -rf .cache
rm -r compile_commands.json
