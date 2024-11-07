set -o errexit
cd $(dirname -- "$0"; )
cd ..

mkdir -p build

CFLAGS="
    -fsanitize=address,undefined
    -Wall -Werror -Wsign-compare
    -fno-sanitize-recover=all
    -std=c2x -I./include
"
for filename in test/*.c; do
  for opt in 0 1 2 3; do
    clang $CFLAGS -O$opt -g $filename -o build/a
    ./build/a
    echo -e "[test] [OK] $filename \t -O$opt."
  done
done
