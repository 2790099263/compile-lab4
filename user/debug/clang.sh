echo "[+]clong make "+$1
clang -O0 -emit-llvm -S $1
