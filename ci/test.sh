set -o errexit
cd $(dirname -- "$0"; )
cd ..

./build/c-coroutines
